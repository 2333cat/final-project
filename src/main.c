#include"ADC.h"
#include"UART.h"
#include"LCD1602.h"


int main()
{	
	UART_Init ();
  ADC_Init ();
	while(1)
	{
	 ADC_read();
	}
} 