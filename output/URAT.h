#ifndef UART_H
#define UART_H
#include "LPC11xx.h" 
void UART_Init();
void UART_SendByte(uint8_t ucDAT);
void UART_SendStr(char  *pucStr);
#endif