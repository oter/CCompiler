//
// Created by Maxym on 10/25/2015.
//

#include "PreprocessorDefine.hpp"

PreprocessorDefine::PreprocessorDefine(const std::string &name, const std::string &value)
{
    set_name(name);
    set_value(value);
    set_defined(true);
}

void PreprocessorDefine::set_defined(bool defined) noexcept
{
    this->defined_ = defined;
}


void PreprocessorDefine::set_name(const std::string &name) noexcept
{
    this->name_ = name;
}

const std::string &PreprocessorDefine::name() const noexcept
{
    return this->name_;
}

bool PreprocessorDefine::defined() const noexcept
{
    return this->defined_;
}

void PreprocessorDefine::set_value(const std::string &string) noexcept
{
    this->value_ = value_;
}

const std::string &PreprocessorDefine::value() const noexcept
{
    return this->value_;
}
