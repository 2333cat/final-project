#include "LPC11xx.h"  // LPC11xx外设寄存器
#include "string.h"

#define LCD_RS_LOW 		LPC_GPIO2->DATA &= ~(1<<0)	//命令状态引脚
#define LCD_RS_HIGH		LPC_GPIO2->DATA |=  (1<<0)	//
#define LCD_RW_LOW		LPC_GPIO2->DATA &= ~(1<<1)	//读写使能引脚
#define LCD_RW_HIGH		LPC_GPIO2->DATA |=  (1<<1)	//
#define LCD_E_LOW		LPC_GPIO2->DATA &= ~(1<<2)	//使能端
#define LCD_E_HIGH		LPC_GPIO2->DATA |=  (1<<2)	//

#define LCD_DATAPORT	LPC_GPIO2->DATA	            //DB0～DB7数据端口
#define LCD_DATA_IN		LPC_GPIO2->DIR &= ~0xFFF	//设置端口为输入
#define LCD_DATA_OUT	LPC_GPIO2->DIR |=  0xFFF	//设置端口为输出

static volatile unsigned int ticks = 0;
/********************************************************************************
* FunctionName   : Delayms(uint16_t ms)
* Description    : 利用系统定时器产生毫秒级延时
* EntryParameter : ms:为延时参数
* ReturnValue    : None
********************************************************************************/
void Delayms(uint16_t ms)					
{
	SysTick->CTRL &= ~(1<<2);
	SysTick->LOAD = 25000*ms-1;			
	SysTick->VAL = 0;						
	SysTick->CTRL = ((1<<1)|(1<<0));       
	while(!ticks);						    
	ticks = 0;							    
	SysTick->CTRL = 0;	
}
/********************************************************************************
* FunctionName   : SysTick_Handler()
* Description    : 系统节拍定时器中断服务函数
* EntryParameter : None
* ReturnValue    : None
********************************************************************************/
void SysTick_Handler(void)
{
	ticks++;
}
/********************************************************************************
* FunctionName   : Busy_Check()
* Description    : 判断LCD模块是否忙，本函数没有取第8位，需要调用者判断
* EntryParameter : None
* ReturnValue    : 从LCD数据端口读取的数据
********************************************************************************/
uint8_t Busy_Check()
{
 	uint8_t LCD_Status;
	LCD_RS_LOW;
	LCD_RW_HIGH;
	LCD_E_HIGH;	//以上为产生读控制信号
	Delayms(1);
	
//	LCD_DATA_IN;	//将端口设置为输入
    LPC_GPIO2->DIR &= ~(0x7F8);
    
    LCD_Status = (LPC_GPIO2->DATA & 0x7F8) >> 3;	//读取端口数据
//	LCD_DATA_OUT;	//将端口设置为输出
	LPC_GPIO2->DIR |= 0x7F8;
    
	LCD_E_LOW;	
	return LCD_Status;
}
/********************************************************************************
* FunctionName   : LCD_WriteCommand()
* Description    : 写命令函数
* EntryParameter : cmd:命令
* ReturnValue    : None
********************************************************************************/
void LCD_WriteCommand(uint32_t cmd)
{
 	while((Busy_Check()	&	0x80)	==	0x80);	//判断LCD模块是否忙
	LCD_RS_LOW;
	LCD_RW_LOW;
	LCD_E_LOW;	//以上为产生写控制信号
    
	LPC_GPIO2->DATA = ((cmd << 3) & 0x7F8) | 0x00;	//将命令写入
	Delayms(1);	
	LCD_E_HIGH;
	Delayms(1);
	LCD_E_LOW;
}
/********************************************************************************
* FunctionName   : LCD_WriteData()
* Description    : 写显示数据函数
* EntryParameter : dat:数据
* ReturnValue    : None
********************************************************************************/
void LCD_WriteData(uint32_t dat)
{
 	while((Busy_Check() & 0x80) == 0x80);	//判断LCD模块是否忙
	LCD_RS_HIGH;
	LCD_RW_LOW;
	LCD_E_LOW;	//以上为产生写控制信号
	LPC_GPIO2->DATA = ((dat << 3) & 0x7F8) | 0x01;	//将数据写入
	Delayms(1);	
	LCD_E_HIGH;
	Delayms(1);
	LCD_E_LOW;
}
/********************************************************************************
* FunctionName   : LCD_DisplayChar()
* Description    : 在指定位置显示字符，y值只能取0，1。x值0=<x<=16。
* EntryParameter : x:x坐标(0~16), y: y坐标(0~1)，ch:为字符
* ReturnValue    : None
********************************************************************************/
uint8_t LCD_DisplayChar(uint8_t x, uint8_t y, uint8_t ch)
{
	if ((y > 2) || (x > 16)) // x,y坐标溢出
	{
	    return 1;
	}
	x |= ((y == 1) ? 0xC0 : 0x80); // 显示坐标
	LCD_WriteCommand(x); // 这里不检测忙信号，发送地址码
	LCD_WriteData(ch);
	return 0;
}
/********************************************************************************
* FunctionName   : LCD_DisplayStr()
* Description    : 在制定位置显示字符串。
* EntryParameter : x - x坐标(0~15), y - y坐标(0~1), *pStr - 显示字符串
* ReturnValue    : 正确返回0，错误返回1
********************************************************************************/
uint8_t LCD_DisplayStr(uint8_t x, uint8_t y, uint8_t *pStr)
{
  uint8_t i;
	uint8_t lineAddr[] = {0x80, 0xC0};

	if ((y >= 2) || (x >= 16))	// x, y坐标溢出
	{
	    return 1;
	}
	LCD_WriteCommand(lineAddr[y] + x);  // 写入地址
	for (i=x; ((i<16) && (*pStr!='\0')); i++)
	{
 		LCD_WriteData(*(pStr++)); // 写入数据
	}
	if (*pStr != '\0') // 判断数据是否写完
	{
 		x = 0; // 没写完写入第2行
		if (++y < 2)
		{
		    LCD_DisplayStr(x, y, pStr); // 下一行继续显示(递归调用)
		} 
	}
	return 0;
}
/********************************************************************************
* FunctionName   : LCDIOInit()
* Description    : 初始化LCD用到的端口,
                   DB0~DB7用到P2.0～P2.7，RS、RW、E用到P3.0~P3.3
* EntryParameter : None
* ReturnValue    : None
********************************************************************************/
void LCD_IOInit(void)
{
	LPC_GPIO2->DIR |= 0xFFF;	//设置P2_0~P2_7为输出
//	LPC_GPIO3->DIR |= 0x0F;	//设置P3_0~P3_3为输出
    LPC_GPIO0->DIR |= 0x02;
    LPC_GPIO0->DATA |= (0x02);    
}
/********************************************************************************
* FunctionName   : InitLCD()
* Description    : 初始化LCD端口并设置其显示方式
* EntryParameter : None
* ReturnValue    : None
********************************************************************************/
void InitLCD()
{
	LCD_IOInit();	//初始化LCD用到的GPIO
 	LCD_WriteCommand(0x38);	//设置为8位总线、2行显示、5×7点阵字体
	Delayms(1);	
	LCD_WriteCommand(0x01);	//清屏，归HOME位
	Delayms(1);	
	LCD_WriteCommand(0x06);	//数据输入为增量方式，显示内容不移动
	Delayms(1);	
	LCD_WriteCommand(0x0c);	//开显示，关闭光标，光标不闪烁
	Delayms(1);	
}
/********************************************************************************
* FunctionName   : main()
* Description    : 主函数，在第一行起始为止开始显示字符串，占满后另起一行
* EntryParameter : None
* ReturnValue    : None
********************************************************************************/
int main()
{
	InitLCD();
	while(1)
	{
		LCD_DisplayStr(0,0,"Hello World!Hello Kitty!");
	}