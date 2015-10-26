//
// Created by Maxym on 10/26/2015.
//

#include "FSettings.hpp"
#define ADD_DEFAULT_SETTING(map_name,enum_name) {map_name[enum_name] = boost::algorithm::to_lower_copy(std::string(#enum_name));}

const FSettings::DefaultSettingsMap FSettings::default_settings_ = [] {
    FSettings::DefaultSettingsMap ret_map;

    ADD_DEFAULT_SETTING(ret_map, Pedantic);

    return ret_map;
}();

void FSettings::LoadDefaultSettings()
{
    Set<bool>(SettingsNames::Pedantic, true);
}
