//
// Created by Maxym on 10/25/2015.
//

#ifndef CCOMPILER_PREPROCESSORDEFINE_HPP
#define CCOMPILER_PREPROCESSORDEFINE_HPP

#include<string>

class PreprocessorDefine
{
public:
    PreprocessorDefine() = default;

    PreprocessorDefine(const std::string &name, const std::string &value);

    void set_defined(bool defined) noexcept;

    bool defined() const noexcept;

    void set_name(const std::string &name) noexcept;

    const std::string &name() const noexcept;

    void set_value(const std::string &value) noexcept;

    const std::string& value() const noexcept;

private:
    std::string name_;
    std::string value_;
    bool defined_;
};


#endif //CCOMPILER_PREPROCESSORDEFINE_HPP
