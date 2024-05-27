#ifndef _DHT11_H_
#define _DHT11_H_

//#include "bitband.h"
#include "sys.h"

/************************/
extern uint8_t Temp;		//温度
extern uint8_t Hmd;		//湿度

#define TEMPDIF 0		//实际温差

/************************/

#if 1

#define DHT11_SET_HIGH()   do { PAout(8) = 1; } while (0)
#define DHT11_SET_LOW()    do { PAout(8) = 0; } while (0)

#define DHT11_IS_LOW()     !PAin(8)
#define DHT11_IS_HIGH()    PAin(8)

#else

#define DHT11_SET_HIGH()   do { GPIO_SetBits(GPIOA, GPIO_Pin_8); } while (0)
#define DHT11_SET_LOW()    do { GPIO_ResetBits(GPIOA, GPIO_Pin_8); } while (0)

#define DHT11_IS_LOW()     (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))
#define DHT11_IS_HIGH()    GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)

#endif

#define DHT11_IN()         do { GPIOA->CRH &= ~0xF; GPIOA->CRH |= 0x4; } while (0)
#define DHT11_OUT()        do { GPIOA->CRH &= ~0xF; GPIOA->CRH |= 0x3; } while (0)

/**
 * 功能：  初始化温湿度计
 * 参数：  无
 * 返回值：无
 **/
void dht11_init(void);

/**
 * 功能：  主机复位信号
 * 参数：  无
 * 返回值：无
 **/
void dht11_reset(void);

/**
 * 功能：  温湿度计应答
 * 参数：  无
 * 返回值： 应答返回 1，非应答返回 0
 **/
int dht11_ack(void);

/**
 * 功能：  读取温湿度的一个位
 * 参数：  无
 * 返回值：0 或 1
 **/
uint8_t dht11_read_bit(void);

/**
 * 功能：  读取温湿度的一个字节
 * 参数：  无
 * 返回值：无
 **/
uint8_t dht11_read_byte(void);

/**
 * 功能：  读取温湿度数据
 * 参数：  dht 保存温湿度的数组
 * 返回值：成功返回 0，失败返回 -1
 **/
int dht11_read_data(uint8_t *humi, uint8_t *temp);

#endif
