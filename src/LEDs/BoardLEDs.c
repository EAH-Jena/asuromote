#include "GPIO_STM32F4xx.h"             // Keil::Device:GPIO
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
