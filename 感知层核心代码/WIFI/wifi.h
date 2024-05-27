#ifndef __WIFI_H_
#define __WIFI_H_

#include "stm32f10x.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "dht11.h"





#define RCVBUF_LEN	1024
#define SEDBUF_LEN	128

extern uint8_t SG90;

extern uint8_t  Rcv_buf[RCVBUF_LEN];
extern uint8_t  Sed_buf[SEDBUF_LEN];
extern __IO uint8_t dmaflag;

void wifi_init(void);
void Send_Data(uint8_t *Pcmd);								//发送自定义数据
void Send_Cmd_Wait_Cmd(uint8_t *SendPcmd,uint8_t *WaitPcmd);	//发送AT指令，等待应答
void esp_init(void);
int isWifiConnected();
#endif
