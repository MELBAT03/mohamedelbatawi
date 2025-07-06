#include "touch_if.h"

void touchIF_create(void)
{
    R_CTSU_PinSetInit();
}

uint32_t touchIF_open(void)
{
#if QE_TOUCH_VERSION == QE_TOUCH_VERSION_1
    return R_TOUCH_Open(gp_ctsu_cfgs, gp_touch_cfgs, QE_NUM_METHODS, QE_TRIG_SOFTWARE);
#elif QE_TOUCH_VERSION == QE_TOUCH_VERSION_3
    return RM_TOUCH_Open(g_qe_touch_instance_config01.p_ctrl, g_qe_touch_instance_config01.p_cfg);
#elif QE_TOUCH_VERSION == QE_TOUCH_VERSION_3_3
    return RM_TOUCH_Open(g_qe_touch_instance_config01.p_ctrl, g_qe_touch_instance_config01.p_cfg);
#endif
}

uint32_t touchIF_scan(void)
{
#if QE_TOUCH_VERSION == QE_TOUCH_VERSION_1
    return R_TOUCH_UpdateDataAndStartScan();
#elif QE_TOUCH_VERSION == QE_TOUCH_VERSION_3
    return RM_TOUCH_ScanStart(g_qe_touch_instance_config01.p_ctrl);
#elif QE_TOUCH_VERSION == QE_TOUCH_VERSION_3_3
    return RM_TOUCH_ScanStart(g_qe_touch_instance_config01.p_ctrl);
#endif
}

uint32_t touchIF_data_get(uint64_t *pdata)
{
#if QE_TOUCH_VERSION == QE_TOUCH_VERSION_1
    return R_TOUCH_GetAllBtnStates(QE_METHOD_CONFIG01, &pdata);
#elif QE_TOUCH_VERSION == QE_TOUCH_VERSION_3
    return RM_TOUCH_DataGet(g_qe_touch_instance_config01.p_ctrl, pdata, NULL, NULL);
#elif QE_TOUCH_VERSION == QE_TOUCH_VERSION_3_3
    return RM_TOUCH_DataGet(g_qe_touch_instance_config01.p_ctrl, pdata, NULL, NULL);
#endif
}
