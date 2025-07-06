#include "tx03_common.h"
#include "mcal.h"

#define CG_WUODR_EXT ((uint16_t)0x0FFF)
#define CG_WUODR_PLL ((uint16_t)0x01F4)

static Result CG_EnableClkMulCircuit(void);

void mcal_cg_init(void)
{
    CG_SetFgearLevel(CG_DIVIDE_1);
    CG_SetPhiT0Src(CG_PHIT0_SRC_FGEAR);

    CG_SetPhiT0Level(CG_DIVIDE_1);

    // CG_SetPhiT0Level(CG_DIVIDE_64);
    CG_SetFosc(CG_FOSC_OSC1, ENABLE);

    // CG_SetSTBYMode(CG_STBY_MODE_STOP);
    // CG_SetPinStateInStopMode(ENABLE);
    /* Set up pll and wait for pll to warm up, set fc source to fpll */
    CG_EnableClkMulCircuit();
}

void mcal_cg_WDTSystem_set(CG_DivideLevel DivideFgearFromFc)
{
    /* Set fgear = fc -----> CG_DIVIDE_1, CG_DIVIDE_2, CG_DIVIDE_4, CG_DIVIDE_8, CG_DIVIDE_16*/
    CG_SetFgearLevel(DivideFgearFromFc);   // it was CG_DIVIDE_2 in the example
    /* Set fperiph to fgear not to fc directly  */
    CG_SetPhiT0Src(CG_PHIT0_SRC_FGEAR);   // CG_PHIT0_SRC_FC    //CG_PHIT0_SRC_FGEAR
    /* Set PhiT0 = fc */
    /*CG_DIVIDE_1, CG_DIVIDE_2, CG_DIVIDE_4, CG_DIVIDE_8,CG_DIVIDE_16,
CG_DIVIDE_32,CG_DIVIDE_64, CG_DIVIDE_128,CG_DIVIDE_256 or CG_DIVIDE_512 */
    CG_SetPhiT0Level(CG_DIVIDE_1);   // it was CG_DIVIDE_64  in the example
    /* Enable high-speed oscillator */
    CG_SetFosc(CG_FOSC_OSC1, ENABLE);
    /* Set low power consumption mode stop */
    CG_SetSTBYMode(CG_STBY_MODE_STOP);
    /* Set pin status in stop mode to "active" */
    CG_SetPinStateInStopMode(ENABLE);
    /* Set up pll and wait for pll to warm up, set fc source to fpll */
    CG_EnableClkMulCircuit();
}

/* CG_NormalToStop */
void mcal_cg_normalToStopMode_set(void)
{
    /* Set CG module: Normal ->Stop mode */
    CG_SetWarmUpTime(CG_WARM_UP_SRC_OSC1, CG_WUODR_EXT);
    /* Enter stop mode */
    __WFI();
}

/* CG_EnableClkMulCircuit */
static Result CG_EnableClkMulCircuit(void)
{
    Result retval = ERROR;
    WorkState st = BUSY;
    retval = CG_SetPLL(ENABLE);
    if (retval == SUCCESS)
    {
        /* Set warm up time */
        CG_SetWarmUpTime(CG_WARM_UP_SRC_OSC1, CG_WUODR_PLL);
        CG_StartWarmUp();

        do
        {
            st = CG_GetWarmUpState();
        } while (st != DONE);

        retval = CG_SetFcSrc(CG_FC_SRC_FPLL);
    }
    else
    {
        /*Do nothing */
    }

    return retval;
}
