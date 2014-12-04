#ifndef __Spi_H
#define __Spi_H
#endif

#include "stm32f4xx_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f4xx_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "GPIO_STM32F4xx.h"             // Keil::Device:GPIO
enum{SPI_1, SPI_2, SPI_3, SPI_4, SPI_5, SPI_6};
void init_SPI(int spi_number);
void mySPI_SendData(uint8_t adress, uint8_t data);
int8_t mySPI_RecvData(uint8_t adress);
