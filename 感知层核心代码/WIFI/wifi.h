#ifndef __WIFI_H_
#define __WIFI_H_

#include "stm32f10x.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "dht11.h"

// 定义接收和发送缓冲区的长度
#define RCVBUF_LEN 1024
#define SEDBUF_LEN 128

// 声明外部变量和标志
extern uint8_t Rcv_buf[RCVBUF_LEN]; // 接收缓冲区
extern uint8_t Sed_buf[SEDBUF_LEN]; // 发送缓冲区
extern __IO uint8_t dmaflag;        // DMA传输完成标志

// 函数声明
void wifi_init(void);                                         // 初始化Wi-Fi模块
void Send_Data(uint8_t *Pcmd);                                // 发送数据函数
void Send_Cmd_Wait_Cmd(uint8_t *SendPcmd, uint8_t *WaitPcmd); // 发送AT指令并等待响应
void esp_init(void);                                          // 初始化ESP模块
int isWifiConnected();                                        // 检查Wi-Fi连接状态

#endif // __WIFI_H_
