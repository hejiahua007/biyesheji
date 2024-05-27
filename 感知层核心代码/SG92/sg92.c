

#include "sg92.h"

void TIM3_PWM_Init(u16 arr,u16 psc)//arr：自动重装值  //psc：时钟预分频数
{  
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
        
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);        //使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
        
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5   

            
    //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形     
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIO
        
                //初始化TIM3
    TIM_TimeBaseStructure.TIM_Period = arr;   // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler =psc; // 预分频值 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; // 设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); // 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
        
     //结构体赋值     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;    // 选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
                
     //初始化TIM3 Channe2 PWM模式  
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  // 根据T指定的参数初始化外设TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    
    TIM_Cmd(TIM3, ENABLE);  //使能TIM3

}


void Servo_Control(uint16_t angle)
{
	float temp;
	temp = (1.0 / 9.0) * angle + 5.0;//比较值寄存器值= 1/9 * 输入角度 + 5
	TIM_SetCompare2(TIM3, (uint16_t)temp);//用来控制占空比，因为使能的TIM3
}

void SG90_On(void){
	Servo_Control(180);//用来控制占空比，因为使能的TIM3
}

void SG90_Off(void){
	Servo_Control(0);//用来控制占空比，因为使能的TIM3
}

