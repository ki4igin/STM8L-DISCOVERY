// Includes --------------------------------------------------------------------
#include "i2c.h"
#include "stm8l15x.h"
#include "gpio.h"
#include "tools.h"

// Private Typedef -------------------------------------------------------------

// Private Macro ---------------------------------------------------------------
#define F_MASTER 8000000 // f_master in Hz
#define LOW_BYTE(a) ((uint8_t)(a))
#define HIGH_BYTE(a) ((uint8_t)((a) >> 8))

#define I2C_FREQ 100000

// Private Variables -----------------------------------------------------------
uint8_t slaveAddr = 0;
uint8_t regAddr = 0;

// Private Function prototypes -------------------------------------------------

// Functions -------------------------------------------------------------------
void I2C_Init(void)
{
    // I2C PC1 PC0
    GPIOC->DDR |= GPIO_Pin_1 | GPIO_Pin_0;
    GPIOC->ODR |= GPIO_Pin_1 | GPIO_Pin_0;
    //GPIOC->CR1 &= ~(GPIO_Pin_1 | GPIO_Pin_0);
    //GPIOC->CR2 |= GPIO_Pin_1 | GPIO_Pin_0;

    CLK->PCKENR1 |= CLK_PCKENR1_I2C1;
    // Clock settings
    I2C1->CR2 |= I2C_CR2_SWRST;
    I2C1->CR2 = 0;
    I2C1->FREQR = F_MASTER / 1000000;
    I2C1->CCRH = HIGH_BYTE(F_MASTER / I2C_FREQ / 2);
    I2C1->CCRL = LOW_BYTE(F_MASTER / I2C_FREQ / 2);
    I2C1->TRISER = 3;

    //I2C1->ITR = I2C_ITR_ITEVTEN;

    // Enable i2c
    I2C1->CR1 = I2C_CR1_PE;
}

uint16_t I2C_Read(uint8_t slaveAddr, uint8_t regAddr, uint8_t *pbuf, uint8_t bufsize)
{
    volatile uint16_t cnt = 0;
    I2C1->CR2 = I2C_CR2_START;
    while (IS_CLEAR_BIT(I2C1->SR1, I2C_SR1_SB))
    {
        cnt++;
    }
    I2C1->DR = (slaveAddr << 1);
    while (IS_CLEAR_BIT(I2C1->SR1, I2C_SR1_ADDR))
    {
        cnt++;
    }
    uint8_t temp1 = I2C1->SR3;

    I2C1->DR = regAddr;
    while (IS_CLEAR_BIT(I2C1->SR1, I2C_SR1_BTF))
    {
        cnt++;
    }

    I2C1->CR2 = I2C_CR2_START;
    while (IS_CLEAR_BIT(I2C1->SR1, I2C_SR1_SB))
    {
        cnt++;
    }
    I2C1->DR = (slaveAddr << 1) + 1;
    while (IS_CLEAR_BIT(I2C1->SR1, I2C_SR1_ADDR))
    {
        cnt++;
    }
    uint8_t temp2 = I2C1->SR3;
    I2C1->CR2 |= I2C_CR2_ACK;
    for (uint8_t i = 0; i < bufsize; i++)
    {
        while (IS_CLEAR_BIT(I2C1->SR1, I2C_SR1_RXNE))
        {
            cnt++;
        }
        *pbuf++ = I2C1->DR;
        if (i == (bufsize - 2))
        {
            I2C1->CR2 &= ~I2C_CR2_ACK;
            I2C1->CR2 = I2C_CR2_STOP;
        }
    }
    return cnt;
}

INTERRUPT_HANDLER(I2C1_IRQHandler, 29)
{
    if (IS_SET_BIT(I2C1->SR1, I2C_SR1_SB))
    {
        I2C1->DR = slaveAddr;
    }
    else if (IS_SET_BIT(I2C1->SR1, I2C_SR1_ADDR))
    {
        I2C1->DR = regAddr;
    }
}
