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

AsmGenerator::AsmGenerator(std::ostream& out, TreeSyntaxShared root) : out_(out)
{
    PutHeader();
    auto ctx = std::make_shared<AsmContext>();
    Generate(root, ctx);
    PutEnding();
}

void AsmGenerator::Generate(TreeSyntaxShared root, AsmContextShared ctx)
{
    switch (root->type())
    {
        case TreeSyntaxType::kImmediate:break;
        case TreeSyntaxType::kVariable:break;
        case TreeSyntaxType::kUnaryOperator:
        {
            auto expr = std::static_pointer_cast<TreeUnaryExpression>(root);
            // Expand expression first
            // TODO: Add unary operations for ++ and --
            Generate(expr->expr(), ctx);
            if (expr->type() == TreeUnaryExpressionType::kBitwiseNegation)
            {
                PutInstruction("not", "%eax");
            } else if (expr->type() == TreeUnaryExpressionType::kLogicalNegation)
            {
                PutInstruction("test", "$0xFFFFFFFF, %eax");
                PutInstruction("setz", "%al");
            }
            break;
        }
        case TreeSyntaxType::kBinaryOperator:break;
        case TreeSyntaxType::kStatementsBlock:break;
        case TreeSyntaxType::kIfStatement:break;
        case TreeSyntaxType::kReturnStatement:break;
        case TreeSyntaxType::kDefineVariable:break;
        case TreeSyntaxType::kFunction:break;
        case TreeSyntaxType::kFunctionCall:break;
        case TreeSyntaxType::kFunctionArgument:break;
        case TreeSyntaxType::kFunctionArguments:break;
        case TreeSyntaxType::kAssignment:break;
        case TreeSyntaxType::kWhileStatement:break;
        case TreeSyntaxType::kTopLevel:break;
    }
}

void AsmGenerator::PutInstruction(const std::string &instr, const std::string& args)
{
    this->out_ << asm_indent_ << instr;
    std::string instr_args_indent_str;
    instr_args_indent_str.append(' ', kMaxInstructionLength - instr.length() + kMinInstrArgsIndent);
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
    PutInstruction("mov", "%1, %eax");
    // TODO: Check return
    PutFunctionEpilogue();
}

void AsmGenerator::PutFunctionDeclaration(const std::string &name)
{
    this->out_ << asm_indent_ << ".global " << name << "\n";
    this->out_ << name << ":";
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


