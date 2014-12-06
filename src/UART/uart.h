#ifndef __UART_H__
#define __UART_H__

#include <stm32f4xx.h>
#include <stm32f4xx_usart.h>
#include <inttypes.h>

void USART1_init(uint32_t baudrate);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);

#endif
