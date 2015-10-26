//
// Created by Maxym on 10/26/2015.
//

#ifndef CCOMPILER_SETTINGS_HPP
#define CCOMPILER_SETTINGS_HPP

#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <map>
#include <memory>
#include <assert.h>

#include <boost/algorithm/string/case_conv.hpp>

class FSettingInterface
{
public:
    virtual ~FSettingInterface() {};
};

template<typename T>
class FSettingField : public FSettingInterface
{

public:

    FSettingField() : t_(typeid(T)) {}
    FSettingField(const T& value) : t_(typeid(T)) { set_value(value); }
    FSettingField(const FSettingField& ref) : t_(ref.t_), value_(ref.value_) {}
    ~FSettingField() {};

    const T& value() {
        CheckTypes();
        return this->value_;
    }
    void set_value(const T& v) {
        CheckTypes();
        this->value_ = v;
    }
private:
    void CheckTypes() {
        auto a = typeid(T) == t_;
        if (!a) {
            std::cout << "Incompatible types: holds(" << typeid(T).name() << ") and requests(" << t_.name() << ")." << std::endl;
        }
        assert(a);
    }
    const std::type_info& t_;
    T value_;
};

enum SettingsNames {
    Pedantic
};

class FSettings
{
public:
    FSettings() {}

    virtual ~FSettings() {}

    virtual void LoadDefaultSettings();

    using DefaultSettingsMap = std::map<SettingsNames, std::string>;

    template<typename SettingType, typename T>
    void Set(T setting, FSettingField<SettingType> value) {
        values_[boost::algorithm::to_lower_copy(GetSettingName(setting))] = std::make_unique<FSettingField<SettingType>>(value);
    }

    template<typename V, typename T>
    const V& Get(T setting) {
        auto s = GetSettingName(setting);
        const auto &found = values_.find(s);
        assert(found != values_.end());
        return static_cast<FSettingField<V>*>(found->second.get())->value();
    }

private:
    inline auto GetSettingName(SettingsNames name)
    {
        const auto &found = FSettings::default_settings_.find(name);
        assert(found != FSettings::default_settings_.end());
        return found->second;
    }

    inline auto GetSettingName(std::string name)
    {
        return boost::algorithm::to_lower_copy(name);
    }

private:
    std::map<std::string, std::unique_ptr<FSettingInterface>> values_;
    static const DefaultSettingsMap default_settings_;
};

#endif //CCOMPILER_SETTINGS_HPP
