#include "LPC11xx.h"   // Device header
#include <stdio.h>
#include <string.h>
#define char  GcRcvBuf[20];//ADC数据缓冲区
				
	void delay(uint32_t ulTime)
{
	uint32_t i;
	i = 0;
	while(ulTime--)
	{
		for(i=0;i<5000;i++);
		
	}
	
}
void ADC_Init(void)
{
	LPC_SYSCON ->SYSAHBCLKCTRL|=(1<<16);
	LPC_IOCON ->R_PIO1_0 &=~0XBF;//配置PIO1_0为模拟输入模式
	LPC_IOCON ->R_PIO1_0 |=0X02;//P1.0模拟输入通道1
	LPC_SYSCON->PDRUNCFG &=~(0X01<<4);//ADC模块上电
	LPC_SYSCON ->SYSAHBCLKCTRL |=(0X01<<13);//使能ADC模块时钟
	LPC_ADC ->CR = (0X02<<0)|//SEL=2，选择ADC即ADC1
	               ((SystemCoreClock /1000000-1)<<8)|//转换时钟1M
								 (0<<16)|//软件控制转换操作
								 (0<<17)|//使用1clocks转换
								 (0<<24)|//ADC转换停止
								 (0<<27);//直接启动ADC转换，此位无效

}
