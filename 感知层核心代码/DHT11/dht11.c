#include "stm32f10x.h"
#include "dht11.h"
#include "delay.h"
#include "stdio.h"

/************************/
uint8_t Temp = 0;		//温度
uint8_t Hmd = 0;		//湿度
/************************/


// THTB_Data - PA8

/**
 * 功能：  初始化温湿度计
 * 参数：  无
 * 返回值：无
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
 * 功能：  主机复位信号
 * 参数：  无
 * 返回值：无
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
 * 功能：  温湿度计应答
 * 参数：  无
 * 返回值： 应答返回 1，非应答返回 0
 **/
int dht11_ack(void)
{
	int timeout = 0;
	
	// 方向：输入
	DHT11_IN();
	// 等待低电平到来
	while (DHT11_IS_HIGH());
	// 低电平计时
	while (DHT11_IS_LOW() && (timeout < 60)) {
		timeout++;
		delay_us(1);
	}
	// 超时
	if (timeout > 50) {
		return 0;
	}
	timeout = 0;
	// 高电平计时
	while (DHT11_IS_HIGH() && (timeout < 60)) {
		timeout++;
		delay_us(1);
	}
	// 超时
	if (timeout > 50) {
		return 0;
	}
	
	// 应答OK
	return 1;
}

/**
 * 功能：  读取温湿度的一个位
 * 参数：  无
 * 返回值：0 或 1
 **/
uint8_t dht11_read_bit(void)
{
	int timeout = 0;
	
	// 等待低电平到来
	while (DHT11_IS_HIGH());
	// 低电平延时
	while (DHT11_IS_LOW() && (timeout < 15)) {
		timeout++;
		delay_us(1);
	}
	timeout = 0;
	// 等待高电平到来
	while (DHT11_IS_LOW());
	// 高电平计时
	while (DHT11_IS_HIGH() && (timeout < 30)) {
		timeout++;
		delay_us(1);
	}
	// 等待高电平结束
	while (DHT11_IS_HIGH());
	
	// 返回这个位：0/1
	return (timeout <= 28) ? 0 : 1;
}

/**
 * 功能：  读取温湿度的一个字节
 * 参数：  无
 * 返回值：无
 **/
uint8_t dht11_read_byte(void)
{
	uint8_t byte = 0, i;
	
	for (i = 0; i < 8; i++) {
		byte <<= 1;   // 高位在前
		byte |= dht11_read_bit();
	}
	
	return byte;
}

/**
 * 功能：  读取温湿度数据
 * 参数：  dht 保存温湿度的数组
 * 返回值：成功返回 0，失败返回 -1
 **/
int dht11_read_data(uint8_t *humi, uint8_t *temp)
{

	int i = 0;
	uint8_t dht[5];
		
	dht11_reset();     // 主机复位


	if (dht11_ack()) {  // 温湿度计应答
		for (i = 4; i >= 0; i--) { // 读取数据
			dht[i] = dht11_read_byte();
		}
		// 校验
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
	
	// 拉高电平为下次读取作准备
	DHT11_OUT();
	DHT11_SET_HIGH();
	
	return -1;
	
}

