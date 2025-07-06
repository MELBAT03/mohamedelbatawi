#ifndef APP_NM_DRY_OPERATION_TABLE_H
#define APP_NM_DRY_OPERATION_TABLE_H

#include "def.h"

#include "app_nm_dry.h"

static const app_dry_operation_timeTable_t app_dry_operation_30MinManualTimeTable[APP_NORMAL_MODE_DRY_MAX_WATER_LEVEL] = {
    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 300000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 300000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 300000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 300000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 300000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 300000U, 0, 0, 180000U, 300000U}}};

static const app_dry_operation_timeTable_t app_dry_operation_60MinManualTimeTable[APP_NORMAL_MODE_DRY_MAX_WATER_LEVEL] = {
    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 2100000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 2100000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 2100000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 2100000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 2100000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 2100000U, 0, 0, 180000U, 300000U}}};

static const app_dry_operation_timeTable_t app_dry_operation_90MinManualTimeTable[APP_NORMAL_MODE_DRY_MAX_WATER_LEVEL] = {
    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 3900000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 3900000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 3900000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 3900000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 3900000U, 0, 0, 180000U, 300000U}},

    {{0, 10000U, 600000U, 300000U, 30000U, 120000U, 300000U, 3900000U, 0, 0, 180000U, 300000U}}};

static const app_dry_operation_timeTable_t app_dry_operation_autoTimeTable[APP_NORMAL_MODE_DRY_MAX_WATER_LEVEL] = {
    {{0, 10000U, 1200000U, 600000U, 30000U, 120000U, 900000U, 900000U, 5400000U, 0, 180000U, 300000U}},

    {{0, 10000U, 1200000U, 600000U, 30000U, 120000U, 900000U, 900000U, 7200000U, 0, 180000U, 300000U}},

    {{0, 10000U, 1200000U, 600000U, 30000U, 120000U, 900000U, 900000U, 9000000U, 0, 180000U, 300000U}},

    {{0, 10000U, 1200000U, 600000U, 30000U, 120000U, 900000U, 900000U, 10800000U, 0, 180000U, 300000U}},

    {{0, 10000U, 1200000U, 600000U, 30000U, 120000U, 900000U, 900000U, 12600000U, 0, 180000U, 300000U}},

    {{0, 10000U, 1200000U, 600000U, 30000U, 120000U, 900000U, 900000U, 14400000U, 0, 180000U, 300000U}}};

static const uint32_t regular_dry_operation_autoCondense2VsCourseTable[] = {
    5400000U,
    7200000U,
    9000000U,
    10800000U,
    12600000U,
    14400000U};

static const uint32_t wool_dry_operation_autoCondense2VsCourseTable[] = {
    0,
    0,
    0,
    0,
    0,
    0};

static const uint32_t sensitive_dry_operation_autoCondense2VsCourseTable[] = {
    0,
    0,
    0,
    0,
    0,
    0};

static const uint32_t quick_dry_operation_autoCondense2VsCourseTable[] = {
    0,
    0,
    0,
    0,
    0,
    0};

static const uint32_t tubWash_dry_operation_autoCondense2VsCourseTable[] = {
    0,
    0,
    0,
    0,
    0,
    0};

static const uint32_t blanket_dry_operation_autoCondense2VsCourseTable[] = {
    0,
    0,
    0,
    0,
    0,
    0};

static const uint32_t lowTempDry_dry_operation_autoCondense2VsCourseTable[] = {
    10800000U,
    14400000U,
    18000000U,
    0,
    0,
    0};

static const uint32_t hygiene_dry_operation_autoCondense2VsCourseTable[] = {
    5400000U,
    7200000U,
    9000000U,
    10800000U,
    0,
    0};

static const uint32_t heavy_dry_operation_autoCondense2VsCourseTable[] = {
    7200000U,
    9000000U,
    10800000U,
    12600000U,
    14400000U,
    16200000U};

static const uint32_t babyCare_dry_operation_autoCondense2VsCourseTable[] = {
    5400000U,
    7200000U,
    9000000U,
    0,
    0,
    0};

static const uint32_t easyIron_dry_operation_autoCondense2VsCourseTable[] = {
    4200000U,
    6000000U,
    0,
    0,
    0,
    0};

static const uint32_t tubDry_dry_operation_autoCondense2VsCourseTable[] = {
    0,
    0,
    0,
    0,
    0,
    0};

#endif
