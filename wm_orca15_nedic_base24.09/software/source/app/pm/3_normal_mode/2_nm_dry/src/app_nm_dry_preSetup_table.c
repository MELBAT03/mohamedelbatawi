#include "def.h"
#include "app_user_interface.h"
#include "app_nm_dry.h"
#include "app_nm_dry_motor.h"

static const app_dry_setup_timeTable_t app_dry_preSetup_timeTable = {
    {0, 120000U, 300000U, 0U}};

const app_dry_setup_timeTable_t *app_nm_dry_preSetup_stateTotalTimeMSec_get(app_nm_userInterface_dryTimeSelection_t selection)
{
    const app_dry_setup_timeTable_t *ret = NULL;

    if ((selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN) && (selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO))
    {
        ret = &app_dry_preSetup_timeTable;
    }

    return ret;
}
