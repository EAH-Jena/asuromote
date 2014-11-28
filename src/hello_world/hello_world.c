#include "stm32f4xx.h"                  // Device header
#include "GPIO_STM32F4xx.h"             // Keil::Device:GPIO
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

/*void wait(void){
	volatile int t=10000;
	while(t) t--;
}*/
osThreadId led_blau;
osThreadId led_rot;
enum{blau, rot};

void set_led(int id, int on){
	switch(id){
		case blau: GPIO_PinWrite(GPIOD, 15,on);
			break;
		case rot: GPIO_PinWrite(GPIOD, 14,on);
			break;
	}
}
void switch_blau(void const *argument){
	while(1){
		set_led(blau,1);
		osDelay(300);
		set_led(blau,0);
		osDelay(300);
	}
}
void switch_rot(void const *argument){
	while(1){
		set_led(rot,1);
		osDelay(1000);
		set_led(rot,0);
		osDelay(1000);
	}
}
osThreadDef(switch_blau,osPriorityNormal, 1,0);
osThreadDef(switch_rot,osPriorityNormal, 1,0);
void main1(void){
	//init LEDs
	//RCC->AHB1ENR |=(1<<3);// Pfeil entspricht: *(RCC.AHB1ENR)      PORTD einschalten
	GPIO_PortClock(GPIOD,1);
	
	/*GPIOD->MODER |= 1<<30;
	GPIOD->MODER &=~(1U<<31);//PD15 als Ausgang, 2Register müssen konfiguriert werden da es pro Pin 4
													//Einstellmöglichkeiten gibt
	GPIOD->OTYPER &=~(1<<15);//OutputType
	GPIOD->OSPEEDR &=~(3U<<30);//OutputSpeed ->Flankensteilheit
	GPIOD->PUPDR &=~(3U<<30);//PullUp PullDown*/
	GPIO_PinConfigure(GPIOD,15,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_2MHz,GPIO_NO_PULL_UP_DOWN);
	GPIO_PinConfigure(GPIOD,14,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_2MHz,GPIO_NO_PULL_UP_DOWN);
	led_blau = osThreadCreate(osThread(switch_blau),NULL);
	led_rot = osThreadCreate(osThread(switch_rot),NULL);
	osDelay(osWaitForever);
	/*while(1){
		GPIOD->ODR |= (1<<15);
		wait();
		GPIOD->ODR &=~(1<<15);
		wait();
	}*/
	
}
