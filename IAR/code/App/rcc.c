// Includes --------------------------------------------------------------------
#include "rcc.h"
#include "tools.h"

// Private Typedef -------------------------------------------------------------
/**
   * @brief System Clock Source.
   */
typedef enum
{
    CLK_SYSCLKSource_HSI = (uint8_t)0x01, /*!< System Clock Source HSI */
    CLK_SYSCLKSource_LSI = (uint8_t)0x02, /*!< System Clock Source LSI */
    CLK_SYSCLKSource_HSE = (uint8_t)0x04, /*!< System Clock Source HSE */
    CLK_SYSCLKSource_LSE = (uint8_t)0x08  /*!< System Clock Source LSE */
} CLK_SYSCLKSource_TypeDef;

/**
  * @brief System Clock Divider
  */
typedef enum
{
    CLK_SYSCLKDiv_1 = (uint8_t)0x00,  /*!< System Clock Divider: 1 */
    CLK_SYSCLKDiv_2 = (uint8_t)0x01,  /*!< System Clock Divider: 2 */
    CLK_SYSCLKDiv_4 = (uint8_t)0x02,  /*!< System Clock Divider: 4 */
    CLK_SYSCLKDiv_8 = (uint8_t)0x03,  /*!< System Clock Divider: 8 */
    CLK_SYSCLKDiv_16 = (uint8_t)0x04, /*!< System Clock Divider: 16 */
    CLK_SYSCLKDiv_32 = (uint8_t)0x05, /*!< System Clock Divider: 32 */
    CLK_SYSCLKDiv_64 = (uint8_t)0x06, /*!< System Clock Divider: 64 */
    CLK_SYSCLKDiv_128 = (uint8_t)0x07 /*!< System Clock Divider: 128 */
} CLK_SYSCLKDiv_TypeDef;

// Private Macro ---------------------------------------------------------------

// Private Variables -----------------------------------------------------------

// Private Function prototypes -------------------------------------------------

// Functions -------------------------------------------------------------------

void RccInit(void)
{
    // Automatic mode
    CLK->ECKCR |= CLK_ECKCR_HSEON;

    while ((CLK->ECKCR & CLK_ECKCR_HSERDY) == 0)
    {
        ;
    }
    CLK->SWCR |= CLK_SWCR_SWEN;
    CLK->SWR = CLK_SYSCLKSource_HSE;
    CLK->CKDIVR = CLK_SYSCLKDiv_1;
}
