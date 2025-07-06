#ifndef TOUCH_IF_H

#define TOUCH_IF_H

#include "def.h"
#include "component_config.h"
#if QE_TOUCH_VERSION == QE_TOUCH_VERSION_1
#include "mcal.h"
#include "r_touch_qe_config.h"
#include "r_touch_qe_config_reference.h"
#include "r_touch_qe_if.h"
#include "r_touch_qe_private.h"

typedef qe_err_t touch_error_state_t;
#define TOUCH_SUCCESS QE_SUCCESS
#define TOUCH_BUSY    QE_ERR_BUSY

#elif QE_TOUCH_VERSION == QE_TOUCH_VERSION_3
#include "components/touch/touch_v3/qe_touch_config.h"
#include "mcal.h"
#include "inc/board.h"
#include <smc_gen/r_bsp/mcu/all/fsp_common_api.h>
#elif QE_TOUCH_VERSION == QE_TOUCH_VERSION_3_3
#include "handlers/input/touch_v3_3/qe_touch_config.h"
#include <r_ctsu_qe_pinset.h>
#include "mcal.h"
#include "../config/board.h"
#include <fsp_common_api.h>

typedef fsp_err_t touch_error_state_t;
#define TOUCH_SUCCESS FSP_SUCCESS
#define TOUCH_BUSY    FSP_ERR_IN_USE
#endif

void touchIF_create(void);
uint32_t touchIF_open(void);
uint32_t touchIF_scan(void);
uint32_t touchIF_data_get(uint64_t *pdata);

#endif
