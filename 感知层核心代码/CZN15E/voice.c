#include "stm32f10x.h"                
#include "voice.h"
void VoiceSensor_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//ʹ��PA11��
 
unsigned char VoiceSensor_Get(void)     //��ȡPA11�ڵĵ�ƽ
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11);
}
 
