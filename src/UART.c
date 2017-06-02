#include "LPC11xx.h"                    // Device header
#include <stdio.h>
#include <string.h>
#define UART_BPS 9600//串口通信波特率
        uint8_t  table[9]={0xff,0x07f,0x03f,0x1f,0x0f,0x07,0x03,0x01,0x00};
void UART_Init()
{
	uint16_t usFdiv;
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<16);//使能IOCON时钟
	LPC_IOCON->PIO1_6&=~0x07;//将P1.6先复位
	LPC_IOCON->PIO1_6|=0x01;//将P1.6设置为RXD即接收数据
	LPC_IOCON->PIO1_7&=~0x07;//将P1.7先复位
	LPC_IOCON->PIO1_7|=0x01;//将P1.7设置为TXD即发送数据
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);//关闭IOCON时钟
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);//打开UART时钟
	LPC_SYSCON->UARTCLKDIV|=0x01;//设置UART时钟分频值为1
	LPC_UART->LCR=0x83;//0X83二进制位1000 0011即设置为8位字符长度 一个停止位 无奇偶校验 且允许对除数锁存器的访问
	//LCR为UART线控制寄存器决定收发的数据字符格式
	
	usFdiv=(SystemCoreClock /LPC_SYSCON ->UARTCLKDIV/16)/UART_BPS;//计算该波特率要求的除数锁存器值
	LPC_UART->DLM=usFdiv/256;//写除数锁存器的高8位
	LPC_UART->DLL=usFdiv%256;//写除数锁存器的低8位
	//DLM与DLL均是UART的除数锁存器DLL为低8位 DLM为高8位
	LPC_UART->LCR=0x03;//锁定波特率
	LPC_UART->FCR=0x07;//使能FIFO 设置8字节触发
	
}
void UART_SendByte(uint8_t ucDAT)
{
		LPC_UART->THR=ucDAT;//发送数据
		while((LPC_UART->LSR&0x40)==0);//等待发送完，TEMT置位
} 
void UART_SendStr(char  *pucStr)
{
	
		while(1)
		{
			if(*pucStr =='\0')
				break;//遇到结束符退出
			UART_SendByte(*pucStr++);  
		}
}
uint8_t UART_GetByte(void )
{
	uint8_t  ucRcvData;
	 while((LPC_UART->LSR&0x01)==0); 
	
	
	//等待接收数据
	
		ucRcvData=LPC_UART->RBR;//读出数据
		return (ucRcvData);
	
}