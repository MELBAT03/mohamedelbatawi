/** @file ptcHeater.c
 *  @brief The implementation for the PTC Heating Element.
 *
 *       This PTC Heating Element consists of TWO heating plates parallel connected through three relays, one acts as general switch
 * and the other two ralays control the two plates of heating element.
 */

#include "def.h"
#include "component_config.h"

#include "ptc_heater.h"

#include "os.h"

typedef enum ptcHeater_heaterStates
{
    PTC_HEATER_INIT_STATE = 0,
    PTC_HEATER_SETUP_STATE = 1,
    PTC_HEATER_FETCH_DATA_STATE = 2,
    PTC_HEATER_ERROR_STATE = 3
} ptcHeater_heaterStates_t;

typedef struct ptcHeater_internalHandler
{
    doutputModule_handle ptcHandle;

    double (*ptr_functionFeedbackEq)(double);

    uint16_t minADCValue;
    uint16_t maxADCValue;

    double feedback;
} ptcHeater_internalHandler_t;

static ptcHeater_internalHandler_t gx_ptcHeater_internalHandler[PTC_HEATER_PLATE_NUMBER];

static ptcHeater_heaterStates_t heaterStates = PTC_HEATER_INIT_STATE;

static volatile uint8_t ptcHeater_errorFlag;
static uint32_t gu32_ptcHeater_feedbackCnt = 0;

static ainputModule_handle gx_ainput;

ptcHeater_handle ptcHeater_init(const ptcHeater_config_t *ptcHeaterConfig)
{
    ptcHeater_handle handle = NULL;
    static uint8_t handlerItemsCounter = 0;
    doutputModule_t plate;
    ainputModule_t cFB;

    if (ptcHeaterConfig != NULL && handlerItemsCounter < PTC_HEATER_PLATE_NUMBER)
    {
        plate.gpio.port = ptcHeaterConfig->plate.gpio.port;
        plate.gpio.pin = ptcHeaterConfig->plate.gpio.pin;
        plate.gpio.state = ptcHeaterConfig->plate.gpio.state;
        plate.gpio.ioState = GPIO_OUTPUT;

        gx_ptcHeater_internalHandler[handlerItemsCounter].ptcHandle = doutputModule_init(&plate);

        if (PTC_HEATER_FEEDBACK_AVAILABLE == ptcHeaterConfig->feedback)
        {
            // PTC Heater feedback Equation Set CallBack
            gx_ptcHeater_internalHandler[handlerItemsCounter].ptr_functionFeedbackEq = ptcHeaterConfig->feedbackEq;

            gx_ptcHeater_internalHandler[handlerItemsCounter].minADCValue = ptcHeaterConfig->minADCValue;
            gx_ptcHeater_internalHandler[handlerItemsCounter].maxADCValue = ptcHeaterConfig->maxADCValue;

            cFB.adcBlock = ptcHeaterConfig->FB.adcBlock;
            cFB.adcCh = ptcHeaterConfig->FB.adcCh;
            cFB.gpio.pin = ptcHeaterConfig->FB.gpio.pin;
            cFB.gpio.port = ptcHeaterConfig->FB.gpio.port;

            gx_ainput = ainputModule_init(&cFB);

            gu32_ptcHeater_feedbackCnt++;
        }

        handle = &gx_ptcHeater_internalHandler[handlerItemsCounter];
        handlerItemsCounter++;
    }

    heaterStates = PTC_HEATER_SETUP_STATE;

    return handle;
}

void ptcHeater_update(uint32_t period)
{
    static uint32_t rawADCData;
    static uint32_t smoothADCData;
    // static float LPF_Beta = 0.025;   // 0<ß<1
    static uint8_t errorCnt;

    switch (heaterStates)
    {
        case PTC_HEATER_INIT_STATE:
        {
            // do nothing
            // this is to confirm the init has been called
        }
        break;

        case PTC_HEATER_SETUP_STATE:
        {
            // for (uint8_t i = 0; i < gu32_ptcHeater_feedbackCnt; i++)
            // {
            //     smoothADCData = ainputModule_value_get(gx_ainput);
            // }
            heaterStates = PTC_HEATER_FETCH_DATA_STATE;
        }
        break;

        case PTC_HEATER_FETCH_DATA_STATE:
        {
            for (uint8_t i = 0; i < gu32_ptcHeater_feedbackCnt; i++)
            {
                rawADCData = ainputModule_value_get(gx_ainput);

                // sanity check
                if (rawADCData >= gx_ptcHeater_internalHandler[i].minADCValue && rawADCData <= gx_ptcHeater_internalHandler[i].maxADCValue)
                {
                    // smoothADCData = smoothADCData - (LPF_Beta * (float)(smoothADCData - rawADCData));
                    smoothADCData = rawADCData;
                    if (gx_ptcHeater_internalHandler[i].ptr_functionFeedbackEq != NULL)
                    {
                        gx_ptcHeater_internalHandler[i].feedback = gx_ptcHeater_internalHandler[i].ptr_functionFeedbackEq((double)smoothADCData);
                    }
                }
                else
                {
                    heaterStates = PTC_HEATER_ERROR_STATE;
                }
            }
        }
        break;

        case PTC_HEATER_ERROR_STATE:
        {
            errorCnt++;
            if (errorCnt <= 5)
            {
                heaterStates = PTC_HEATER_FETCH_DATA_STATE;
            }
            else
            {
                ptcHeater_errorFlag = 1;
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    LOG('p', 'v', "ptcState:%d  adcData:%d  filterData:%d  calculation:%d", heaterStates, rawADCData, smoothADCData, (uint32_t)gx_ptcHeater_internalHandler[0].feedback);
}

double ptcHeater_feedback_get(ptcHeater_handle ptcHeater)
{
    double ret = 0;

    if (ptcHeater != NULL)
    {
        ret = ptcHeater->feedback;
    }

    return ret;
}

void ptcHeater_plate_set(ptcHeater_handle ptcHeater, ptcHeater_plateState_t plateState)
{
    if (ptcHeater != NULL)
    {
        doutputModule_state_set(ptcHeater->ptcHandle, plateState);
    }
}
