#ifndef APP_NM_WASH_RINSE_TABLE_H
#define APP_NM_WASH_RINSE_TABLE_H

#include "def.h"

#include "app_nm_wash_rinse.h"
#include "app_nm_wash.h"
#include "app_user_interface.h"

static const app_nm_wash_rinse_stateTime_t course_cotton_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 180000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 240000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_white_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 180000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 240000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_dark_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 120000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 240000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_light_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 120000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 180000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_babycare_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 120000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 240000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_eco_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 120000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 180000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_wool_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 120000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 180000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_jeans_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 180000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 240000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_mix_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 180000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 180000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_sensitive_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 120000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 180000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_sports_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 60000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 120000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_alergy_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 120000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 240000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_rapidWash_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 120000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 240000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_stateTime_t course_bedding_rinseTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .spinTimeMSec = 430000U, .spinBrakeTimeMSec = APP_MOTOR_SPIN_BRAKE_TIME_MSEC, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 180000U, .untangleTimeMSec = 60000U};

static const app_nm_wash_rinse_waterLevel_t rinse_waterLevelTable[] = {
    {.fillWaterLevelLiter = 27U},
    {.fillWaterLevelLiter = 38U},
    {.fillWaterLevelLiter = 55U},
    {.fillWaterLevelLiter = 72U},
    {.fillWaterLevelLiter = 86U},
    {.fillWaterLevelLiter = 86U},
    {.fillWaterLevelLiter = 86U},
    {.fillWaterLevelLiter = 86U},
    {.fillWaterLevelLiter = 86U},
    {.fillWaterLevelLiter = 98U}};

#endif
