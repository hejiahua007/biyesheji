#include "wifi.h"
#include "delay.h"
#include "sg92.h"
#include "stdio.h"
#include "led.h"
#include "usart.h"
/**********************/
uint8_t Rcv_buf[RCVBUF_LEN];
uint8_t Sed_buf[SEDBUF_LEN];
char str1[100] = {0};
char str2[100] = {0};
uint8_t SG90 = 0;
uint8_t Led = 0;
/**********************/

/**********************/
static void dma1_channel7_usart2_tx(void);
static void dma1_channel6_usart2_rx(void);

/**********************/

// USART2
void wifi_init(void)
{
	// 结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	// 串口通信（异步通信，全双工）
	USART_InitTypeDef USART_InitStructure;
	// 嵌套向量中断控制器
	NVIC_InitTypeDef NVIC_InitStructure;
	// GPIO口和USART2的时钟的使能配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// USART2 的 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;					//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//
	NVIC_Init(&NVIC_InitStructure);

	// PA3 配置为输入浮空
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA2 配置为复用功能输出推挽
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART2 配置
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);

	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
	// USART_IT_IDLE：当USART接收到一帧数据后，在接收行为完成之前会出发这个中断。
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); // 总线空闲中断

	dma1_channel7_usart2_tx();
	dma1_channel6_usart2_rx();

	USART_Cmd(USART2, ENABLE);
}
// 直接寄存器
static void dma1_channel7_usart2_tx(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	//	NVIC_InitTypeDef NVIC_InitStructure;

	// 使能时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Sed_buf; // 内存地址暂不设置，等到传输时才设置
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

static void dma1_channel6_usart2_rx(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	//	NVIC_InitTypeDef NVIC_InitStructure;

	// 使能时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//	// DMA1 通道4 中断配置

	DMA_InitStructure.DMA_BufferSize = RCVBUF_LEN; // 传输大小暂不设置，等到要传输时才设置
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Rcv_buf; // 内存地址暂不设置，等到传输时才设置
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);

	// DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Channel6, ENABLE);
}

// DMA1 通道7 传输到 USART2 发送端
static void dma1_ch7_to_usart2tx(uint32_t size)
{

	DMA_Cmd(DMA1_Channel7, DISABLE); // 关闭 DMA 才能修改 CNDTR 寄存器
	DMA_SetCurrDataCounter(DMA1_Channel7, size);
	DMA_Cmd(DMA1_Channel7, ENABLE);
}

// DMA1 通道7 传输到 USART2 发送端
static void dma1_ch6_to_usart2rx(uint32_t size)
{

	DMA_Cmd(DMA1_Channel6, DISABLE); // 关闭 DMA 才能修改 CNDTR 寄存器
	DMA_SetCurrDataCounter(DMA1_Channel6, size);
	DMA_Cmd(DMA1_Channel6, ENABLE);
}

void Send_Data(uint8_t *Pcmd)
{
	// 获取长度
	uint8_t len = strlen((const char *)Pcmd);
	// 把数据复制到全局数组
	strcpy((char *)Sed_buf, (const char *)Pcmd);
	dma1_ch7_to_usart2tx(len);
}

void Send_Cmd_Wait_Cmd(uint8_t *SendPcmd, uint8_t *WaitPcmd)
{
	memset(Rcv_buf, 0, RCVBUF_LEN); // 清空接收缓冲区
	Send_Data(SendPcmd);						// 发送指令到模块

	while (1)
	{ // 开始无限循环等待响应

		if (dmaflag)
		{ // 如果DMA标志被设置，表示数据已经准备好
			if (strstr((const char *)Rcv_buf, (const char *)WaitPcmd) != NULL)
				break; // 如果接收缓冲区中有等待的指令，则跳出循环
		}
	}
	dmaflag = !dmaflag; // 重置DMA标志，为下一次接收准备
}

__IO uint8_t dmaflag = 0;

// 增加操作
//  USART2 中断处理程序
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
	{ // 收完了
		dma1_ch6_to_usart2rx(RCVBUF_LEN);
		dmaflag = 1;
		printf(Rcv_buf);
		USART_ReceiveData(USART2); // 总线空闲时也要读取数据寄存器
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
	{ // 等待接收到响应
		if (dmaflag)
		{ // 如果DMA标志被设置，表示数据已经准备好
			// 检查响应中是否含有"+CWJAP:"，表示已连接到Wi-Fi网络
			if (strstr(Rcv_buf, "+CWJAP:") != NULL)
			{
				return 1;
			}
			if (strstr(Rcv_buf, "ERROR") != NULL)
			{
				return 0;
			}
			dmaflag = 0; // 重置DMA标志，为下一次接收准备
		}
		// 在此可以添加超时判断，避免无限循环等待响应
	}

	return 0;
}
// 检测ESP模块是否已连接到Wi-Fi网络
int isTcpConnected()
{
	memset(Rcv_buf, 0, RCVBUF_LEN); // 清空接收缓冲区
	dmaflag = 0;										// 重置DMA标志

	// 发送AT命令获取当前TCP连接状态
	Send_Data("AT+CIPSTATUS\r\n");

	while (1)
	{ // 等待接收到响应
		if (dmaflag)
		{ // 如果DMA标志被设置，表示数据已经准备好
			// 检查响应中是否含有"STATUS:3"，表示已建立TCP连接
			if (strstr(Rcv_buf, "STATUS:3") != NULL)
			{
				return 1;
			}
			// 检查响应中是否含有"STATUS:"，已连接 AP，获得 IP 地址
			if (strstr(Rcv_buf, "STATUS:2") != NULL)
			{
				return 0;
			}
			dmaflag = 0; // 重置DMA标志，为下一次接收准备
		}
		// 在此可以添加超时判断，避免无限循环等待响应
	}

	return 0;
}

void esp_init(void)
{

	//	Send_Data("+++\r\n");
	//	// 测试 AT 启动
	//	Send_Cmd_Wait_Cmd("AT\r\n","OK");
	//    //
	//	Send_Cmd_Wait_Cmd("ATE0\r\n","OK");
	//    //重启模块
	//	Send_Cmd_Wait_Cmd("AT+RST\r\n","ready");
	//    //设置Wi-Fi模式并保存到flash
	//  Send_Cmd_Wait_Cmd("AT+CWMODE_DEF=1\r\n","OK");

	// 连接ap
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	Send_Cmd_Wait_Cmd("AT\r\n", "OK");
	if (isWifiConnected())
	{
		printf("WIFI连接成功\r\n");
	}
	else
	{
		Send_Cmd_Wait_Cmd("AT+CWJAP=\"***\",\"***\"\r\n", "OK");
		printf("WIFI连接成功\r\n");
	}

	// 检查TCP连接状态
	if (isTcpConnected())
	{
		printf("TCP连接成功\r\n");
	}
	else
	{
		// 尝试建立TCP连接
		Send_Cmd_Wait_Cmd("AT+CIPSTART=\"TCP\",\"***\",***\r\n", "OK");
		printf("尝试连接TCP服务器\r\n");
	}
	printf("配置完成");

	delay_ms(2000);
}
