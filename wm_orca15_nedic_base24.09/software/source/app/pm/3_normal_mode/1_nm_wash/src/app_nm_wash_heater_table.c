#include "def.h"

#include "app_nm_user_interface.h"

#include "app_nm_wash_heater.h"
#include "app_nm_wash_heater_table.h"

static const uint32_t * const steamTechONTempPtr[] = {
    &course_white_steamTechONTempTable,
    NULL,
    NULL,
    &course_eco_steamTechONTempTable,
    &course_cotton_steamTechONTempTable,
    NULL,
    NULL,
    NULL,
    NULL,
    &course_alergy_steamTechONTempTable,
    NULL,
    NULL};

static const uint32_t *const steamTechOFFTempPtr[] = {
    &course_white_steamTechOFFTempTable,
    NULL,
    NULL,
    &course_eco_steamTechOFFTempTable,
    &course_cotton_steamTechOFFTempTable,
    NULL,
    NULL,
    NULL,
    NULL,
    &course_alergy_steamTechOFFTempTable,
    NULL,
    NULL};

uint32_t app_nm_wash_heater_steamTechONTemp_get(void)
{
    return course_steamTechONTemp;
}

uint32_t app_nm_wash_heater_steamTechOFFTemp_get(void)
{
    return course_steamTechOFFTemp;
}

uint32_t app_nm_wash_heater_steamTechTargetONTemp_get(app_nm_userInterface_courseSelection_t course)
{
    return *(steamTechONTempPtr[course - 1]);
}

uint32_t app_nm_wash_heater_steamTechTargetOFFTemp_get(app_nm_userInterface_courseSelection_t course)
{
    return *(steamTechOFFTempPtr[course - 1]);
}
