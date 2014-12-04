//Multiplexer schalten
//Geschwindigkeit einstellen 10x so oft abtasten
//PIN und PORT ausschalten GPIO
//clock enable
//enable adc
//trigger source
//Schmidttrigger ausschalten
//Messauflösung einstellen
//AD Wandler kann mehrere Messwerte in einer Messung durchführen?
//clock from APB2(72Mhz) + Prescaler->36MHz IST NICHT SAMPLERATE!!! 6 bis 12(Anzahl der Bits von der Auflösung) +3 Zyklen braucht der AD-Wandler für eine Messung
//kontinuirliche Messung? oder Interrupt gesteuert
//6-12Bit Ergebnis kommt in 16Bit Register->links oder rechtsbündig das Ergebnis rein schreiben
//Tempsensor braucht dass der Samplingschalter mindestens 10us geschlossen ist
//2.5mV /°C und 0.76V bei 25°C
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f4xx_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void ADC_Config(void){
ADC_InitTypeDef ADC_InitStruc;
ADC_CommonInitTypeDef ADC_CommonInitStruc;
ADC_DeInit();
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
ADC_InitStruc.ADC_Resolution=ADC_Resolution_12b;
ADC_InitStruc.ADC_ScanConvMode=DISABLE;
ADC_InitStruc.ADC_ContinuousConvMode=ENABLE;
ADC_InitStruc.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
ADC_InitStruc.ADC_DataAlign=ADC_DataAlign_Right;
ADC_InitStruc.ADC_NbrOfConversion=1;
ADC_Init(ADC1,&ADC_InitStruc);
ADC_CommonInitStruc.ADC_Mode=ADC_Mode_Independent;
ADC_CommonInitStruc.ADC_Prescaler=ADC_Prescaler_Div4;
ADC_CommonInitStruc.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
ADC_CommonInitStruc.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_10Cycles;	
ADC_CommonInit(&ADC_CommonInitStruc);
ADC_RegularChannelConfig(ADC1,ADC_Channel_TempSensor,1,ADC_SampleTime_144Cycles);	
ADC_TempSensorVrefintCmd(ENABLE);
ADC_Cmd(ADC1,ENABLE);
}


int main2(void){
float TemperatureValue = 0;
ADC_Config();
ADC_SoftwareStartConv(ADC1);
while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
TemperatureValue = ADC_GetConversionValue(ADC1);
TemperatureValue *= 3300;
TemperatureValue /= 0xfff; 
TemperatureValue /= 1000.0;
TemperatureValue -= 0.760; 
TemperatureValue /= .0025; 
TemperatureValue += 25.0; 
}
