//
// Created by Maxym on 12/12/2015.
//

#ifndef CCOMPILER_ASMVARIABLE_HPP
#define CCOMPILER_ASMVARIABLE_HPP

#include <string>
#include <vector>
#include <memory>

class AsmVariable
{
public:
    explicit AsmVariable(const std::string& name, int offset) : name_(name), offset_(offset) {};

    const std::string& name() const noexcept { return this->name_; }

    int offset() const noexcept { return this->offset_; }

private:
    std::string name_;
    int offset_;
};

using AsmVariableShared = std::shared_ptr<AsmVariable>;
using AsmVariables = std::vector<AsmVariableShared>;


#endif //CCOMPILER_ASMVARIABLE_HPP
