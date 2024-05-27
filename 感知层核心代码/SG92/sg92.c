

#include "sg92.h"

void TIM3_PWM_Init(u16 arr,u16 psc)//arr���Զ���װֵ  //psc��ʱ��Ԥ��Ƶ��
{  
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
        
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);        //ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
        
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5   

            
    //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��     
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   //��ʼ��GPIO
        
                //��ʼ��TIM3
    TIM_TimeBaseStructure.TIM_Period = arr;   // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc; // Ԥ��Ƶֵ 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; // ����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); // ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
        
     //�ṹ�帳ֵ     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;    // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
                
     //��ʼ��TIM3 Channe2 PWMģʽ  
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  // ����Tָ���Ĳ�����ʼ������TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    
    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3

}


void Servo_Control(uint16_t angle)
{
	float temp;
	temp = (1.0 / 9.0) * angle + 5.0;//�Ƚ�ֵ�Ĵ���ֵ= 1/9 * ����Ƕ� + 5
	TIM_SetCompare2(TIM3, (uint16_t)temp);//��������ռ�ձȣ���Ϊʹ�ܵ�TIM3
}

void SG90_On(void){
	Servo_Control(180);//��������ռ�ձȣ���Ϊʹ�ܵ�TIM3
}

void SG90_Off(void){
	Servo_Control(0);//��������ռ�ձȣ���Ϊʹ�ܵ�TIM3
}

