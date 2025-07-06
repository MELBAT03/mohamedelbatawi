#ifndef APP_NM_TUBCLEAN_DEFAULTS_H
#define APP_NM_TUBCLEAN_DEFAULTS_H

#include "def.h"
#include "app_nm_tubclean_user_interface.h"

static const app_nm_tubclean_userInterface_defaults_data_t app_tubclean_defaults[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_WARM, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_ON},
    
    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_WARM, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_ON},

    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_WARM, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_ON},
    
    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_WARM, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_ON},

    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_WARM, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_ON}};

// NOTE: LOWER LIMIT FOR SELECTION
static const app_nm_tubclean_userInterface_defaults_data_t app_tubclean_lowLimits[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_WARM, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_OFF},

    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_WARM, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_OFF},

    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_WARM, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_OFF},

    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_WARM, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_OFF},

    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_WARM, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_OFF}};

// NOTE: UPPER LIMIT FOR SELECTION
static const app_nm_tubclean_userInterface_defaults_data_t app_tubclean_highLimits[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_HOT, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_ON},

    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_HOT, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_ON},

    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_HOT, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_ON},
    
    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_HOT, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_ON},

    {.defaultWaterTemp = APP_NORMAL_MODE_UI_WATER_TEMP_HOT, .defaultSteamTech = APP_NORMAL_MODE_UI_STEAM_TECH_ON}};

#endif
