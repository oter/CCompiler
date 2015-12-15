//
// Created by Maxym on 12/12/2015.
//
#include "asm_generator.hpp"

//Disable warnings when use deprecated features like auto_ptr in boost.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/format.hpp>
#pragma GCC diagnostic pop

decltype(AsmGenerator::asm_indent_) AsmGenerator::asm_indent_ = "    ";

AsmGenerator::AsmGenerator(std::ostream& out, TreeSyntaxShared root) : out_(out), status_(0)
{
    PutHeader();
    auto ctx = std::make_shared<AsmContext>();
    Generate(root, ctx);
    PutEnding();
    this->status_ = 0;
}

void AsmGenerator::Generate(TreeSyntaxShared root, AsmContextShared& ctx)
{
    switch (root->type())
    {
        case TreeSyntaxType::kImmediate:
            PutInstruction("mov", (boost::format("$%1%, %%eax") % std::static_pointer_cast<TreeImmediate>(root)->value()).str());
            break;
        case TreeSyntaxType::kVariable:
            PutInstruction("mov", (boost::format("%1%(%%ebp), %%eax") % ctx->current_scope()->GetVariable(std::static_pointer_cast<TreeVariable>(root)->name())->offset()).str());
            break;
        case TreeSyntaxType::kUnaryOperator:
        {
            auto expr = std::static_pointer_cast<TreeUnaryExpression>(root);
            // Expand expression first
            // TODO: Add unary operations for ++ and --
            Generate(expr->expression(), ctx);
            if (expr->expr_type() == TreeUnaryExpressionType::kBitwiseNegation)
            {
                PutInstruction("not", "%eax");
            } else if (expr->expr_type() == TreeUnaryExpressionType::kLogicalNegation)
            {
                PutInstruction("test", "$0xFFFFFFFF, %eax");
                PutInstruction("setz", "%al");
            }
            break;
        }
        case TreeSyntaxType::kBinaryOperator:
        {
            auto binary = std::static_pointer_cast<TreeBinaryExpression>(root);
            int offset = ctx->stack_offset();
            ctx->set_stack_offset(offset - AsmContext::kWordSize);

            PutInstruction("sub", "$4, %esp");
            Generate(binary->rhs(), ctx);

            switch (binary->expr_type())
            {
                case TreeBinaryExpressionType::kDivision:
                    PutInstruction("sub", (boost::format("%%eax, %1%(%%ebp)") % offset).str());
                    PutInstruction("mov", (boost::format("%1%(%%ebp), %%eax") % offset).str());
                    break;
                case TreeBinaryExpressionType::kMultiplication:
                    PutInstruction("mull", (boost::format("%1%(%%ebp)") % offset).str());
                    break;
                case TreeBinaryExpressionType::kAddition:
                    PutInstruction("add", (boost::format("%1%(%%ebp), %%eax") % offset).str());
                    break;
                case TreeBinaryExpressionType::kSubtraction:
                    PutInstruction("sub", (boost::format("%%eax, %1%(%%ebp)") % offset).str());
                    PutInstruction("mov", (boost::format("%1%(%%ebp), %%eax") % offset).str());
                    break;
                case TreeBinaryExpressionType::kLessThan:
                    PutInstruction("cmp", (boost::format("%%eax, %1%(%%ebp)") % offset).str());
                    PutInstruction("setl", "%al");
                    PutInstruction("movzbl", (boost::format("%al, %eax") % offset).str());
                    break;
                case TreeBinaryExpressionType::kLessThanOrEqual:
                    PutInstruction("cmp", (boost::format("%%eax, %1%(%%ebp)") % offset).str());
                    PutInstruction("setle", "%al");
                    PutInstruction("movzbl", (boost::format("%al, %eax") % offset).str());
                    break;
            }
            break;
        }
        case TreeSyntaxType::kStatementsBlock:
        {
            auto stmts = std::static_pointer_cast<TreeStatementsBlock>(root)->stmts();
            for (auto stmt : stmts)
            {
                // TODO: change ctx?
                Generate(stmt, ctx);
            }
            break;
        }
        case TreeSyntaxType::kIfStatement:
        {
            // TODO: if statement
            break;
        }
        case TreeSyntaxType::kReturnStatement:
            Generate(std::static_pointer_cast<TreeReturnStatement>(root)->expression(), ctx);
            //PutFunctionEpilogue(); // TODO: is it needed?
            break;
        case TreeSyntaxType::kDefineVariable:
        {
            auto var = std::static_pointer_cast<TreeDefineVariable>(root);
            int offset = ctx->stack_offset();
            auto new_var = std::make_shared<AsmVariable >(var->name(), offset);
            ctx->current_scope()->AddVariable(new_var);
            PutInstruction("sub", "$4, %esp");
            ctx->set_stack_offset(offset - AsmContext::kWordSize);
            Generate(var->init_value(), ctx);
            PutInstruction("mov", (boost::format("%%eax, %1%(%%ebp)") % offset).str());
            break;
        }
        case TreeSyntaxType::kFunction:
        {
            auto func = std::static_pointer_cast<TreeFunction>(root);
            ctx = std::make_shared<AsmContext>();
            ctx->set_current_scope(std::make_shared<AsmScope>());

            PutFunctionDeclaration(func->name());
            PutFunctionPrologue();
            Generate(func->body(), ctx);
            PutFunctionEpilogue();
            break;
        }
        case TreeSyntaxType::kFunctionCall:
            // TODO: push arguments
            PutInstruction("call", std::static_pointer_cast<TreeFunctionCall>(root)->name());
            break;
        case TreeSyntaxType::kFunctionArgument:break;
        case TreeSyntaxType::kFunctionArguments:break;
        case TreeSyntaxType::kAssignment:
        {
            auto assign = std::static_pointer_cast<TreeAssignment>(root);
            Generate(assign->expr(), ctx);
            PutInstruction("mov", (boost::format("%%eax, %1%(%%ebp)") % ctx->current_scope()->GetVariable(assign->var_name())->offset()).str());
            break;
        }
        case TreeSyntaxType::kWhileStatement:
        {
            auto while_stmt = std::static_pointer_cast<TreeWhileStatement>(root);

            auto start_label = ctx->GetLabel("while_start_ctx");
            auto end_label = ctx->GetLabel("while_end_ctx");

            PutLabel(start_label);
            Generate(while_stmt->condition(), ctx);

            PutInstruction("test", "%eax, %eax");
            PutInstruction("jz", end_label);

            Generate(while_stmt->body(), ctx);
            PutInstruction("jmp", start_label);
            PutLabel(end_label);
            break;
        }
        case TreeSyntaxType::kTopLevel:
        {
            auto top = std::static_pointer_cast<TreeTopLevel>(root);
            for (auto decl : top->decls())
            {
                Generate(decl, ctx);
            }
            break;
        }
        case TreeSyntaxType::kArrayIndexer:break;
        case TreeSyntaxType::kArrayAssignment:break;
        case TreeSyntaxType::kEmptyStatement:break;
        case TreeSyntaxType::kTreeForStatement:break;
    }
}

