/*
Author: Nilesh Ghule <nilesh@sunbeaminfo.com>
Course: PG-DESD @ Sunbeam Infotech
Date: Sep 25, 2024
*/

#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx.h"

// SPI1 -- GPIOA (Alt Fn 5)
#define SPI_SCLK_PIN	5
#define SPI_MISO_PIN	6
#define SPI_MOSI_PIN	7

// CS -- GPIOE
#define SPI_CS_PIN		3

void SPI_Init(void);
void SPI_CS_Enable(void);
void SPI_CS_Disable(void);
uint16_t SPI_Transmit(uint16_t val);
uint16_t SPI_Receive(void);
void SPI_ReadIO(uint8_t regr, uint8_t data[], uint8_t size);

#endif /* SPI_H_ */
