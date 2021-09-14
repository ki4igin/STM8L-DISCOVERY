// Includes --------------------------------------------------------------------
#include "uart.h"
#include "gpio.h"
#include "tools.h"

// Private Typedef -------------------------------------------------------------

// Private Macro ---------------------------------------------------------------
#define F_CPU 8000000UL

// #define UART1_TX_Pin    BORT_TX_Pin
// #define UART1_RX_Pin    BORT_RX_Pin
// #define UART1_Port      BORT_GPIO_Port
#define UART_BAUD_RATE 115200
#define NBUF_RX 4

// Private Variables -----------------------------------------------------------
uint8_t bufRx[4] = {0};
uint8_t cntRx = 0;
// Private Function prototypes -------------------------------------------------

// Functions -------------------------------------------------------------------
void UartInit(void)
{
    CLK->PCKENR1 |= CLK_PCKENR1_USART1;

    // UART TX pin, PC3
    GPIOC->DDR |= GPIO_Pin_3;

    // UART RX pin, PC2
    GPIOC->DDR &= ~GPIO_Pin_2;

    uint8_t baudrate2 = (((F_CPU / UART_BAUD_RATE) & 0xF000) >> 12) +
                        ((F_CPU / UART_BAUD_RATE) & 0x000F);
    uint8_t baudrate1 = (((F_CPU / UART_BAUD_RATE) & 0x0FF0) >> 4);
    USART1->BRR2 = baudrate2;
    USART1->BRR1 = baudrate1;

    USART1->CR2 |= USART_CR2_RIEN;
    USART1->CR2 |= USART_CR2_TEN | USART_CR2_REN;
}

void UartShutdown(void)
{
}

void UsartSendStr(uint8_t *str)
{
    while (*str != 0)
    {
        while (IS_CLEAR_BIT(USART1->SR, USART_SR_TXE))
        {
            ;
        }

        USART1->DR = *str++;
    }
}

void UsartSendArray(uint8_t *pbuf, uint8_t len)
{
    do
    {
        while (IS_CLEAR_BIT(USART1->SR, USART_SR_TXE))
        {
            ;
        }

        USART1->DR = *pbuf++;
    } while (--len > 0);
}

void UsartSendByte(uint8_t data)
{
    while (IS_CLEAR_BIT(USART1->SR, USART_SR_TXE))
    {
        ;
    }
    USART1->DR = data;
}

void UsartSendWord(uint16_t data)
{

    while (IS_CLEAR_BIT(USART1->SR, USART_SR_TXE))
    {
        ;
    }
    USART1->DR = (uint8_t)(data >> 8);

    while (IS_CLEAR_BIT(USART1->SR, USART_SR_TXE))
    {
        ;
    }
    USART1->DR = (uint8_t)data;
}

INTERRUPT_HANDLER(USART1_RX_IRQHandler, 28)
{
    bufRx[cntRx] = USART1->DR;
    cntRx++;
    if (cntRx == NBUF_RX)
    {
        cntRx = 0;
        Usart1Rx_Callback(*(uint32_t *)bufRx);
    }
}
