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
	// �ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	// ����ͨ�ţ��첽ͨ�ţ�ȫ˫����
	USART_InitTypeDef USART_InitStructure;
	// Ƕ�������жϿ�����
	NVIC_InitTypeDef NVIC_InitStructure;
	// GPIO�ں�USART2��ʱ�ӵ�ʹ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// USART2 �� NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;					//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//
	NVIC_Init(&NVIC_InitStructure);

	// PA3 ����Ϊ���븡��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA2 ����Ϊ���ù����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART2 ����
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);

	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
	// USART_IT_IDLE����USART���յ�һ֡���ݺ��ڽ�����Ϊ���֮ǰ���������жϡ�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); // ���߿����ж�

	dma1_channel7_usart2_tx();
	dma1_channel6_usart2_rx();

	USART_Cmd(USART2, ENABLE);
}
// ֱ�ӼĴ���
static void dma1_channel7_usart2_tx(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	//	NVIC_InitTypeDef NVIC_InitStructure;

	// ʹ��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Sed_buf; // �ڴ��ַ�ݲ����ã��ȵ�����ʱ������
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

	// ʹ��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//	// DMA1 ͨ��4 �ж�����

	DMA_InitStructure.DMA_BufferSize = RCVBUF_LEN; // �����С�ݲ����ã��ȵ�Ҫ����ʱ������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Rcv_buf; // �ڴ��ַ�ݲ����ã��ȵ�����ʱ������
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

// DMA1 ͨ��7 ���䵽 USART2 ���Ͷ�
static void dma1_ch7_to_usart2tx(uint32_t size)
{

	DMA_Cmd(DMA1_Channel7, DISABLE); // �ر� DMA �����޸� CNDTR �Ĵ���
	DMA_SetCurrDataCounter(DMA1_Channel7, size);
	DMA_Cmd(DMA1_Channel7, ENABLE);
}

// DMA1 ͨ��7 ���䵽 USART2 ���Ͷ�
static void dma1_ch6_to_usart2rx(uint32_t size)
{

	DMA_Cmd(DMA1_Channel6, DISABLE); // �ر� DMA �����޸� CNDTR �Ĵ���
	DMA_SetCurrDataCounter(DMA1_Channel6, size);
	DMA_Cmd(DMA1_Channel6, ENABLE);
}

void Send_Data(uint8_t *Pcmd)
{
	// ��ȡ����
	uint8_t len = strlen((const char *)Pcmd);
	// �����ݸ��Ƶ�ȫ������
	strcpy((char *)Sed_buf, (const char *)Pcmd);
	dma1_ch7_to_usart2tx(len);
}

void Send_Cmd_Wait_Cmd(uint8_t *SendPcmd, uint8_t *WaitPcmd)
{
	memset(Rcv_buf, 0, RCVBUF_LEN); // ��ս��ջ�����
	Send_Data(SendPcmd);						// ����ָ�ģ��

	while (1)
	{ // ��ʼ����ѭ���ȴ���Ӧ

		if (dmaflag)
		{ // ���DMA��־�����ã���ʾ�����Ѿ�׼����
			if (strstr((const char *)Rcv_buf, (const char *)WaitPcmd) != NULL)
				break; // ������ջ��������еȴ���ָ�������ѭ��
		}
	}
	dmaflag = !dmaflag; // ����DMA��־��Ϊ��һ�ν���׼��
}

__IO uint8_t dmaflag = 0;

// ���Ӳ���
//  USART2 �жϴ������
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
	{ // ������
		dma1_ch6_to_usart2rx(RCVBUF_LEN);
		dmaflag = 1;
		printf(Rcv_buf);
		USART_ReceiveData(USART2); // ���߿���ʱҲҪ��ȡ���ݼĴ���
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
	{ // �ȴ����յ���Ӧ
		if (dmaflag)
		{ // ���DMA��־�����ã���ʾ�����Ѿ�׼����
			// �����Ӧ���Ƿ���"+CWJAP:"����ʾ�����ӵ�Wi-Fi����
			if (strstr(Rcv_buf, "+CWJAP:") != NULL)
			{
				return 1;
			}
			if (strstr(Rcv_buf, "ERROR") != NULL)
			{
				return 0;
			}
			dmaflag = 0; // ����DMA��־��Ϊ��һ�ν���׼��
		}
		// �ڴ˿�����ӳ�ʱ�жϣ���������ѭ���ȴ���Ӧ
	}

	return 0;
}
// ���ESPģ���Ƿ������ӵ�Wi-Fi����
int isTcpConnected()
{
	memset(Rcv_buf, 0, RCVBUF_LEN); // ��ս��ջ�����
	dmaflag = 0;										// ����DMA��־

	// ����AT�����ȡ��ǰTCP����״̬
	Send_Data("AT+CIPSTATUS\r\n");

	while (1)
	{ // �ȴ����յ���Ӧ
		if (dmaflag)
		{ // ���DMA��־�����ã���ʾ�����Ѿ�׼����
			// �����Ӧ���Ƿ���"STATUS:3"����ʾ�ѽ���TCP����
			if (strstr(Rcv_buf, "STATUS:3") != NULL)
			{
				return 1;
			}
			// �����Ӧ���Ƿ���"STATUS:"�������� AP����� IP ��ַ
			if (strstr(Rcv_buf, "STATUS:2") != NULL)
			{
				return 0;
			}
			dmaflag = 0; // ����DMA��־��Ϊ��һ�ν���׼��
		}
		// �ڴ˿�����ӳ�ʱ�жϣ���������ѭ���ȴ���Ӧ
	}

	return 0;
}

void esp_init(void)
{

	//	Send_Data("+++\r\n");
	//	// ���� AT ����
	//	Send_Cmd_Wait_Cmd("AT\r\n","OK");
	//    //
	//	Send_Cmd_Wait_Cmd("ATE0\r\n","OK");
	//    //����ģ��
	//	Send_Cmd_Wait_Cmd("AT+RST\r\n","ready");
	//    //����Wi-Fiģʽ�����浽flash
	//  Send_Cmd_Wait_Cmd("AT+CWMODE_DEF=1\r\n","OK");

	// ����ap
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	Send_Cmd_Wait_Cmd("AT\r\n", "OK");
	if (isWifiConnected())
	{
		printf("WIFI���ӳɹ�\r\n");
	}
	else
	{
		Send_Cmd_Wait_Cmd("AT+CWJAP=\"***\",\"***\"\r\n", "OK");
		printf("WIFI���ӳɹ�\r\n");
	}

	// ���TCP����״̬
	if (isTcpConnected())
	{
		printf("TCP���ӳɹ�\r\n");
	}
	else
	{
		// ���Խ���TCP����
		Send_Cmd_Wait_Cmd("AT+CIPSTART=\"TCP\",\"***\",***\r\n", "OK");
		printf("��������TCP������\r\n");
	}
	printf("�������");

	delay_ms(2000);
}
