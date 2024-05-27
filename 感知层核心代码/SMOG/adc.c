#include "adc.h"

/*******************************************************************************
*������ԭ�ͣ�void ADC_Pin_Init(void)
*�����Ĺ��ܣ�GPIO��ʼ��
*�����Ĳ�����None
*��������ֵ��None
*������˵����PA0 ADC1_IN0 ģ������
*������д�ߣ���
*������д���ڣ�2021/2/28
*�����İ汾�ţ�V1.0
********************************************************************************/
void ADC_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
						| RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//����ת��
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//���ݶ���
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_InitStruct.ADC_NbrOfChannel = 1;//ת������
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//��ͨ��ɨ��
	ADC_Init(ADC1,&ADC_InitStruct);
	
	//ת��ͨ�� �ڼ���ת�� ����ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);//
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	ADC_Cmd(ADC1,ENABLE);
}

/*******************************************************************************
*������ԭ�ͣ�u16 ADC_Trans(void)
*�����Ĺ��ܣ�ADC��ȡ����
*�����Ĳ�����None
*��������ֵ��
	@ u16������50�ε�ƽ��ֵ
*������˵����
*������д�ߣ���
*������д���ڣ�2021/2/28
*�����İ汾�ţ�V1.0
********************************************************************************/
u16 ADC_Trans(void)
{
	u16 adc_value = 0;
	u8 i = 0;
	
	for(i = 0; i < 50; i++)
	{ 
		//��ʼת��
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		
		//ת���Ƿ����
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET);
		adc_value = adc_value + ADC_GetConversionValue(ADC1);//��ADC�е�ֵ
	}
	
	return adc_value / 50;
}
