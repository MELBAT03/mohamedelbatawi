/******************************************************************************* * 
 * Header: inverter 
 *  Author: mohamed ibrahim elbatawi
 * Description: configer the movement of the WM inverter 
 *  mohaemd elbatawi  4/5/2025
 * *******************************************************************************/

#ifndef	_INVERTER_H_
#define	_INVERTER_H_

#include "def.h"
typedef enum{
	empty=335,  //335
	_1K=418,   // 418
	_2K=462,  //462
	_3K=488,  //488
	_4K=591,	//591
	_5K=807, //807
	_6K=1532,	//1532
	_7K=1777,	//1777
	_8K=2175, //2175
	_9K=2378,	//2378
	_10K=2408,	//2408
	_11K=2482,	//2482
    _12K=3162,	//3162
   	_13K=3370,	//3370
    _14K=3372,
    _15K=3400,
    _16K=3499,

}APP_load_detection_range;
typedef enum {
	empty_in_kg,
	_1K_KG=10,
	_2K_KG=20,
	_3K_KG=30,
	_4K_KG=40,
	_5K_KG=50,
	_6K_KG=60,
	_7K_KG=70,
	_8K_KG=80,
	_9K_KG=90,
	_10K_KG=100,
	_11K_KG=110,
	max_weight_in_kg=120
}weight_value_in_kg;

typedef enum{
	start_load_detection_CW,
	measuring_the_power_CW,
        start_load_detection_CCW,
	measuring_the_power_CCW,
	load_display
}APP_load_detection_states;

void inverter_Init(void);
void inverter_fail_Update(uint32_t period);
void WM_Update(uint32_t period);

unsigned char load_detection_level_get(void);
void motor_stop(void);

#endif