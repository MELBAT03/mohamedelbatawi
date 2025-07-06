/** @file ptcHeater.h
 *  @brief The interface definition for the PTC Heating Element.
 *
 *       This PTC Heating Element consists of TWO heating plates parallel connected through three relays, one acts as general switch
 * and the other two ralays control the two plates of heating element.
 *
 *  This is the header file for the definition of the interface for PTC Heating Element Module.
 */

#ifndef PTC_HEATER_H
#define PTC_HEATER_H

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "ainput_module/ainput_module.h"   /* For ADC Read */
#include "doutput_module/doutput_module.h" /* For DIO Control */
#include "component_config.h"              /* For PTC Heater component configuration */
                                           /* For PTC Heater component ID */

/******************************************************************************
 * Typedefs
 *******************************************************************************/
/**
 * Defines the PTC Heater configuration elements that are used
 * by ptcHeater_init to configure :
 * 1- Plate number
 * 2- Feedback
 * 3- Feedback Equation Set CallBack
 * 4- Sanity check by checking the min and max ADC Value
 */

typedef enum ptcHeater_feedbackAvailability
{
    PTC_HEATER_FEEDBACK_NOT_AVAILABLE = 0,
    PTC_HEATER_FEEDBACK_AVAILABLE = 1
} ptcHeater_feedbackAvailability_t;

typedef struct
{
    doutputModule_t plate;
    ainputModule_t FB;

    ptcHeater_feedbackAvailability_t feedback;
    double (*feedbackEq)(double);
    uint16_t minADCValue;
    uint16_t maxADCValue;
} ptcHeater_config_t;

/**
 * Defines the possible states for plates of heating element.
 */
typedef enum
{
    PTC_HEATER_OFF = 0,
    PTC_HEATER_ON = 1
} ptcHeater_plateState_t;

typedef struct ptcHeater_internalHandler *ptcHeater_handle;

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/
ptcHeater_handle ptcHeater_init(const ptcHeater_config_t *ptcHeaterConfig);

void ptcHeater_update(uint32_t period);

double ptcHeater_feedback_get(ptcHeater_handle ptcHeater);

void ptcHeater_plate_set(ptcHeater_handle ptcHeater, ptcHeater_plateState_t plateState);

#endif
