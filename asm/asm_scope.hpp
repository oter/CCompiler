//
// Created by Maxym on 12/12/2015.
//

#ifndef CCOMPILER_ASM_SCOPE_HPP
#define CCOMPILER_ASM_SCOPE_HPP

#include <functional>

#include "asm_variable.hpp"

class AsmScope
{
public:
    AsmScope() : size_(0), variables_(0) {}

    size_t size() const noexcept { return this->size_; }

    const AsmVariables & variables() const noexcept { this->variables_; }

    void AddVariable(AsmVariableShared var);

    AsmVariableShared GetVariable(size_t offset);

    AsmVariableShared GetVariable(const std::string& name);

    AsmVariableShared GetVariable(std::function<bool(const AsmVariable &)> comparator);

private:
    size_t size_;
    AsmVariables variables_;
};

#endif //CCOMPILER_ASM_SCOPE_HPP