void AsmGenerator::PutInstruction(const std::string &instr, const std::string& args)
{
    this->out_ << asm_indent_ << instr;
    std::string instr_args_indent_str(kMaxInstructionLength - instr.length() + kMinInstrArgsIndent, ' ');
    this->out_ << instr_args_indent_str << args << "\n";
}

void AsmGenerator::PutHeader()
{
    this->out_ << asm_indent_ << ".code32\n" << asm_indent_ << ".text\n";
}

void AsmGenerator::PutEnding()
{
    PutFunctionDeclaration("_start");
    PutFunctionPrologue();
    PutInstruction("call", "main");
    PutInstruction("mov", "%eax, %ebx");
    PutInstruction("mov", "$1, %eax");
    // TODO: Check return
    PutFunctionEpilogue();
}

void AsmGenerator::PutFunctionDeclaration(const std::string &name)
{
    this->out_ << asm_indent_ << ".global " << name << "\n";
    this->out_ << name << ":\n";
}

void AsmGenerator::PutFunctionPrologue()
{
    PutInstruction("pushl", "%ebp");
    PutInstruction("mov", "%esp, %ebp");
}

void AsmGenerator::PutFunctionEpilogue()
{
    this->out_ << asm_indent_ << "leave\n";
    this->out_ << asm_indent_ << "ret\n";
}

void AsmGenerator::PutLabel(const std::string &name)
{
    this->out_ << name << ":\n";
}


