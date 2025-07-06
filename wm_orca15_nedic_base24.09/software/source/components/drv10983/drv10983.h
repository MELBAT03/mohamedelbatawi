#ifndef _DRV10983_H
#define _DRV10983_H

#include "main.h"
#include "util.h"

void drv10983_init(uint8_t *motorParameters);
void drv10983_motorParameters_set(uint8_t *array);
void drv10983_motorParameters_get(uint8_t *readValues);
void drv10983_speed_set(void);
uint8_t drv10983_speedCmdFB_get(void);
void drv10983_motorReadings_get(uint8_t *readValues);

#endif
