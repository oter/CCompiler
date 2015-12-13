#include "asm_context.hpp"

std::string AsmContext::GetLabel(const std::string &prefix)
{
    std::string name = "." + prefix + '_' + std::to_string(labels_count());
    this->labels_count_++;
    return name;
}
