// Includes --------------------------------------------------------------------
#include "rtc.h"
#include "uart.h"
#include "tools.h"

// Private Typedef -------------------------------------------------------------

// Private Macro ---------------------------------------------------------------

// Private Variables -----------------------------------------------------------

// Private Function prototypes -------------------------------------------------

// Functions -------------------------------------------------------------------

void RtcInit(void)
{
    CLK->PCKENR2 |= CLK_PCKENR2_RTC;
    CLK->CRTCR = CLK_CRTCR_RTCSEL & (0x04 << 1);

    // unlock the write protection
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    RTC->CR2 = 0;
    while (IS_CLEAR_BIT(RTC->ISR1, RTC_ISR1_WUTWF))
    {
        ;
    }

    RTC->ISR1 |= RTC_ISR1_INIT;

    while (IS_CLEAR_BIT(RTC->ISR1, RTC_ISR1_INITF))
    {
        ;
    }

    RTC->APRER = 0x00;
    RTC->SPRERH = 0x0F;
    RTC->SPRERL = 0x9F;

    RTC->ISR1 &= ~RTC_ISR1_INIT;

    RTC->CR1 = 0x04;
    RTC->WUTRH = 0x27;
    RTC->WUTRL = 0x0F;
    RTC->CR2 |= RTC_CR2_WUTE | RTC_CR2_WUTIE;
}

INTERRUPT_HANDLER(RTC_IRQHandler, 4)
{
    if (IS_SET_BIT(RTC->ISR2, RTC_ISR2_WUTF))
    {
        RTC->ISR2 &= ~RTC_ISR2_WUTF;
        UsartSendStr("w");
    }
}
