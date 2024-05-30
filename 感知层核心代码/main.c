/*
 * 本代码用于一个物联网项目，收集环境数据，包括温度、湿度、光照强度、CO2浓度、声音强度和风力。
 * 收集到的数据通过ESP8266 WiFi模块传输到TCP服务器。
 * 代码初始化了各种传感器和接口，从传感器读取数据，并发送数据到服务器。
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

// 定义全局变量
u8 wendu = 0, shidu = 0;
u16 guangzhao = 0, co2 = 0, shengxiang = 0, fengli = 0;
char str[100] = {0};
u16 ad = 0;

int main(void)
{
	uart_init(9600); // 初始化串口，波特率为9600
	printf("uart_init\r\n");
	delay_init(); // 初始化延时函数
	printf("delay_init\r\n");
	LED_Init(); // 初始化与LED连接的硬件接口
	printf("LED_Init\r\n");
	dht11_init(); // 初始化DHT11传感器
	printf("dht11_Init OK\r\n");
	BH1750_Config_Init();	 // 初始化BH1750传感器
	bh_data_send(BHPowOn); // 打开模块，等待测量命令
	bh_data_send(BHReset); // 重置数据寄存器值，在PowerOn模式下有效
	bh_data_send(BHModeL); // 设置为低分辨率，单位4lx，测量时间16ms
	delay_ms(180);				 // 等待测量结束，延时180ms，保证通讯
	printf("BH1750_Init OK\r\n");
	VoiceSensor_Init(); // 初始化声音传感器
	printf("voice_Init OK\r\n");
	ADC_Pin_Init(); // 初始化ADC引脚
	printf("\r\nSMOG_Init OK\r\n");

	wifi_init(); // 初始化WiFi模块
	printf("wifi_init OK\r\n");
	esp_init(); // 初始化ESP8266
	printf("esp_Init OK\r\n");

	while (1)
	{
		// 获取温度和湿度
		dht11_read_data(&shidu, &wendu);

		// 获取光照强度
		guangzhao = bh_data_read() * 4 / 1.2; // 光照强度的计算公式 = 寄存器值 * 分辨率 / 1.2

		// 获取CO2浓度
		ad = ADC_Trans();
		co2 = ad * 99 / 4096.0; // 将AD值转换成百分比，范围为0~99

		// 获取声音强度
		if (VoiceSensor_Get() == 1) // 如果PA11口为高电平，即有声音
		{
			shengxiang = 1;
		}
		else
		{
			shengxiang = 0;
		}

		LED1 = !LED1; // 闪烁LED，提示系统正在运行
		printf("wendu=%d&shidu=%d&guangzhao=%d&co2=%d&shengxiang=%d&fengli=%d&\r\n", wendu, shidu, guangzhao, co2, shengxiang, fengli);

		// 格式化字符串，用于发送数据到TCP服务器
		sprintf(str, "wendu=%d&shidu=%d&guangzhao=%d&co2=%d&shengxiang=%d&fengli=%d&\r\n", wendu, shidu, guangzhao, co2, shengxiang, fengli);
		ESP8266_SendString(DISABLE, str, sizeof(str) - 40, 5);

		delay_ms(1000); // 延时1秒
	}
}
