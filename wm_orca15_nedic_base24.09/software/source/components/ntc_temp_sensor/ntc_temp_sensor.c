/****************************************************************************
 * Title                 :   Negative Temperature Coefficient  (NTC)
 * Filename              :   ntc.c
 * Author                :   Iman Khairy
 * Origin Date           :   Sep 27, 2023
 * Version               :   1.0.0
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "component_config.h"
#include "ntc_temp_sensor.h" /* For this modules interfaces */
#include "os.h"

/******************************************************************************
 * Macros
 *******************************************************************************/
#define NUMBER_OF_AGREEABLE_ERRORS 10U

#define STARTING_TEMP_VALUE (30U)

/******************************************************************************
 * Typedefs
 *******************************************************************************/
typedef struct ntc_internalHandler
{
    /* to store the configuration data that is needed */
    const uint16_t *adcLookupTableRef;
    uint16_t lookupTableSize;
    uint16_t adcFaultUpperThreshold;
    uint16_t adcFaultLowerThreshold;
    uint16_t tempStep;
    uint16_t currentTemp;
    uint8_t error_counts;
    uint8_t errorFlag;
    uint16_t filter_adcValues[NTC_NUMBER_OF_FILTER_SAMPLES];
    uint8_t samplesCount;
    ainputModule_handle ainputHandle;
} ntc_internalHandler_t;

/******************************************************************************
 * Module Variable Declarations
 *******************************************************************************/
/**
 *  Declare internal handler to store the ntc cofiguration and other needed internal data.
 */
static ntc_internalHandler_t gax_ntc_internalHandler[NTC_SENSOR_NUM];

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/
/**
 *  Define counter to count the number of sensors.
 */
static uint8_t gu8ntcSensorCount = 0;

static uint8_t gu8_handlerItemsCounter = 0;

/******************************************************************************
 * Functions prototype
 *******************************************************************************/
static uint16_t adcSamples_filter(void);

/******************************************************************************
 * Functions definition
 *******************************************************************************/
/******************************************************************************
 * Function : ntc_init()
 */
/**
 * \b Description:
 *
 * This function is used to initialize the ntc based on the ntc configuration
 * structure which is passed to the function.
 *
 * PRE-CONDITION: The MCU clocks must be configured and enabled.
 * PRE-CONDITION: The pointer of the input parameter not equal NULL.
 * PRE-CONDITION: The pointer of the array that contains adc values not equal NULL.
 * PRE-CONDITION: the ended temperature is larger than the started value.
 * PRE-CONDITION: the number of sensors doesn't exceed th enuber that is defined.
 *
 * POST-CONDITION: The ntc component is setup with the configuration settings.
 *
 * @param  		ntc_ptrConfig is a pointer to the configuration structure that contains
 *				the initialization for the peripheral.
 *
 * @return 		void
 *******************************************************************************/

ntcSensor_handle ntc_init(ntc_config_t *ntc_ptrConfig)
{
    ntcSensor_handle handler = NULL;

    if ((ntc_ptrConfig != NULL) &&
        (ntc_ptrConfig->adcLookupTableRef != NULL) &&
        (ntc_ptrConfig->lookupTableSize > 0) &&
        (gu8_handlerItemsCounter < NTC_SENSOR_NUM))
    {
        /* init the ainput module to init adc module */
        gax_ntc_internalHandler[gu8_handlerItemsCounter].ainputHandle = ainputModule_init(&ntc_ptrConfig->ainput);

        /* Save data of configuration */
        gax_ntc_internalHandler[gu8_handlerItemsCounter].adcLookupTableRef = ntc_ptrConfig->adcLookupTableRef;
        gax_ntc_internalHandler[gu8_handlerItemsCounter].lookupTableSize = ntc_ptrConfig->lookupTableSize;
        gax_ntc_internalHandler[gu8_handlerItemsCounter].currentTemp = STARTING_TEMP_VALUE;
        gax_ntc_internalHandler[gu8_handlerItemsCounter].tempStep = ntc_ptrConfig->tempStep;
        gax_ntc_internalHandler[gu8_handlerItemsCounter].adcFaultUpperThreshold = ntc_ptrConfig->adcFaultUpperThreshold;
        gax_ntc_internalHandler[gu8_handlerItemsCounter].adcFaultLowerThreshold = ntc_ptrConfig->adcFaultLowerThreshold;

        gax_ntc_internalHandler[gu8_handlerItemsCounter].errorFlag = 0;
        gax_ntc_internalHandler[gu8_handlerItemsCounter].error_counts = 0;
        gax_ntc_internalHandler[gu8_handlerItemsCounter].samplesCount = 0;

        for (uint8_t i = 0; i < NTC_NUMBER_OF_FILTER_SAMPLES; i++)
        {
            gax_ntc_internalHandler[gu8_handlerItemsCounter].filter_adcValues[i] = 0;
        }

        handler = &gax_ntc_internalHandler[gu8_handlerItemsCounter];
        gu8_handlerItemsCounter++;
    }

    return handler;
}

/******************************************************************************
 * Function : ntc_update()
 */
/**
 * \b Description:
 *
 * This function is used to update the current tempreture of one of the sensors periodically
 * at one time.
 *
 * PRE-CONDITION: Initialization of the module.
 *
 * POST-CONDITION: The current temperature of one sensor at time is set.
 *
 * @param  		period is period of time the task needs to iterate.
 *
 * @return 		void
 *******************************************************************************/
