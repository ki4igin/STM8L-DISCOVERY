#ifndef __GPIO_H
#define __GPIO_H

// Includes --------------------------------------------------------------------
#include "stm8l15x.h"

// Typedef ---------------------------------------------------------------------

/**
  * @brief Definition of the GPIO pins. Used by the @ref GPIO_Init function in
  * order to select the pins to be initialized.
  */

typedef enum
{
    GPIO_Pin_0 = ((uint8_t)0x01),    /*!< Pin 0 selected */
    GPIO_Pin_1 = ((uint8_t)0x02),    /*!< Pin 1 selected */
    GPIO_Pin_2 = ((uint8_t)0x04),    /*!< Pin 2 selected */
    GPIO_Pin_3 = ((uint8_t)0x08),    /*!< Pin 3 selected */
    GPIO_Pin_4 = ((uint8_t)0x10),    /*!< Pin 4 selected */
    GPIO_Pin_5 = ((uint8_t)0x20),    /*!< Pin 5 selected */
    GPIO_Pin_6 = ((uint8_t)0x40),    /*!< Pin 6 selected */
    GPIO_Pin_7 = ((uint8_t)0x80),    /*!< Pin 7 selected */
    GPIO_Pin_LNib = ((uint8_t)0x0F), /*!< Low nibble pins selected */
    GPIO_Pin_HNib = ((uint8_t)0xF0), /*!< High nibble pins selected */
    GPIO_Pin_All = ((uint8_t)0xFF)   /*!< All pins selected */
} GPIO_Pin_TypeDef;

// Macro -----------------------------------------------------------------------

#define SWIM_Pin
#define SWIM_Port GPIOA

// Variables -------------------------------------------------------------------

// Function prototypes ---------------------------------------------------------
void GpioInit(void);
void GpioShutdown(void);

// Inline Functions ------------------------------------------------------------

#endif
