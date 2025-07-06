#ifndef	_APP_MEASURMENT_H_
#define	_APP_MEASURMENT_H_

#include "../McUtil/McTarget.h"
#include "AppWashingMachine.h"

void APP_measurment_init(void);
//void APP_measurment_Update(void* arg);
void APP_measurment_Update(uint32_t period);


unsigned int avg_speed_rpm_get(void);
unsigned int avg_DC_bus_get(void);


#endif 