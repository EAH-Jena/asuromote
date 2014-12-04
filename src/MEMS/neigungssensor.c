#include "stm32f4xx.h"                  // Device header
#include "GPIO_STM32F4xx.h"             // Keil::Device:GPIO
#include "BoardLEDs.h"
#include "Spi.h"

int8_t x = 0, y = 0, z = 0;
void wait(int ms){
		volatile uint32_t i = 0;
		i = ms*16800;
		while(i>0){
			i--;
		}
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
