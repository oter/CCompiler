//
// Created by Maxym on 12/12/2015.
//

#ifndef CCOMPILER_ASM_GENERATOR_HPP
#define CCOMPILER_ASM_GENERATOR_HPP

#include <iostream>
#include "../tree_syntax/tree_syntax.hpp"
#include "asm_context.hpp"

class AsmGenerator
{
    enum
    {
        kMaxInstructionLength = 6,
        kMinInstrArgsIndent = 3
    };
public:
    explicit AsmGenerator(std::ostream& out, TreeSyntaxShared root);

    int status() const noexcept {return this->status_; }

private:
    void Generate(TreeSyntaxShared root, AsmContextShared& ctx);
    void PutHeader();
    void PutEnding();
    void PutFunctionDeclaration(const std::string& name);
    void PutFunctionPrologue();
    void PutFunctionEpilogue();
    void PutLabel(const std::string& name);
    void PutInstruction(const std::string& instr, const std::string& args);

private:
    // TODO: Replace with std::setfill('*') << std::setw(12)  ??
    static const std::string asm_indent_;
    std::ostream& out_;
    int status_;
};

#endif //CCOMPILER_ASM_GENERATOR_HPP
