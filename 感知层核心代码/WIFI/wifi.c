/*
 * 本代码用于配置ESP8266 Wi-Fi模块，初始化串口通信，发送和接收数据。
 * 主要包括DMA传输的配置，USART2的初始化，Wi-Fi和TCP连接状态的检查。
 * 通过USART2与ESP8266通信，实现无线数据传输。
 */

#include "wifi.h"
#include "delay.h"
#include "stdio.h"
#include "led.h"
#include "usart.h"

/**********************/
uint8_t Rcv_buf[RCVBUF_LEN]; // 接收缓冲区
uint8_t Sed_buf[SEDBUF_LEN]; // 发送缓冲区
char str1[100] = {0};
char str2[100] = {0};
uint8_t SG90 = 0;
uint8_t Led = 0;
/**********************/

/**********************/
static void dma1_channel7_usart2_tx(void); // DMA1通道7 USART2传输初始化
static void dma1_channel6_usart2_rx(void); // DMA1通道6 USART2接收初始化
/**********************/

// USART2初始化
void wifi_init(void)
{
	// 结构体定义
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// GPIO口和USART2的时钟使能配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// USART2的NVIC配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	// PA3配置为输入浮空
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA2配置为复用功能输出推挽
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART2配置
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);

	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); // 总线空闲中断

	dma1_channel7_usart2_tx(); // 初始化DMA1通道7用于USART2传输
	dma1_channel6_usart2_rx(); // 初始化DMA1通道6用于USART2接收

	USART_Cmd(USART2, ENABLE);
}

// DMA1通道7 USART2传输初始化
static void dma1_channel7_usart2_tx(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	// 使能时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Sed_buf; // 内存地址在传输时设置
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

	DMA_Cmd(DMA1_Channel7, DISABLE);
}

// DMA1通道6 USART2接收初始化
static void dma1_channel6_usart2_rx(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	// 使能时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitStructure.DMA_BufferSize = RCVBUF_LEN; // 传输大小在传输时设置
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Rcv_buf; // 内存地址在传输时设置
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);

	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Channel6, ENABLE);
}

// DMA1通道7传输到USART2发送端
static void dma1_ch7_to_usart2tx(uint32_t size)
{
	DMA_Cmd(DMA1_Channel7, DISABLE); // 关闭DMA才能修改CNDTR寄存器
	DMA_SetCurrDataCounter(DMA1_Channel7, size);
	DMA_Cmd(DMA1_Channel7, ENABLE);
}

// DMA1通道6传输到USART2接收端
static void dma1_ch6_to_usart2rx(uint32_t size)
{
	DMA_Cmd(DMA1_Channel6, DISABLE); // 关闭DMA才能修改CNDTR寄存器
	DMA_SetCurrDataCounter(DMA1_Channel6, size);
	DMA_Cmd(DMA1_Channel6, ENABLE);
}

// 发送数据
void Send_Data(uint8_t *Pcmd)
{
	uint8_t len = strlen((const char *)Pcmd);		 // 获取长度
	strcpy((char *)Sed_buf, (const char *)Pcmd); // 把数据复制到全局数组
	dma1_ch7_to_usart2tx(len);									 // 通过DMA发送数据
}

// 发送指令并等待响应
void Send_Cmd_Wait_Cmd(uint8_t *SendPcmd, uint8_t *WaitPcmd)
{
	memset(Rcv_buf, 0, RCVBUF_LEN); // 清空接收缓冲区
	Send_Data(SendPcmd);						// 发送指令到模块

	while (1)
	{
		if (dmaflag) // 如果DMA标志被设置，表示数据已经准备好
		{
			if (strstr((const char *)Rcv_buf, (const char *)WaitPcmd) != NULL)
				break; // 如果接收缓冲区中有等待的指令，则跳出循环
		}
	}
	dmaflag = !dmaflag; // 重置DMA标志，为下一次接收准备
}

__IO uint8_t dmaflag = 0; // DMA标志

// USART2中断处理程序
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
	{
		dma1_ch6_to_usart2rx(RCVBUF_LEN); // 重新配置DMA接收缓冲区
		dmaflag = 1;
		printf(Rcv_buf);					 // 打印接收的数据
		USART_ReceiveData(USART2); // 读取数据寄存器以清除中断标志
	}
}

// 检测ESP模块是否已连接到Wi-Fi网络
int isWifiConnected()
{
	memset(Rcv_buf, 0, RCVBUF_LEN); // 清空接收缓冲区
	dmaflag = 0;										// 重置DMA标志

	// 发送AT命令获取当前Wi-Fi连接状态
	Send_Data("AT+CWJAP?\r\n");

	while (1)
	{
		if (dmaflag) // 如果DMA标志被设置，表示数据已经准备好
		{
			if (strstr(Rcv_buf, "+CWJAP:") != NULL)
			{
				return 1; // 已连接到Wi-Fi网络
			}
			if (strstr(Rcv_buf, "ERROR") != NULL)
			{
				return 0; // 未连接到Wi-Fi网络
			}
			dmaflag = 0; // 重置DMA标志，为下一次接收准备
		}
	}

	return 0;
}

// 检测ESP模块是否已连接到TCP服务器
int isTcpConnected()
{
	memset(Rcv_buf, 0, RCVBUF_LEN); // 清空接收缓冲区
	dmaflag = 0;										// 重置DMA标志

	// 发送AT命令获取当前TCP连接状态
	Send_Data("AT+CIPSTATUS\r\n");

	while (1)
	{
		if (dmaflag) // 如果DMA标志被设置，表示数据已经准备好
		{
			if (strstr(Rcv_buf, "STATUS:3") != NULL)
			{
				return 1; // 已建立TCP连接
			}
			if (strstr(Rcv_buf, "STATUS:2") != NULL)
			{
				return 0; // 未建立TCP连接
			}
			dmaflag = 0; // 重置DMA标志，为下一次接收准备
		}
	}

	return 0;
}

// ESP初始化函数
void esp_init(void)
{
	delay_ms(3000);										 // 延时等待模块稳定
	Send_Cmd_Wait_Cmd("AT\r\n", "OK"); // 发送AT命令确认模块启动

	if (isWifiConnected())
	{
		printf("WIFI连接成功\r\n");
	}
	else
	{
		Send_Cmd_Wait_Cmd("AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n", "OK"); // 连接到指定Wi-Fi网络
		printf("WIFI连接成功\r\n");
	}

	if (isTcpConnected())
	{
		printf("TCP连接成功\r\n");
	}
	else
	{
		Send_Cmd_Wait_Cmd("AT+CIPSTART=\"TCP\",\"SERVER_IP\",SERVER_PORT\r\n", "OK"); // 连接到指定TCP服务器
		printf("尝试连接TCP服务器\r\n");
	}
	printf("配置完成");

	delay_ms(2000);
}
