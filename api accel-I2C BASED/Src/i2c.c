/*
 * i2c.c
 *
 *  Created on: Sep 25, 2024
 *      Author: spam
 */


#include "i2c.h"


void I2C_Init(void)
{
	//ENABLE GPIOS FOR THE I2C
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIO_I2C->MODER |= BV((SCL_PIN*2+1)) | BV((SDA_PIN*2) + 1);
	GPIO_I2C->MODER &= ~(BV(SCL_PIN*2) | BV(SDA_PIN*2));
    GPIO_I2C->PUPDR &= ~(BV(SCL_PIN*2+1) | BV(SDA_PIN*2+1) | BV(SCL_PIN*2) | BV(SDA_PIN*2)); // no pull-up/down

	GPIO_I2C->AFR[0] |= BV(26) | BV(30);
	GPIO_I2C->AFR[0] &= ~(BV(31) | BV(29) | BV(28) | BV(27) | BV(25) | BV(24));

	//ENABLE I2C
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	I2C1->CR1 = I2C_CR1_SWRST;
	I2C1->CR1 = 0;
	I2C1->CR2 |= 16;
	I2C1->CCR &= ~I2C_CCR_FS;
	I2C1->CCR |= 80;
	I2C1->TRISE |= 17;
	I2C1->CR1 |= BV(I2C_CR1_ACK_Pos);
	I2C1->CR1 |= BV(I2C_CR1_PE_Pos);
}


void I2C_Start(void)
{
	I2C1->CR1 |= BV(I2C_CR1_START_Pos);
	while(!(I2C1->SR1 & BV(I2C_SR1_SB_Pos)))
		;
}

void I2C_Stop(void)
{
	I2C1->CR1 |= BV(I2C_CR1_STOP_Pos);
	 while(!(I2C1->SR2 & I2C_SR2_BUSY))
			;
}

void I2C_repeatstart(void)
{
	I2C_Start();
}

void I2C_SendSlaveAddr(uint8_t addr)
{
	I2C1->DR = addr;
	while(!(I2C1->SR1 & BV(I2C_SR1_ADDR_Pos)));
    (void)I2C1->SR1;
    (void)I2C1->SR2;
}

void 	I2C_SendData(uint8_t data)
{
	while(!(I2C1->SR1 & BV(I2C_SR1_TXE_Pos)));
	I2C1->DR = data;
	while(!(I2C1->SR1 & BV(I2C_SR1_BTF_Pos)));
}

uint16_t I2C_recvdata(void)
{
	I2C1->CR1 |= I2C_CR1_ACK | I2C_CR1_POS;
	while(!(I2C1->SR1 & BV(I2C_SR1_RXNE_Pos)));
    uint16_t data = I2C1->DR;
    return data;
}

uint16_t I2C_recvdatanack(void) {
	I2C1->CR1 &= ~(I2C_CR1_ACK | I2C_CR1_POS);
	// wait until receive buffer is not empty
	while (!(I2C1->SR1 & I2C_SR1_RXNE));
	// read content and clear flags
	uint16_t val = I2C1->DR;
	return val;
}

int I2C_IsDeviceReady(uint8_t addr)
{
		// DR -- write slave addr in data regr
	    I2C1->DR = addr;
		// SR1 -- poll for addr is transferred
	    while (!(I2C1->SR1 & I2C_SR1_ADDR));
	    // dummy read to clear flags
	    (void)I2C1->SR1;
	    (void)I2C1->SR2;
	    return 1;
}





