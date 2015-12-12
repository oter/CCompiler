//
// Created by Maxym on 12/12/2015.
//

#ifndef CCOMPILER_ASM_CONTEXT_HPP
#define CCOMPILER_ASM_CONTEXT_HPP

#include "asm_scope.hpp"

class AsmContext
{
    enum : int
    {
        WORD_SIZE = 4
    };
public:
    AsmContext() : stack_offset_(-WORD_SIZE), labels_count_(0) {}

    int stack_offset() const noexcept { return this->stack_offset_; }

    size_t labels_count() const noexcept { return this->labels_count_; }

private:
    int stack_offset_;
    size_t labels_count_;
    std::shared_ptr<AsmScope> current_scope_;

};


#endif //CCOMPILER_ASM_CONTEXT_HPP
