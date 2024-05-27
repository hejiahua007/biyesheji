//#include "usart2.h"
//#include "stm32f10x.h"                  // Device header
//#include <stdio.h>
//#include <stdarg.h>
////#include "tcp.h"
//uint8_t Usart2_RxData;		//定义串口接收的数据变量
//uint8_t Usart2_RxFlag;		//定义串口接收的标志位变量
//volatile u8 TcpClosedFlag = 0;

//void uart2_Init(u32 bound)//串口2  引脚为PA2  PA3
//{
//	USART_InitTypeDef USART_InitStructure;
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能指定端口时钟
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIO
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIO
//	
//	//Usart2 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         
//  NVIC_Init(&NVIC_InitStructure); 

//	
//	//USART2配置
//	USART_InitStructure.USART_BaudRate = bound;	//设置串口波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶校验
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无流控
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//	USART_Init(USART2, &USART_InitStructure); //配置USART参数
//	
//	USART_ITConfig(USART2, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//配置了接收中断和总线空闲中断
//	
//	USART_Cmd(USART2, ENABLE);                    //使能USART
//}
//void usart2_SendByte(uint8_t Byte)
//{
//	USART_SendData(USART2, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
//	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送完成
//	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
//}
//void USART2_SendString(char *String)
//{
//	uint8_t i;
//	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
//	{
//		usart2_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
//	}
//}
//uint8_t USART2_GetRxFlag(void)
//{
//	if (Usart2_RxFlag == 1)			//如果标志位为1
//	{
//		Usart2_RxFlag = 0;
//		return 1;					//则返回1，并自动清零标志位
//	}
//	return 0;						//如果标志位为0，则返回0
//}
//void USART2_IRQHandler(void)
//{   
//    u8 ucCh;

//    if(USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET )
//    {
//        Usart2_RxData  = USART_ReceiveData( USART2 );
//			  usart2_SendByte(Usart2_RxData);
//				Usart2_RxFlag = 1;
//				USART_ClearITPendingBit(USART2, USART_IT_RXNE);	

//    }

//}


//void uart3_Init(u32 bound)//串口3  引脚为PB10  PB11
//{

//    GPIO_InitTypeDef GPIO_InitStructure;
//    USART_InitTypeDef USART_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //?′ó?í?íìê?3?
//    GPIO_Init(GPIOB, &GPIO_InitStructure);


//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

//    //Usart3 NVIC 配置
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //响应优先级0
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //USART_IRQn通道使能
//    NVIC_Init(&NVIC_InitStructure); //初始化NVIC

//    //USART3 配置
//    USART_InitStructure.USART_BaudRate = bound;//波特率
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据长度
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位1
//    USART_InitStructure.USART_Parity = USART_Parity_No;//校验位无
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制无
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //使能串口的接收和发送功能
//    USART_Init(USART3, &USART_InitStructure); //初始化串口

//    USART_ITConfig(USART3, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//配置了接收中断和总线空闲中断

//    USART_Cmd(USART3, ENABLE);      //串口外设使能    
//}

////void USART3_IRQHandler( void )
////{   
////    u8 ucCh;

////    if(USART_GetITStatus( USART3, USART_IT_RXNE ) != RESET )
////    {
////        ucCh  = USART_ReceiveData( USART3 );

////        if(ESP8266_Fram_Record_Struct .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
////        {
////            //留最后一位做结束位
////            ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = ucCh;   
////        }                      
////    }
////		
////    if( USART_GetITStatus( USART3, USART_IT_IDLE ) == SET )                              //如果总线空闲
////    {
////        ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;
////        ucCh = USART_ReceiveData( USART3 );                                                              //由软件序列清除中断标志位（先读USART_SR,然后读USART_DR）
////        TcpClosedFlag = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
////	
////    }   

////}
