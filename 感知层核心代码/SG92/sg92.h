#ifndef _SG92_H
#define _SG92_H

#include "stm32f10x.h"


void TIM3_PWM_Init(u16 arr,u16 psc);
void SG90_On(void);
void SG90_Off(void);

extern uint8_t Door;

#endif

