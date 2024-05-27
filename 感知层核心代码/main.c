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
	uart_init(9600);	 //串口初始化为9600
	printf("uart_init\r\n");
	delay_init();	    	 //延时函数初始化	  
	printf("delay_init\r\n");
	LED_Init();		  	//初始化与LED连接的硬件接口
	printf("LED_Init\r\n");
	dht11_init();
	printf("dht11_Init OK\r\n");
	BH1750_Config_Init();     //BH1750的初始化
	bh_data_send(BHPowOn);   //打开模块等待测量命令
	bh_data_send(BHReset);     //重置数据寄存器值在PowerOn模式下有效
	bh_data_send(BHModeL);      //低分辨率 单位4lx  测量时间16ms 
	delay_ms(180);             //等待测量结束延时180ms，保证通讯 
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
	//获取温度\湿度
	dht11_read_data(&shidu,&wendu);
	
	//获取光照
	guangzhao = bh_data_read()*4/1.2;  //光照强度的计算公式 = 寄存器值*分辨率/1.2 
		
	//获取co2
	ad = ADC_Trans();
	co2 = ad * 99 / 4096.0;//为什么要乘99  把AD值转换成百分比0~99
	//printf("电压值：%f\r\n",3.3/4095*ad);	
		
	//获取声音
	if (VoiceSensor_Get() == 1)     //如果PA11口为高电平，即有声音，灯亮
	{
	shengxiang=1;			
	}else
	{
	shengxiang=0;
	}
	
	LED1=!LED1;//闪烁LED,提示系统正在运行.
	printf ("wendu=%d&shidu=%d&guangzhao=%d&co2=%d&shengxiang=%d&fengli=%d&\r\n",wendu,shidu,guangzhao,co2,shengxiang,fengli);
	sprintf (str,"wendu=%d&shidu=%d&guangzhao=%d&co2=%d&shengxiang=%d&fengli=%d&\r\n",wendu,shidu,guangzhao,co2,shengxiang,fengli);//格式化发送字符串到TCP服务器
	ESP8266_SendString ( DISABLE, str, sizeof(str)-40, 5 );
		
	delay_ms(1000);   
	}
   

}

