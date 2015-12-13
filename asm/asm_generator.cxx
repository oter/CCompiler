//
// Created by Maxym on 12/12/2015.
//
#include "asm_generator.hpp"
#include "asm_context.hpp"

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
    Generate(out, root, ctx);
    PutEnding();
}

void AsmGenerator::Generate(std::ostream &out, TreeSyntaxShared root, AsmContextShared ctx)
{

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


