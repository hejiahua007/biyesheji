/*
 * ����������һ����������Ŀ���ռ��������ݣ������¶ȡ�ʪ�ȡ�����ǿ�ȡ�CO2Ũ�ȡ�����ǿ�Ⱥͷ�����
 * �ռ���������ͨ��ESP8266 WiFiģ�鴫�䵽TCP��������
 * �����ʼ���˸��ִ������ͽӿڣ��Ӵ�������ȡ���ݣ����������ݵ���������
 */

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dht11.h"
#include "bh1750.h"
#include "voice.h"
#include "wifi.h"
#include "adc.h"

// ����ȫ�ֱ���
u8 wendu = 0, shidu = 0;
u16 guangzhao = 0, co2 = 0, shengxiang = 0, fengli = 0;
char str[100] = {0};
u16 ad = 0;

int main(void)
{
	uart_init(9600); // ��ʼ�����ڣ�������Ϊ9600
	printf("uart_init\r\n");
	delay_init(); // ��ʼ����ʱ����
	printf("delay_init\r\n");
	LED_Init(); // ��ʼ����LED���ӵ�Ӳ���ӿ�
	printf("LED_Init\r\n");
	dht11_init(); // ��ʼ��DHT11������
	printf("dht11_Init OK\r\n");
	BH1750_Config_Init();	 // ��ʼ��BH1750������
	bh_data_send(BHPowOn); // ��ģ�飬�ȴ���������
	bh_data_send(BHReset); // �������ݼĴ���ֵ����PowerOnģʽ����Ч
	bh_data_send(BHModeL); // ����Ϊ�ͷֱ��ʣ���λ4lx������ʱ��16ms
	delay_ms(180);				 // �ȴ�������������ʱ180ms����֤ͨѶ
	printf("BH1750_Init OK\r\n");
	VoiceSensor_Init(); // ��ʼ������������
	printf("voice_Init OK\r\n");
	ADC_Pin_Init(); // ��ʼ��ADC����
	printf("\r\nSMOG_Init OK\r\n");

	wifi_init(); // ��ʼ��WiFiģ��
	printf("wifi_init OK\r\n");
	esp_init(); // ��ʼ��ESP8266
	printf("esp_Init OK\r\n");

	while (1)
	{
		// ��ȡ�¶Ⱥ�ʪ��
		dht11_read_data(&shidu, &wendu);

		// ��ȡ����ǿ��
		guangzhao = bh_data_read() * 4 / 1.2; // ����ǿ�ȵļ��㹫ʽ = �Ĵ���ֵ * �ֱ��� / 1.2

		// ��ȡCO2Ũ��
		ad = ADC_Trans();
		co2 = ad * 99 / 4096.0; // ��ADֵת���ɰٷֱȣ���ΧΪ0~99

		// ��ȡ����ǿ��
		if (VoiceSensor_Get() == 1) // ���PA11��Ϊ�ߵ�ƽ����������
		{
			shengxiang = 1;
		}
		else
		{
			shengxiang = 0;
		}

		LED1 = !LED1; // ��˸LED����ʾϵͳ��������
		printf("wendu=%d&shidu=%d&guangzhao=%d&co2=%d&shengxiang=%d&fengli=%d&\r\n", wendu, shidu, guangzhao, co2, shengxiang, fengli);

		// ��ʽ���ַ��������ڷ������ݵ�TCP������
		sprintf(str, "wendu=%d&shidu=%d&guangzhao=%d&co2=%d&shengxiang=%d&fengli=%d&\r\n", wendu, shidu, guangzhao, co2, shengxiang, fengli);
		ESP8266_SendString(DISABLE, str, sizeof(str) - 40, 5);

		delay_ms(1000); // ��ʱ1��
	}
}
