/*
 * ��������������ESP8266 Wi-Fiģ�飬��ʼ������ͨ�ţ����ͺͽ������ݡ�
 * ��Ҫ����DMA��������ã�USART2�ĳ�ʼ����Wi-Fi��TCP����״̬�ļ�顣
 * ͨ��USART2��ESP8266ͨ�ţ�ʵ���������ݴ��䡣
 */

#include "wifi.h"
#include "delay.h"
#include "stdio.h"
#include "led.h"
#include "usart.h"

/**********************/
uint8_t Rcv_buf[RCVBUF_LEN]; // ���ջ�����
uint8_t Sed_buf[SEDBUF_LEN]; // ���ͻ�����
char str1[100] = {0};
char str2[100] = {0};
uint8_t SG90 = 0;
uint8_t Led = 0;
/**********************/

/**********************/
static void dma1_channel7_usart2_tx(void); // DMA1ͨ��7 USART2�����ʼ��
static void dma1_channel6_usart2_rx(void); // DMA1ͨ��6 USART2���ճ�ʼ��
/**********************/

// USART2��ʼ��
void wifi_init(void)
{
	// �ṹ�嶨��
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// GPIO�ں�USART2��ʱ��ʹ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// USART2��NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	// PA3����Ϊ���븡��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA2����Ϊ���ù����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART2����
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);

	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); // ���߿����ж�

	dma1_channel7_usart2_tx(); // ��ʼ��DMA1ͨ��7����USART2����
	dma1_channel6_usart2_rx(); // ��ʼ��DMA1ͨ��6����USART2����

	USART_Cmd(USART2, ENABLE);
}

// DMA1ͨ��7 USART2�����ʼ��
static void dma1_channel7_usart2_tx(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	// ʹ��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Sed_buf; // �ڴ��ַ�ڴ���ʱ����
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

// DMA1ͨ��6 USART2���ճ�ʼ��
static void dma1_channel6_usart2_rx(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	// ʹ��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitStructure.DMA_BufferSize = RCVBUF_LEN; // �����С�ڴ���ʱ����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Rcv_buf; // �ڴ��ַ�ڴ���ʱ����
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

// DMA1ͨ��7���䵽USART2���Ͷ�
static void dma1_ch7_to_usart2tx(uint32_t size)
{
	DMA_Cmd(DMA1_Channel7, DISABLE); // �ر�DMA�����޸�CNDTR�Ĵ���
	DMA_SetCurrDataCounter(DMA1_Channel7, size);
	DMA_Cmd(DMA1_Channel7, ENABLE);
}

// DMA1ͨ��6���䵽USART2���ն�
static void dma1_ch6_to_usart2rx(uint32_t size)
{
	DMA_Cmd(DMA1_Channel6, DISABLE); // �ر�DMA�����޸�CNDTR�Ĵ���
	DMA_SetCurrDataCounter(DMA1_Channel6, size);
	DMA_Cmd(DMA1_Channel6, ENABLE);
}

// ��������
void Send_Data(uint8_t *Pcmd)
{
	uint8_t len = strlen((const char *)Pcmd);		 // ��ȡ����
	strcpy((char *)Sed_buf, (const char *)Pcmd); // �����ݸ��Ƶ�ȫ������
	dma1_ch7_to_usart2tx(len);									 // ͨ��DMA��������
}

// ����ָ��ȴ���Ӧ
void Send_Cmd_Wait_Cmd(uint8_t *SendPcmd, uint8_t *WaitPcmd)
{
	memset(Rcv_buf, 0, RCVBUF_LEN); // ��ս��ջ�����
	Send_Data(SendPcmd);						// ����ָ�ģ��

	while (1)
	{
		if (dmaflag) // ���DMA��־�����ã���ʾ�����Ѿ�׼����
		{
			if (strstr((const char *)Rcv_buf, (const char *)WaitPcmd) != NULL)
				break; // ������ջ��������еȴ���ָ�������ѭ��
		}
	}
	dmaflag = !dmaflag; // ����DMA��־��Ϊ��һ�ν���׼��
}

__IO uint8_t dmaflag = 0; // DMA��־

// USART2�жϴ������
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
	{
		dma1_ch6_to_usart2rx(RCVBUF_LEN); // ��������DMA���ջ�����
		dmaflag = 1;
		printf(Rcv_buf);					 // ��ӡ���յ�����
		USART_ReceiveData(USART2); // ��ȡ���ݼĴ���������жϱ�־
	}
}

// ���ESPģ���Ƿ������ӵ�Wi-Fi����
int isWifiConnected()
{
	memset(Rcv_buf, 0, RCVBUF_LEN); // ��ս��ջ�����
	dmaflag = 0;										// ����DMA��־

	// ����AT�����ȡ��ǰWi-Fi����״̬
	Send_Data("AT+CWJAP?\r\n");

	while (1)
	{
		if (dmaflag) // ���DMA��־�����ã���ʾ�����Ѿ�׼����
		{
			if (strstr(Rcv_buf, "+CWJAP:") != NULL)
			{
				return 1; // �����ӵ�Wi-Fi����
			}
			if (strstr(Rcv_buf, "ERROR") != NULL)
			{
				return 0; // δ���ӵ�Wi-Fi����
			}
			dmaflag = 0; // ����DMA��־��Ϊ��һ�ν���׼��
		}
	}

	return 0;
}

// ���ESPģ���Ƿ������ӵ�TCP������
int isTcpConnected()
{
	memset(Rcv_buf, 0, RCVBUF_LEN); // ��ս��ջ�����
	dmaflag = 0;										// ����DMA��־

	// ����AT�����ȡ��ǰTCP����״̬
	Send_Data("AT+CIPSTATUS\r\n");

	while (1)
	{
		if (dmaflag) // ���DMA��־�����ã���ʾ�����Ѿ�׼����
		{
			if (strstr(Rcv_buf, "STATUS:3") != NULL)
			{
				return 1; // �ѽ���TCP����
			}
			if (strstr(Rcv_buf, "STATUS:2") != NULL)
			{
				return 0; // δ����TCP����
			}
			dmaflag = 0; // ����DMA��־��Ϊ��һ�ν���׼��
		}
	}

	return 0;
}

// ESP��ʼ������
void esp_init(void)
{
	delay_ms(3000);										 // ��ʱ�ȴ�ģ���ȶ�
	Send_Cmd_Wait_Cmd("AT\r\n", "OK"); // ����AT����ȷ��ģ������

	if (isWifiConnected())
	{
		printf("WIFI���ӳɹ�\r\n");
	}
	else
	{
		Send_Cmd_Wait_Cmd("AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n", "OK"); // ���ӵ�ָ��Wi-Fi����
		printf("WIFI���ӳɹ�\r\n");
	}

	if (isTcpConnected())
	{
		printf("TCP���ӳɹ�\r\n");
	}
	else
	{
		Send_Cmd_Wait_Cmd("AT+CIPSTART=\"TCP\",\"SERVER_IP\",SERVER_PORT\r\n", "OK"); // ���ӵ�ָ��TCP������
		printf("��������TCP������\r\n");
	}
	printf("�������");

	delay_ms(2000);
}
