#ifndef HANDLER_MOTOR_H
#define HANDLER_MOTOR_H 1

#include "def.h"
#include "components/ddm_motor/ddm_motor.h"

typedef DDM_command_t handler_motor_command_t;
#define HANDLER_MOTOR_COMMAND_PING              DDM_COMMAND_PING
#define HANDLER_MOTOR_COMMAND_REQUEST           DDM_COMMAND_REQUEST
#define HANDLER_MOTOR_COMMAND_CW                DDM_COMMAND_CW
#define HANDLER_MOTOR_COMMAND_CCW               DDM_COMMAND_CCW
#define HANDLER_MOTOR_COMMAND_AGITATION         DDM_COMMAND_AGITATION
#define HANDLER_MOTOR_COMMAND_SPIN              DDM_COMMAND_CW
#define HANDLER_MOTOR_COMMAND_SPIN_PULSATOR     DDM_COMMAND_CW
#define HANDLER_MOTOR_COMMAND_STOP              DDM_COMMAND_STOP
#define HANDLER_MOTOR_COMMAND_BRAKE             DDM_COMMAND_BRAKE
#define HANDLER_MOTOR_COMMAND_VOLTAGE_DETECTION DDM_COMMAND_VOLTAGE_DETECTION
#define HANDLER_MOTOR_COMMAND_WEIGHT_DETECTION  DDM_COMMAND_WEIGHT_DETECTION
#define HANDLER_MOTOR_COMMAND_FABRIC_DETECTION  DDM_COMMAND_FABRIC_DETECTION
#define HANDLER_MOTOR_COMMAND_CLUTCH            DDM_COMMAND_CLUTCH
#define HANDLER_MOTOR_COMMAND_DECLUTCH          DDM_COMMAND_DECLUTCH
#define HANDLER_MOTOR_COMMAND_FORCED_BRAKE      DDM_COMMAND_FORCED_BRAKE

typedef DDM_motionPattern_t handler_motor_motionPattern_t;

typedef DDM_error_t handler_motor_error_t;
#define HANDLER_MOTOR_NO_ERROR                        DDM_NO_ERROR
#define HANDLER_MOTOR_ERROR_GENERAL_FAULT             DDM_ERROR_GENERAL_FAULT
#define HANDLER_MOTOR_ERROR_IPM_OV_TEMP               DDM_ERROR_IPM_OV_TEMP
#define HANDLER_MOTOR_ERROR_BUS_VOLTAGE               DDM_ERROR_BUS_VOLTAGE
#define HANDLER_MOTOR_ERROR_MOTOR_STALL_TACHO_MISSING DDM_ERROR_MOTOR_STALL_TACHO_MISSING
#define HANDLER_MOTOR_ERROR_HW_OV_CURRENT             DDM_ERROR_HW_OV_CURRENT
#define HANDLER_MOTOR_ERROR_BUS_UNDER_VOLTAGE         DDM_ERROR_BUS_UNDER_VOLTAGE
#define HANDLER_MOTOR_ERROR_LOST_PHASE                DDM_ERROR_LOST_PHASE
#define HANDLER_MOTOR_ERROR_MOTOR_OV_TEMP             DDM_ERROR_MOTOR_OV_TEMP
#define HANDLER_MOTOR_ERROR_NO_COM                    DDM_ERROR_NO_COM
#define HANDLER_MOTOR_ERROR_NACK                      DDM_ERROR_NACK
#define HANDLER_MOTOR_ERROR_MODE                      DDM_ERROR_MODE
#define HANDLER_MOTOR_ERROR_FAULT                     DDM_ERROR_FAULT
#define HANDLER_MOTOR_ERROR_CRC                       DDM_ERROR_CRC

typedef DDM_state_t handler_motor_state_t;
#define HANDLER_MOTOR_SYNC_STATE               DDM_SYNC_STATE
#define HANDLER_MOTOR_BRAKE_STATE              DDM_BRAKE_STATE
#define HANDLER_MOTOR_FORCE_BRAKE_STARTUP      DDM_FORCE_BRAKE_STARTUP
#define HANDLER_MOTOR_FORCE_BRAKE_CANCEL       DDM_FORCE_BRAKE_CANCEL
#define HANDLER_MOTOR_FORCE_BRAKE_STARTUP_DONE DDM_FORCE_BRAKE_STARTUP_DONE
#define HANDLER_MOTOR_OPERATION_STATE          DDM_OPERATION_STATE
#define HANDLER_MOTOR_FAULT_STATE              DDM_FAULT_STATE

void handler_motor_init(void);
uint32_t handler_motor_weightFeedback_get(void);
uint32_t handler_motor_weightRawFeedback_get(void);
void handler_motor_weightFeedback_set(uint32_t value);
uint32_t handler_motor_testModePulseCount_get(void);
void handler_motor_command_set(handler_motor_command_t command, const handler_motor_motionPattern_t *pattern);
handler_motor_state_t handler_motor_state_get(void);
void handler_motor_state_set(handler_motor_state_t value);
uint16_t handler_motor_version_get(void);
uint8_t handler_motor_clutchDoneFlag_get(void);
void handler_motor_clutchDoneFlag_set(uint8_t value);
handler_motor_error_t handler_motor_error_get(void);
void handler_motor_error_set(handler_motor_error_t error);
uint32_t handler_motor_speed_get(void);
void handler_motor_speed_set(uint32_t value);

#endif
