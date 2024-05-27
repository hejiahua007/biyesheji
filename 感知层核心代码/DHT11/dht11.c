#include "stm32f10x.h"
#include "dht11.h"
#include "delay.h"
#include "stdio.h"

/************************/
uint8_t Temp = 0;		//�¶�
uint8_t Hmd = 0;		//ʪ��
/************************/


// THTB_Data - PA8

/**
 * ���ܣ�  ��ʼ����ʪ�ȼ�
 * ������  ��
 * ����ֵ����
 **/
void dht11_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	DHT11_SET_HIGH();
}

/**
 * ���ܣ�  ������λ�ź�
 * ������  ��
 * ����ֵ����
 **/
void dht11_reset(void)
{
	DHT11_OUT();
	DHT11_SET_LOW();
	delay_ms(18);
	DHT11_SET_HIGH();
	delay_us(40);
}

/**
 * ���ܣ�  ��ʪ�ȼ�Ӧ��
 * ������  ��
 * ����ֵ�� Ӧ�𷵻� 1����Ӧ�𷵻� 0
 **/
int dht11_ack(void)
{
	int timeout = 0;
	
	// ��������
	DHT11_IN();
	// �ȴ��͵�ƽ����
	while (DHT11_IS_HIGH());
	// �͵�ƽ��ʱ
	while (DHT11_IS_LOW() && (timeout < 60)) {
		timeout++;
		delay_us(1);
	}
	// ��ʱ
	if (timeout > 50) {
		return 0;
	}
	timeout = 0;
	// �ߵ�ƽ��ʱ
	while (DHT11_IS_HIGH() && (timeout < 60)) {
		timeout++;
		delay_us(1);
	}
	// ��ʱ
	if (timeout > 50) {
		return 0;
	}
	
	// Ӧ��OK
	return 1;
}

/**
 * ���ܣ�  ��ȡ��ʪ�ȵ�һ��λ
 * ������  ��
 * ����ֵ��0 �� 1
 **/
uint8_t dht11_read_bit(void)
{
	int timeout = 0;
	
	// �ȴ��͵�ƽ����
	while (DHT11_IS_HIGH());
	// �͵�ƽ��ʱ
	while (DHT11_IS_LOW() && (timeout < 15)) {
		timeout++;
		delay_us(1);
	}
	timeout = 0;
	// �ȴ��ߵ�ƽ����
	while (DHT11_IS_LOW());
	// �ߵ�ƽ��ʱ
	while (DHT11_IS_HIGH() && (timeout < 30)) {
		timeout++;
		delay_us(1);
	}
	// �ȴ��ߵ�ƽ����
	while (DHT11_IS_HIGH());
	
	// �������λ��0/1
	return (timeout <= 28) ? 0 : 1;
}

/**
 * ���ܣ�  ��ȡ��ʪ�ȵ�һ���ֽ�
 * ������  ��
 * ����ֵ����
 **/
uint8_t dht11_read_byte(void)
{
	uint8_t byte = 0, i;
	
	for (i = 0; i < 8; i++) {
		byte <<= 1;   // ��λ��ǰ
		byte |= dht11_read_bit();
	}
	
	return byte;
}

/**
 * ���ܣ�  ��ȡ��ʪ������
 * ������  dht ������ʪ�ȵ�����
 * ����ֵ���ɹ����� 0��ʧ�ܷ��� -1
 **/
int dht11_read_data(uint8_t *humi, uint8_t *temp)
{

	int i = 0;
	uint8_t dht[5];
		
	dht11_reset();     // ������λ


	if (dht11_ack()) {  // ��ʪ�ȼ�Ӧ��
		for (i = 4; i >= 0; i--) { // ��ȡ����
			dht[i] = dht11_read_byte();
		}
		// У��
		if (dht[0] == dht[1] + dht[2] + dht[3] + dht[4]) {
			
			Hmd = dht[4];
			
			Temp = dht[2] - TEMPDIF;
			
			*humi = Hmd;
			*temp = Temp;
			
			return 0;
		}
		//printf("Check error\n");
	}
	else {
		//printf("NACK\n");
	}
	
	// ���ߵ�ƽΪ�´ζ�ȡ��׼��
	DHT11_OUT();
	DHT11_SET_HIGH();
	
	return -1;
	
}

