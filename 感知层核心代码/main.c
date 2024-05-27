#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dht11.h"
#include "bh1750.h"
#include "voice.h"
#include "wifi.h"
#include "adc.h"
u8 wendu = 0,shidu  = 0;
u16 guangzhao = 0,co2 = 0 ,shengxiang =0,fengli=0;
char str[100]={0};
u16 ad = 0;
int main(void)
{	
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	printf("uart_init\r\n");
	delay_init();	    	 //��ʱ������ʼ��	  
	printf("delay_init\r\n");
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	printf("LED_Init\r\n");
	dht11_init();
	printf("dht11_Init OK\r\n");
	BH1750_Config_Init();     //BH1750�ĳ�ʼ��
	bh_data_send(BHPowOn);   //��ģ��ȴ���������
	bh_data_send(BHReset);     //�������ݼĴ���ֵ��PowerOnģʽ����Ч
	bh_data_send(BHModeL);      //�ͷֱ��� ��λ4lx  ����ʱ��16ms 
	delay_ms(180);             //�ȴ�����������ʱ180ms����֤ͨѶ 
	printf("BH1750_Init OK\r\n");
	VoiceSensor_Init();
	printf("voice_Init OK\r\n");
	ADC_Pin_Init();
	printf("\r\nSMOG_Init OK\r\n");
	
	wifi_init();
	printf("wifi_init OK\r\n");
	esp_init();
	printf("esp_Init OK\r\n");

	
	while(1)
	{
	//��ȡ�¶�\ʪ��
	dht11_read_data(&shidu,&wendu);
	
	//��ȡ����
	guangzhao = bh_data_read()*4/1.2;  //����ǿ�ȵļ��㹫ʽ = �Ĵ���ֵ*�ֱ���/1.2 
		
	//��ȡco2
	ad = ADC_Trans();
	co2 = ad * 99 / 4096.0;//ΪʲôҪ��99  ��ADֵת���ɰٷֱ�0~99
	//printf("��ѹֵ��%f\r\n",3.3/4095*ad);	
		
	//��ȡ����
	if (VoiceSensor_Get() == 1)     //���PA11��Ϊ�ߵ�ƽ����������������
	{
	shengxiang=1;			
	}else
	{
	shengxiang=0;
	}
	
	LED1=!LED1;//��˸LED,��ʾϵͳ��������.
	printf ("wendu=%d&shidu=%d&guangzhao=%d&co2=%d&shengxiang=%d&fengli=%d&\r\n",wendu,shidu,guangzhao,co2,shengxiang,fengli);
	sprintf (str,"wendu=%d&shidu=%d&guangzhao=%d&co2=%d&shengxiang=%d&fengli=%d&\r\n",wendu,shidu,guangzhao,co2,shengxiang,fengli);//��ʽ�������ַ�����TCP������
	ESP8266_SendString ( DISABLE, str, sizeof(str)-40, 5 );
		
	delay_ms(1000);   
	}
   

}

