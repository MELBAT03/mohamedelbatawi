#ifndef APP_EXIT_SHUTDOWN_H
#define APP_EXIT_SHUTDOWN_H

#include "def.h"

void app_exit_shutdown(void);
void app_exit_shutdown_request(void (*condition)(void));

#endif
