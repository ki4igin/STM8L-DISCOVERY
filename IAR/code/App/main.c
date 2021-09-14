// Includes --------------------------------------------------------------------
#include "main.h"
#include "rcc.h"
#include "uart.h"
#include "i2c.h"
#include "debug.h"
#include "rtc.h"
#include "tools.h"

// Private Typedef -------------------------------------------------------------


// Private Macro ---------------------------------------------------------------
#define EEPROM_DATA_ADR (0x1000 + 0)

// Private Variables -----------------------------------------------------------
__eeprom uint8_t data = 0;
uint8_t cnt;
// Private Function prototypes -------------------------------------------------

// Functions -------------------------------------------------------------------
static inline void Tim2Init(void);

int main(void)
{
    RccInit();

    // Unlock write to EEPROM
    // FLASH->DUKR = 0xAE;
    // FLASH->DUKR = 0x56;

    //*(uint8_t *)0x8000 = 0x00;

    cnt = data;

    UartInit();

    DebugSendMessage("Hello");

    Tim2Init();
    RtcInit();
    I2C_Init();

    rim();

    while (1)
    {
        ;
    }
}

static inline void Tim2Init(void)
{
    CLK->PCKENR1 |= CLK_PCKENR1_TIM2;

    TIM2->PSCR |= 0xF & TIM_PSCR_PSC;
    TIM2->ARRH = 0xFF;
    TIM2->ARRL = 0xFF;
    TIM2->IER |= TIM_IER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break Interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_IRQHandler, 19)
{
    /* In order to detect unexpected events during development,
   it is recommended to set a breakpoint on the following instruction.
*/
    if (IS_SET_BIT(TIM2->SR1, TIM_SR1_UIF))
    {
        TIM2->SR1 &= ~TIM_SR1_UIF;
        uint8_t buf[4] = {0};
        uint16_t cnt = I2C_Read(0x6A, 0x0F, (uint8_t *)&buf, sizeof(buf));
        UsartSendWord(cnt);
    }
}

void Usart1Rx_Callback(uint32_t data)
{
    switch (data)
    {
    case 0x636C6572: // cler
        break;
    case 0x65726173: // eras
        // Перепрограммирование
        FLASH->PUKR = 0x56;
        FLASH->PUKR = 0xAE;
        *(uint8_t *)0x8000 = 0x00;
        FLASH->PUKR = 0x00;
        FLASH->PUKR = 0x00;

        // Software reset
        WWDG->CR |= WWDG_CR_WDGA;
        WWDG->CR &= ~WWDG_CR_T6;
        break;

    default:
        UsartSendArray((uint8_t *)&data, sizeof(data));
        break;
    }
}
