//
// Created by Maxym on 12/12/2015.
//

#include <assert.h>
#include "asm_scope.hpp"

void AsmScope::AddVariable(AsmVariableShared var)
{
    this->size_++;
    variables_.push_back(var);
}

AsmVariableShared AsmScope::GetVariable(size_t offset)
{
    return GetVariable([&](const AsmVariable &var)
                       {
                           return var.offset() == offset;
                       });
}

AsmVariableShared AsmScope::GetVariable(std::function<bool(const AsmVariable &)> comparator)
{
    for (const auto var : this->variables_)
    {
        if (comparator(*var))
        {
            return var;
        }
    }
    assert(0 && "Cannot find variable!");
    return std::shared_ptr<AsmVariable>();
}

AsmVariableShared AsmScope::GetVariable(const std::string &name)
{
    return GetVariable([&](const AsmVariable &var)
                       {
                           return !var.name().compare(name);
                       });
}
