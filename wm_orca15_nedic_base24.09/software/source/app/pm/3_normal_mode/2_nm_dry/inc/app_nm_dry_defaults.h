#ifndef APP_NM_DRY_DEFAULTS_H
#define APP_NM_DRY_DEFAULTS_H

#include "def.h"
#include "app_nm_dry_user_interface.h"
#include "app_nm_user_interface.h"

// regular
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseRegularDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseRegularLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseRegularHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

// wool
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseWoolDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseWoolLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseWoolHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

// sensitive
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseSensitiveDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseSensitiveLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseSensitiveHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

// quick
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseQuickDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseQuickLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseQuickHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_90_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

// hygiene
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseHygieneDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseHygieneLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseHygieneHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

// heavy
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseHeavyDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseHeavyLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseHeavyHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

// tub wash
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseTubWashDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseTubWashLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseTubWashHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

// babycare
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseBabyCareDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseBabyCareLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseBabyCareHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

// blanket
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseBlanketDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseBlanketLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseBlanketHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

// low temp dry
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseLowTempDryDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseLowTempDryLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseLowTempDryHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

// easy iron
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseEasyIronDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseEasyIronLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseEasyIronHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6}};

// tub dry
static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseTubDryDefaultData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseTubDryLowLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

static const app_nm_dry_userInterafce_defaults_data_t app_dry_defaults_courseTubDryHighLimitData[APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM] = {
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0},
    {.defaultDryTime = 0, .defaultDryIntensity = 0, .defaultDryWaterLevel = 0},
    {.defaultDryTime = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN, .defaultDryIntensity = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE, .defaultDryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0}};

#endif
