 #include "LPC11xx.h"                    // Device header
   void KEY_Init ()
	{
	LPC_GPIO2->DIR|=0xFFF;//将P2_0~P2_7设置为输出
	LPC_GPIO2->DATA|=(1<<8);//将P2_8~p2_11设置为高电平
	LPC_GPIO2->DATA|=(1<<9);
	LPC_GPIO2->DATA|=(1<<10);
	LPC_GPIO2->DATA|=(1<<11);
	LPC_GPIO3->DIR&=~(0xFF);//将控制按键的p3_0~p3_4都设置为输入
	
	LPC_GPIO3->IE|=(1<<0);
	LPC_GPIO3->IE|=(1<<1);
	LPC_GPIO3->IE|=(1<<2);
	LPC_GPIO3->IE|=(1<<3);
	LPC_GPIO3->IE|=(1<<4);//允许p3_0~p3_4上产生中断
	
	LPC_GPIO3->IS&=~(1<<0);
	LPC_GPIO3->IS&=~(1<<1);
	LPC_GPIO3->IS&=~(1<<2);
	LPC_GPIO3->IS&=~(1<<3);
	LPC_GPIO3->IS&=~(1<<4);//允许p3_0~p3_4上的中断边沿触发
	
	LPC_GPIO3->IEV&=~(1<<0);
	LPC_GPIO3->IEV&=~(1<<1);
	LPC_GPIO3->IEV&=~(1<<2);
	LPC_GPIO3->IEV&=~(1<<3);
	LPC_GPIO3->IEV&=~(1<<4);//允许p3_0~p3_4上的中断下降沿有效
	NVIC_EnableIRQ(EINT3_IRQn);
}