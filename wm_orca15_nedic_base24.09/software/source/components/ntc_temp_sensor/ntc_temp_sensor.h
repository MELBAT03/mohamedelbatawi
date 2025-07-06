/****************************************************************************
 * Title                 :   Negative Temperature Coefficient  (NTC)
 * Filename              :   ntc.h
 * Author                :   Iman Khairy
 * Origin Date           :   Sep 27, 2023
 * Version               :   1.0.0
 *
 *****************************************************************************/
/*************** INTERFACE NOTES **************************************
 * !important to read before use this module.
 * NOTES: 1- accordong to the lower layer, the interface uses the ainput module
 *           to use adc for analog input catch.
 *        2- you have to call the update function of the ainput module to get
 *           the updated adc values.
 *        3- accordong to the upper layer, this interface is used by app_temp_sensor
 *           in handler input layer and the app layer.
 *        4- there are many ways to calculate the temperature from the voltage drop
 *           and the chosen method is lookup table method to save time.
 *        5- the lookup table is an array (adcRecordedValues) contains adc recorded values
 *           from equations and its index multiply tempStep expresses the temperature
 *        6- configuration members:
 *           tempStartIndex --> for the first temperature the lookup table started from
 *           tempEndIndex --> for the last temperature the lookup table ended to
 *           tempStep --> the step of temperature between the readings at the adcRecordedValues.
 *           ntcSensorID --> is the ID that U shoud put at component_id file for your sensor.
 *           tempTypicalValue --> is the Typical temperature Value in your application.
 *
 *****************************************************************************/
/** @file ntc.h
 *  @brief The interface definition for the NTC sensor.
 *
 *  This is the header file for the definition of the interface for a negative
 *  temperature coefficient sensor for multiple sensors.
 */
#ifndef NTC_TEMP_SENSOR_H
#define NTC_TEMP_SENSOR_H

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "ainput_module/ainput_module.h" /* For ADC usage */

#include "component_config.h"

/******************************************************************************
 * Typedefs
 *******************************************************************************/
typedef struct
{
    ainputModule_t ainput;
    const uint16_t *adcLookupTableRef;
    uint16_t lookupTableSize;
    uint16_t adcFaultUpperThreshold;
    uint16_t adcFaultLowerThreshold;
    uint16_t tempStep;
} ntc_config_t;

typedef struct ntc_internalHandler *ntcSensor_handle;

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/
ntcSensor_handle ntc_init(ntc_config_t *ntc_ptrConfig);
void ntc_update(uint32_t period);
uint16_t ntc_temp_get(ntcSensor_handle ntc);
uint8_t ntc_error_get(ntcSensor_handle ntc);

/* API that is used for testing */
void ntc_temp_set(ntcSensor_handle ntc, uint16_t value);

#endif
