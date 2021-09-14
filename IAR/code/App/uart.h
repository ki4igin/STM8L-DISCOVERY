#ifndef __UART_H
#define __UART_H

// Includes --------------------------------------------------------------------

#include "stm8l15x.h"

// Typedef ---------------------------------------------------------------------

// Macro -----------------------------------------------------------------------

// Variables -------------------------------------------------------------------

// Function prototypes ---------------------------------------------------------
void UartInit(void);
void UartShutdown(void);
void UsartSendStr(uint8_t *str);
void UsartSendArray(uint8_t *pbuf, uint8_t len);
void UsartSendByte(uint8_t data);
void UsartSendWord(uint16_t data);
void Usart1Rx_Callback(uint32_t data);

// Inline Functions
// ------------------------------------------------------------
static inline void UsartEnable(void)
{
    USART1->CR1 |= USART_CR1_USARTD;
}

static inline void UsartDisable(void)
{
    USART1->CR1 &= ~USART_CR1_USARTD;
    
}

#endif
