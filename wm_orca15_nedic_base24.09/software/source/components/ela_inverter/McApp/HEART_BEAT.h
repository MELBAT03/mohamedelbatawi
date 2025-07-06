#ifndef	_HEARTBEAT_H_
#define	_HEARTBEAT_H_

#include "../McUtil/McTarget.h"

void HEARTBEAT_Init(void);
//void HEARTBEAT_Update(void* arg);
void HEARTBEAT_Update(uint32_t period);

#endif 