#ifndef _DHT11_H_
#define _DHT11_H_

//#include "bitband.h"
#include "sys.h"

/************************/
extern uint8_t Temp;		//�¶�
extern uint8_t Hmd;		//ʪ��

#define TEMPDIF 0		//ʵ���²�

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
 * ���ܣ�  ��ʼ����ʪ�ȼ�
 * ������  ��
 * ����ֵ����
 **/
void dht11_init(void);

/**
 * ���ܣ�  ������λ�ź�
 * ������  ��
 * ����ֵ����
 **/
void dht11_reset(void);

/**
 * ���ܣ�  ��ʪ�ȼ�Ӧ��
 * ������  ��
 * ����ֵ�� Ӧ�𷵻� 1����Ӧ�𷵻� 0
 **/
int dht11_ack(void);

/**
 * ���ܣ�  ��ȡ��ʪ�ȵ�һ��λ
 * ������  ��
 * ����ֵ��0 �� 1
 **/
uint8_t dht11_read_bit(void);

/**
 * ���ܣ�  ��ȡ��ʪ�ȵ�һ���ֽ�
 * ������  ��
 * ����ֵ����
 **/
uint8_t dht11_read_byte(void);

/**
 * ���ܣ�  ��ȡ��ʪ������
 * ������  dht ������ʪ�ȵ�����
 * ����ֵ���ɹ����� 0��ʧ�ܷ��� -1
 **/
int dht11_read_data(uint8_t *humi, uint8_t *temp);

#endif
