#include "LPC11xx.h"   // Device header
#include "UART.h"
#include <stdio.h>
#include <string.h>
#define const unsigned long NTC_Tem_table[]={};
				char  GcRcvBuf [20];//ADC���ݻ�����
			
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
	LPC_IOCON ->PIO1_11 &=~0XBF;//����PIO1_11Ϊģ������ģʽ
	LPC_IOCON ->PIO1_11 |=0X01;//P1.11ģ������ͨ��7
	LPC_SYSCON->PDRUNCFG &=~(0X01<<4);//ADCģ���ϵ�
	LPC_SYSCON ->SYSAHBCLKCTRL |=(0X01<<13);//ʹ��ADCģ��ʱ��
	LPC_ADC ->CR = (0X01<<7)|//SEL=7��ѡ��ADC��ADC7
	               ((SystemCoreClock /1000000-1)<<8)|//ת��ʱ��1M
								 (0<<16)|//�������ת������
								 (0<<17)|//ʹ��1clocksת��
								 (0<<24)|//ADCת��ֹͣ
								 (0<<27);//ֱ������ADCת������λ��Ч

}
void ADC_read()
{
	for(i=0;i<10;i++)
	{
		LPC_ADC ->CR |=(1<<24);//����ת��
		while((LPC_ADC->DR[7]&0X80000000)==0);//��ȡDR0��Done
		ulADCBuf=LPC_ADC->DR[7];
		ulADCBuf=(ulADCBuf>>6)&0x3ff;//���� �ӵ���λ��ʼ�洢��ռ��10λ
		ulADCData+=ulADCBuf;//
	}
	  ulADCData=ulADCData/10;//����10�ν����˲�
		ulADCData=(ulADCData*1000)/(3300-ulADCData);//��R
		
		sprintf (GcRcvBuf ,"RES=%4dŷķ\r\n",ulADCData);//�����ݷ��͵����ڽ�����ʾ
		UART_SendStr(GcRcvBuf );//�����ݷ��͵�������ʾ	
		
		delay (200);
}
		