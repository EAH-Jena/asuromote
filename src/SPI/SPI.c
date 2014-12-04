#include "stm32f4xx_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f4xx_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "GPIO_STM32F4xx.h"             // Keil::Device:GPIO

enum{SPI_1, SPI_2, SPI_3, SPI_4, SPI_5, SPI_6};
void init_SPI(int spi_number){
		SPI_InitTypeDef SPI_InitTypeDefStruct;
		//SPI1 initialisieren
		//Program the Polarity, Phase, First Data, Baud Rate Prescaler, Slave 
		//Management, Peripheral Mode and CRC Polynomial values
		SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
		SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//bidirectional for read and write
		SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;//als master
		SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;//Chip select to Software
		SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_High;//clock auf steigende Flanke?
		SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_2Edge;
		
	//Enable peripheral clock
		switch(spi_number){
				case SPI_1:
						RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);// for SPI1
						//SPI1 mit struct initialisieren 
						SPI_Init(SPI1, &SPI_InitTypeDefStruct);
						SPI_Cmd(SPI1, ENABLE);
						break;
				case SPI_2:
						RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);// for SPI2
						SPI_Init(SPI2, &SPI_InitTypeDefStruct);
						SPI_Cmd(SPI2, ENABLE);
						break;
				case SPI_3:
						RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);// for SPI3
						SPI_Init(SPI3, &SPI_InitTypeDefStruct);
						SPI_Cmd(SPI3, ENABLE);
						break;
				case SPI_4:
						RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);// for SPI4
						SPI_Init(SPI4, &SPI_InitTypeDefStruct);
						SPI_Cmd(SPI4, ENABLE);
						break;
				case SPI_5:
						RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);// for SPI5
						SPI_Init(SPI5, &SPI_InitTypeDefStruct);
						SPI_Cmd(SPI5, ENABLE);
						break;
				case SPI_6:
						RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);// for SPI6
						SPI_Init(SPI6, &SPI_InitTypeDefStruct);
						SPI_Cmd(SPI6, ENABLE);
						break;
		}
}
void mySPI_SendData(uint8_t adress, uint8_t data){
		GPIO_PinWrite(GPIOE, 3,0);
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
		SPI_I2S_SendData(SPI1, adress);
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
		SPI_I2S_ReceiveData(SPI1);
		 
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
		SPI_I2S_SendData(SPI1, data);
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
		SPI_I2S_ReceiveData(SPI1);
		GPIO_PinWrite(GPIOE, 3,1);
}
int8_t mySPI_RecvData(uint8_t adress){
		GPIO_PinWrite(GPIOE, 3,0);
		adress = adress | 0x80;
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
		SPI_I2S_SendData(SPI1, adress);
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
		SPI_I2S_ReceiveData(SPI1);
		 
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
		SPI_I2S_SendData(SPI1, 0x00);
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
		
		GPIO_PinWrite(GPIOE, 3,1);
	return SPI_I2S_ReceiveData(SPI1);
}
