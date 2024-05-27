#ifndef __ESP8266_H
#define __ESP8266_H 			   
#include "stm32f10x.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

//ESP8266ģʽѡ��
typedef enum
{
    STA,
    AP,
    STA_AP  
}ENUM_Net_ModeTypeDef;

//���紫���Э�飬ö������
typedef enum{
     enumTCP,
     enumUDP,
} ENUM_NetPro_TypeDef;
//���Ӻţ�ָ��Ϊ�����Ӻſ��Է�ֹ�������������ͬһ�˿ڶ���������
typedef enum{
    Multiple_ID_0 = 0,
    Multiple_ID_1 = 1,
    Multiple_ID_2 = 2,
    Multiple_ID_3 = 3,
    Multiple_ID_4 = 4,
    Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;

#define ESP8266_RST_Pin          GPIO_Pin_4    //��λ�ܽ�
#define ESP8266_RST_Pin_Port     GPIOA    //��λ 
#define ESP8266_RST_Pin_Periph_Clock  RCC_APB2Periph_GPIOA       //��λʱ��

#define ESP8266_CH_PD_Pin     GPIO_Pin_5   //ʹ�ܹܽ�
#define ESP8266_CH_PD_Pin_Port     GPIOA   //ʹ�ܶ˿�
#define ESP8266_CH_PD_Pin_Periph_Clock  RCC_APB2Periph_GPIOA                     //ʹ��ʱ��


#define ESP8266_RST_Pin_SetH     GPIO_SetBits(ESP8266_RST_Pin_Port,ESP8266_RST_Pin)
#define ESP8266_RST_Pin_SetL     GPIO_ResetBits(ESP8266_RST_Pin_Port,ESP8266_RST_Pin)


#define ESP8266_CH_PD_Pin_SetH     GPIO_SetBits(ESP8266_CH_PD_Pin_Port,ESP8266_CH_PD_Pin)
#define ESP8266_CH_PD_Pin_SetL     GPIO_ResetBits(ESP8266_CH_PD_Pin_Port,ESP8266_CH_PD_Pin)


#define ESP8266_USART(fmt, ...)  USART_printf (USART2, fmt, ##__VA_ARGS__)    
#define PC_USART(fmt, ...)       printf(fmt, ##__VA_ARGS__)       //���Ǵ��ڴ�ӡ����������1��ִ��printf����Զ�ִ��fput�������ض�����printf��



#define User_ESP8266_SSID     "***"          //wifi��
#define User_ESP8266_PWD      "***"      //wifi����


//Wifi ���ӷ����������Ժ�ESP8266 ���ӵ���ͬһ��·����
#define User_ESP8266_TCPServer_IP     "192.168.43.94"     //������IP
#define User_ESP8266_TCPServer_PORT   "8888"      				//�������˿ں�


extern volatile uint8_t TcpClosedFlag;  //����״̬��־

void ESP8266_STA_TCPClient_Test(void);

#define RX_BUF_MAX_LEN 1024       //����ֽ���
extern struct STRUCT_USART_Fram   //����֡�ṹ��
{
    char Data_RX_BUF[RX_BUF_MAX_LEN];
    union 
    {
        __IO u16 InfAll;
        struct 
        {
            __IO u16 FramLength       :15;                               // 14:0 
            __IO u16 FramFinishFlag   :1;                                // 15 
        }InfBit;
    }; 
	
}ESP8266_Fram_Record_Struct;


//��ʼ����TCP���ܺ���
void ESP8266_Init(u32 bound);
void ESP8266_AT_Test(void);
bool ESP8266_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,u32 time);
void ESP8266_Rst(void);
bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode);
bool ESP8266_JoinAP( char * pSSID, char * pPassWord );
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx );
bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
bool ESP8266_UnvarnishSend ( void );
void ESP8266_ExitUnvarnishSend ( void );
u8 ESP8266_Get_LinkStatus ( void );
void USART_printf( USART_TypeDef * USARTx, char * Data, ... );

void ESP8266_STA_TCPClient_Test(void);

//MQTT���ܺ���
//bool ESP8266_MQTTUSERCFG( char * pClient_Id, char * pUserName,char * PassWord);
//bool ESP8266_MQTTCONN( char * Ip, int Num);
//bool ESP8266_MQTTSUB(char * Topic);
//bool ESP8266_MQTTPUB( char * Topic,char *temp);
//bool ESP8266_MQTTCLEAN(void);
//bool MQTT_SendString(char * pTopic,char *temp2);

#endif
