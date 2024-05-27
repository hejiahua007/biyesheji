#ifndef __USART2_H
#define __USART2_H 			   

#include "stm32f10x.h"
#include "stdio.h"

void uart2_Init(u32 bound);
void usart2_SendByte(uint8_t Byte);
void USART2_SendString(char *String);
uint8_t USART2_GetRxFlag(void);

void uart3_Init(u32 bound);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

#endif
