/**
 ****************************************************************************
 * @file     system_TMPM370.c
 * @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Source File for the
 *           TOSHIBA 'TMPM370' Device Series
 * @version  V2.0.2.4
 * @date:    2014/05/20
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *****************************************************************************
 */

#include "TMPM370.h"
#include "board.h"

/*-------- <<< Start of configuration section >>> ----------------------------*/

/* Watchdog Timer (WD) Configuration */
#define WD_SETUP  1
#define WDMOD_Val (0x00000000UL)
// #define WDMOD_Val ((uint32_t)0x00000050)
#define WDCR_Val (0x000000B1UL)

/* Clock Generator (CG) Configuration */
#define CLOCK_SETUP 1
#define SYSCR_Val   (0x00010000UL)
#define OSCCR_Val   (0x000E0104UL)
#define STBYCR_Val  (0x00000103UL)
#define PLLSEL_Val  (0x0000A13FUL)
#define CKSEL_Val   (0x00000000UL)

/*-------- <<< End of configuration section >>> ------------------------------*/

/*-------- DEFINES -----------------------------------------------------------*/
/* Define clocks */
#define XTALH (F_OSC) /* External high-speed oscillator freq */

/* Configure Warm-up time */
#define HZ_1M       1000000UL
#define WU_TIME_PLL 200UL
// #define WU_TIME_PLL 5000UL                                           /* warm-up time for PLL is 200us */
#define OSCCR_WUODR_PLL ((WU_TIME_PLL * XTALH / HZ_1M / 16U) << 20U) /* OSCCR<WUPODR11:0> = warm-up time(us) * EXTALH (MHz) / 16 */

/* Determine core clock frequency according to settings */
#if (CKSEL_Val & (1U << 1))                               /* If system clock is low-speed clock */
#define __CORE_CLK (0U)                                   /* error, no XT, define nothing */
#else                                                     /* If system clock is high-speed clock*/
#if ((PLLSEL_Val & (1U << 0)) && (OSCCR_Val & (1U << 2))) /* If PLL selected and enabled */
#if ((SYSCR_Val & 7U) == 0U)                              /* Gear -> fc                         */
#define __CORE_CLK (XTALH * 8U)
#elif ((SYSCR_Val & 7U) == 4U) /* Gear -> fc/2                       */
#define __CORE_CLK (XTALH * 8U / 2U)
#elif ((SYSCR_Val & 7U) == 5U) /* Gear -> fc/4                       */
#define __CORE_CLK (XTALH * 8U / 4U)
#elif ((SYSCR_Val & 7U) == 6U) /* Gear -> fc/8                       */
#define __CORE_CLK (XTALH * 8U / 8U)
#elif ((SYSCR_Val & 7U) == 7U) /* Gear -> fc/16                      */
#define __CORE_CLK (XTALH * 8U / 16U)
#else /* Gear -> reserved                   */
#define __CORE_CLK (0U)
#endif
#else
#if ((SYSCR_Val & 7U) == 0U) /* Gear -> fc                         */
#define __CORE_CLK (XTALH)
#elif ((SYSCR_Val & 7U) == 4U) /* Gear -> fc/2                       */
#define __CORE_CLK (XTALH / 2U)
#elif ((SYSCR_Val & 7U) == 5U) /* Gear -> fc/4                       */
#define __CORE_CLK (XTALH / 4U)
#elif ((SYSCR_Val & 7U) == 6U) /* Gear -> fc/8                       */
#define __CORE_CLK (XTALH / 8U)
#elif ((SYSCR_Val & 7U) == 7U) /* Gear -> fc/16                      */
#define __CORE_CLK (XTALH / 16U)
#else /* Gear -> reserved                   */
#define __CORE_CLK (0U)
#endif
#endif /* If PLL not used                    */
#endif

/* Clock Variable definitions */
uint32_t SystemCoreClock = __CORE_CLK; /*!< System Clock Frequency (Core Clock)*/

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Update SystemCoreClock according register values.
 */
void SystemCoreClockUpdate(void) /* Get Core Clock Frequency      */
{
    /* Determine clock frequency according to clock register values        */
    if (TSB_CG_CKSEL_SYSCK)
    { /* If system clock is low-speed clock  */
        /* no XT, do nothing */
    }
    else
    { /* If system clock is high-speed clock */
        if (TSB_CG_PLLSEL_PLLSEL && TSB_CG_OSCCR_PLLON)
        { /* If PLL enabled      */
            switch (TSB_CG->SYSCR & 7U)
            {
                case 0U: /* Gear -> fc          */
                    SystemCoreClock = XTALH * 8U;
                    break;
                case 1U:
                case 2U:
                case 3U: /* Gear -> reserved    */
                    SystemCoreClock = 0U;
                    break;
                case 4U: /* Gear -> fc/2        */
                    SystemCoreClock = XTALH * 8U / 2U;
                    break;
                case 5U: /* Gear -> fc/4        */
                    SystemCoreClock = XTALH * 8U / 4U;
                    break;
                case 6U: /* Gear -> fc/8        */
                    SystemCoreClock = XTALH * 8U / 8U;
                    break;
                case 7U: /* Gear -> fc/16       */
                    SystemCoreClock = XTALH * 8U / 16U;
                    break;
                default:
                    SystemCoreClock = 0U;
                    break;
            }
        }
        else
        { /* If PLL not used      */
            switch (TSB_CG->SYSCR & 7U)
            {
                case 0U: /* Gear -> fc          */
                    SystemCoreClock = XTALH;
                    break;
                case 1U:
                case 2U:
                case 3U:
                case 7U: /* Gear -> reserved    */
                    SystemCoreClock = 0U;
                    break;
                case 4U: /* Gear -> fc/2        */
                    SystemCoreClock = XTALH / 2U;
                    break;
                case 5U: /* Gear -> fc/4        */
                    SystemCoreClock = XTALH / 4U;
                    break;
                case 6U: /* Gear -> fc/8        */
                    SystemCoreClock = XTALH / 8U;
                    break;
                default:
                    SystemCoreClock = 0U;
                    break;
            }
        }
    }
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit(void)
{
#if (WD_SETUP) /* Watchdog Setup */
    TSB_WD->MOD = WDMOD_Val;
    if (!TSB_WD_MOD_WDTE)
    { /* If watchdog is to be disabled */
        TSB_WD->CR = WDCR_Val;
    }
#endif

#if (CLOCK_SETUP) /* Clock Setup */
    TSB_CG->SYSCR = SYSCR_Val;

    TSB_CG->OSCCR = OSCCR_Val | OSCCR_WUODR_PLL;

    TSB_CG->STBYCR = STBYCR_Val;

    if (TSB_CG_OSCCR_PLLON)
    { /* If PLL enabled */
        TSB_CG_OSCCR_WUEON = 1U;
        while (TSB_CG_OSCCR_WUEF)
        { /* Warm-up        */
            /* Do nothing */
        }
    }

    TSB_CG->PLLSEL = PLLSEL_Val;
    TSB_CG->CKSEL = CKSEL_Val;
#endif
}
