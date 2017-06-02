#include "LPC11xx.h"   // Device header
#include "UART.h"
#include <stdio.h>
#include <string.h>
#define const unsigned long NTC_Tem_table[]={};
				char  GcRcvBuf [20];//ADC数据缓冲区
			
				uint32_t ulADCBuf;
			  uint32_t ulADCData;
				uint32_t i;
				
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
	LPC_IOCON ->PIO1_11 &=~0XBF;//配置PIO1_11为模拟输入模式
	LPC_IOCON ->PIO1_11 |=0X01;//P1.11模拟输入通道7
	LPC_SYSCON->PDRUNCFG &=~(0X01<<4);//ADC模块上电
	LPC_SYSCON ->SYSAHBCLKCTRL |=(0X01<<13);//使能ADC模块时钟
	LPC_ADC ->CR = (0X01<<7)|//SEL=7，选择ADC即ADC7
	               ((SystemCoreClock /1000000-1)<<8)|//转换时钟1M
								 (0<<16)|//软件控制转换操作
								 (0<<17)|//使用1clocks转换
								 (0<<24)|//ADC转换停止
								 (0<<27);//直接启动ADC转换，此位无效

}
void ADC_read()
{
	for(i=0;i<10;i++)
	{
		LPC_ADC ->CR |=(1<<24);//立即转换
		while((LPC_ADC->DR[7]&0X80000000)==0);//读取DR0的Done
		ulADCBuf=LPC_ADC->DR[7];
		ulADCBuf=(ulADCBuf>>6)&0x3ff;//数据 从第六位开始存储，占据10位
		ulADCData+=ulADCBuf;//
	}
	  ulADCData=ulADCData/10;//采样10次进行滤波
		ulADCData=(ulADCData*1000)/(3300-ulADCData);//求R
		
		sprintf (GcRcvBuf ,"RES=%4d欧姆\r\n",ulADCData);//将数据发送到串口进行显示
		UART_SendStr(GcRcvBuf );//将数据发送到串口显示	
		
		delay (200);
}
		