void ntc_update(uint32_t period)
{
    ntc_internalHandler_t *ptrCurrentHandler = NULL;
    static uint8_t waitSamplesCount = 0;
    uint16_t adcValue;

    if (0 != gu8_handlerItemsCounter)
    {
        ptrCurrentHandler = &gax_ntc_internalHandler[gu8ntcSensorCount];

        /* Get the adc value from ainput module */
        ptrCurrentHandler->filter_adcValues[ptrCurrentHandler->samplesCount] = (uint16_t)ainputModule_value_get(gax_ntc_internalHandler[gu8ntcSensorCount].ainputHandle);

        /** Sanity check */
        if ((ptrCurrentHandler->filter_adcValues[ptrCurrentHandler->samplesCount] >= ptrCurrentHandler->adcLookupTableRef[0]) && (ptrCurrentHandler->filter_adcValues[ptrCurrentHandler->samplesCount] <= ptrCurrentHandler->adcLookupTableRef[ptrCurrentHandler->lookupTableSize - 1]) && (ptrCurrentHandler->error_counts < NUMBER_OF_AGREEABLE_ERRORS))
        {
            /* Increment the counter and check if U store the number of samples needed to start filtering or not */
            ptrCurrentHandler->samplesCount++;
            if (ptrCurrentHandler->samplesCount >= NTC_NUMBER_OF_FILTER_SAMPLES)
            {
                ptrCurrentHandler->samplesCount = 0;
            }

            if (waitSamplesCount < NTC_NUMBER_OF_FILTER_SAMPLES)
            {
                waitSamplesCount++;
                /* Still need samples */
            }
            else
            {
                /* Filtering */
                adcValue = adcSamples_filter();

                for (uint16_t i = 0; i < ptrCurrentHandler->lookupTableSize; i++)
                {
                    if (adcValue >= ptrCurrentHandler->adcLookupTableRef[i] && adcValue <= ptrCurrentHandler->adcLookupTableRef[i + 1])
                    {
                        ptrCurrentHandler->currentTemp = i * ptrCurrentHandler->tempStep;
                    }
                }

                gu8ntcSensorCount++;
                if (gu8ntcSensorCount >= gu8_handlerItemsCounter)
                {
                    gu8ntcSensorCount = 0;
                }

                ptrCurrentHandler->error_counts = 0;
            }
        }
        else if (ptrCurrentHandler->error_counts > NUMBER_OF_AGREEABLE_ERRORS)
        {
            ptrCurrentHandler->error_counts = 0;
            ptrCurrentHandler->errorFlag = 1;
        }
        else
        {
            if ((ptrCurrentHandler->filter_adcValues[ptrCurrentHandler->samplesCount] < ptrCurrentHandler->adcFaultLowerThreshold) || (ptrCurrentHandler->filter_adcValues[ptrCurrentHandler->samplesCount] > ptrCurrentHandler->adcFaultUpperThreshold))
            {
                ptrCurrentHandler->error_counts++;
            }
        }
    }
}

/******************************************************************************
 * Function : ntc_temp_get()
 */
/**
 * \b Description:
 *
 * This function is used to get the temperature of needed sensor.
 *
 * PRE-CONDITION: Initialization of the module.
 * PRE-CONDITION: the temperature was set.
 *
 * POST-CONDITION: The temperature is getten.
 *
 * @param  		void
 *
 * @return 		float to get the temperature.
 *******************************************************************************/
uint16_t ntc_temp_get(ntcSensor_handle ntc)
{
    uint16_t ret = 0;

    if (ntc != NULL)
    {
        ret = ntc->currentTemp;
    }

    return ret;
}

/******************************************************************************
 * Function : ntc_error_get()
 */
/**
 * \b Description:
 *
 * This function is used to get the error state.
 *
 * PRE-CONDITION: Initialization of the module.
 *
 * POST-CONDITION: The error is getten.
 *
 * @param  		void
 *
 * @return 		uint8_t to get the error state.
 *******************************************************************************/
uint8_t ntc_error_get(ntcSensor_handle ntc)
{
    uint8_t ret = 0;

    if (ntc != NULL)
    {
        ret = ntc->errorFlag;
    }

    return ret;
}

/******************************************************************************
 * Function : ntc_temp_set()
 */
/**
 * \b Description:
 *
 * This function is used for testing to set the the temperature of needed sensor.
 *
 * PRE-CONDITION: Initialization of the module.
 *
 * POST-CONDITION: The temperature is set.
 *
 * @param  		tempValue is the temperature.
 * @param  		ntcSensorID is the ID of the sensor that will set.
 *
 * @return 		void
 *******************************************************************************/
void ntc_temp_set(ntcSensor_handle ntc, uint16_t value)
{
    if (ntc != NULL)
    {
        ntc->currentTemp = value;
    }
}

/******************************************************************************
 * Function : adcSamples_filter()
 */
/**
 * \b Description:
 *
 * This static function is used to get the average of adc readings' samples.
 *
 * PRE-CONDITION: the number of samples readings are stored before filtering.
 *
 * POST-CONDITION: The filtered value is ready.
 *
 * @param  		void
 *
 * @return 		uint16_t to get the filtered value.
 *******************************************************************************/
static uint16_t adcSamples_filter(void)
{
    uint32_t adcValue = 0;
    for (uint8_t count = 0; count < NTC_NUMBER_OF_FILTER_SAMPLES; count++)
    {
        adcValue += gax_ntc_internalHandler[gu8ntcSensorCount].filter_adcValues[count];
    }
    return (adcValue / NTC_NUMBER_OF_FILTER_SAMPLES);
}
