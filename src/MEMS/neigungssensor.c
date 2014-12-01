#include "stm32f4xx.h"                  // Device header
#include "GPIO_STM32F4xx.h"             // Keil::Device:GPIO
#include "stm32f4xx_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f4xx_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

int8_t x = 0, y = 0, z = 0;
void wait(int ms){
		volatile uint32_t i = 0;
		i = ms*16800;
		while(i>0){
			i--;
		}
}
enum{blau, rot, gruen, orange, all};
void init_LEDs(){
		GPIO_PortClock(GPIOD,1);
		//LED green
		GPIO_PinConfigure(GPIOD,12,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_2MHz,GPIO_NO_PULL_UP_DOWN);
		//LED orange
		GPIO_PinConfigure(GPIOD,13,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_2MHz,GPIO_NO_PULL_UP_DOWN);
		//LED red
		GPIO_PinConfigure(GPIOD,14,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_2MHz,GPIO_NO_PULL_UP_DOWN);
		//LED blue
		GPIO_PinConfigure(GPIOD,15,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_2MHz,GPIO_NO_PULL_UP_DOWN);
}
void switch_LED(int farbe, int wert){
		//wert = 0 ->set off
		//wert = 1 ->set on
		//wert = 2 ->toggle LED
		if(wert<2){
				switch(farbe){
						case blau:
							GPIO_PinWrite(GPIOD,15,wert);//LED blue
							break;
						case gruen:
							GPIO_PinWrite(GPIOD,12,wert);//LED green
							break;
						case rot:
							GPIO_PinWrite(GPIOD,14,wert);//LED red
							break;
						case orange:
							GPIO_PinWrite(GPIOD,13,wert);//LED orange
							break;
						case all:
							GPIO_PinWrite(GPIOD,15,wert);//LED blue
							GPIO_PinWrite(GPIOD,12,wert);//LED green
							GPIO_PinWrite(GPIOD,14,wert);//LED red
							GPIO_PinWrite(GPIOD,13,wert);//LED orange
							break;
				}
		}
		else{
				switch(farbe){
						case blau:
							GPIO_PinWrite(GPIOD,15,~GPIO_PinRead(GPIOD,15));//LED blue
							break;
						case gruen:
							GPIO_PinWrite(GPIOD,12,~GPIO_PinRead(GPIOD,12));//LED green
							break;
						case rot:
							GPIO_PinWrite(GPIOD,14,~GPIO_PinRead(GPIOD,14));//LED red
							break;
						case orange:
							GPIO_PinWrite(GPIOD,13,~GPIO_PinRead(GPIOD,13));//LED orange
							break;
						case all:
							GPIO_PinWrite(GPIOD,15,~GPIO_PinRead(GPIOD,15));//LED blue
							GPIO_PinWrite(GPIOD,12,~GPIO_PinRead(GPIOD,12));//LED green
							GPIO_PinWrite(GPIOD,14,~GPIO_PinRead(GPIOD,14));//LED red
							GPIO_PinWrite(GPIOD,13,~GPIO_PinRead(GPIOD,13));//LED orange
							break;
				}
		}
}
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
void init_MEM(){
		//Enable GPIO Peripherie Clock für PORT A und E (SCK, MOSI, MISO and NSS GPIO clocks)
		GPIO_PortClock(GPIOA,1);
		GPIO_PortClock(GPIOE,1);
		
		//Enable GPIO Peripherie Clock für PORT D (Chip select)
		GPIO_PortClock(GPIOD,1);
		
		//alternative Funktion benutzen
		/*RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOJ,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOK,ENABLE);*/
	
		//Configuring der SCK, MISO, MOSI -Pin´s mit AF-MODE
		GPIO_PinConfigure(GPIOA,5,GPIO_MODE_AF,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_50MHz,GPIO_NO_PULL_UP_DOWN);
		GPIO_PinConfigure(GPIOA,6,GPIO_MODE_AF,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_50MHz,GPIO_NO_PULL_UP_DOWN);
		GPIO_PinConfigure(GPIOA,7,GPIO_MODE_AF,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_50MHz,GPIO_NO_PULL_UP_DOWN);
		
		//Configuring ChipSelect-Pin PE3 
		GPIO_PinConfigure(GPIOE,3,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_50MHz,GPIO_PULL_UP); 

		//Pin´s ans SPI1 koppeln
		GPIO_PinAF(GPIOA, 5, GPIO_AF_SPI1);
		GPIO_PinAF(GPIOA, 6, GPIO_AF_SPI1);
		GPIO_PinAF(GPIOA, 7, GPIO_AF_SPI1);
		
		GPIO_PinWrite(GPIOE,3,1);//Chipselect high
		wait(10);//warten bis MEM/SPI hochgefahren ist
		//CTRL 4 init
		mySPI_SendData(0x20,0x7F);
		//CTRL 5 init
		mySPI_SendData(0x24,0x00);
}
int check_MEMs_who_am_i(){
	volatile int8_t tmp = 0;
	tmp = mySPI_RecvData(0x0F);
		//SPI Verbindung auf Richtigkeit prüfen mit dem "WHO AM I" Register
		if(tmp==63){
				return 1;
		}
		else{
				return 0;
		}
}
enum{X,Y,Z};
int8_t get_MEMs_Axis_value(int axis){
	volatile int8_t t;
	switch(axis){
		case X:
			t = mySPI_RecvData(0x29)-x;
			break;
		case Y:
			t = mySPI_RecvData(0x2B)-y;
			break;
		case Z:
			t = mySPI_RecvData(0x2D)-z;
			break;
		default:
			t = 0xFF;
			break;
	}
	return t;
}
void set_MEMs_Offsets(){
		mySPI_SendData(0x10,0);//X-Offset
		mySPI_SendData(0x11,0);//Y-Offset
		mySPI_SendData(0x12,0);//Z-Offset
		x = get_MEMs_Axis_value(X);
		y = get_MEMs_Axis_value(Y);
		z = get_MEMs_Axis_value(Z);
}
void reboot_mem(){
	volatile uint16_t index = 0;
	//reboot matze
	mySPI_SendData(0x25,128);
	while(index<30000){
			index++;
	}
	//remove matze
}
int main(void){
		//vier CursorLEDs einrichten
		init_LEDs();
		//SPI1 für Motionsensor einrichten
		init_SPI(SPI_1);
		//GPIO SCK,MOSI usw. einrichten für Motionsensor und Controlregister beschreiben
		init_MEM();
		//SPI zum MEM prüfen, bei korrekt gehen alle 4 LEDs an
		if(check_MEMs_who_am_i()){
				switch_LED(all,1);
				wait(1000);
				switch_LED(all,0);
		}
		//Offset einstellen
		set_MEMs_Offsets();
		while(1){
				if(get_MEMs_Axis_value(Y)>0){
					switch_LED(blau,1);//LED blue on
					switch_LED(orange,0);//LED orange off
				}
				if(get_MEMs_Axis_value(Y)<0){
					switch_LED(blau,0);//LED blue off
					switch_LED(orange,1);//LED orange on
				}
				if(get_MEMs_Axis_value(X)>0){
					switch_LED(gruen,1);//LED green on
					switch_LED(rot,0);//LED red off
				}
				if(get_MEMs_Axis_value(X)<0){
					switch_LED(gruen,0);//LED green off
					switch_LED(rot,1);//LED red on
				}
				wait(100);
		}
}
