//
// Created by Maxym on 12/12/2015.
//

#ifndef CCOMPILER_ASM_CONTEXT_HPP
#define CCOMPILER_ASM_CONTEXT_HPP

#include "asm_scope.hpp"

class AsmContext
{
public:
    enum : int
    {
        kWordSize = 4
    };

    AsmContext() : stack_offset_(-kWordSize), labels_count_(0) {}

    void set_stack_offset(int offset) { this->stack_offset_ = offset; }

    int stack_offset() const noexcept { return this->stack_offset_; }

    size_t labels_count() const noexcept { return this->labels_count_; }

    std::string GetLabel(const std::string&prefix);

    std::shared_ptr<AsmScope> current_scope() { return this->current_scope_; }

    void set_current_scope(std::shared_ptr<AsmScope> scope) { this->current_scope_ = scope; }

private:
    int stack_offset_;
    size_t labels_count_;
    std::shared_ptr<AsmScope> current_scope_;
};

using AsmContextShared = std::shared_ptr<AsmContext>;

#endif //CCOMPILER_ASM_CONTEXT_HPP
