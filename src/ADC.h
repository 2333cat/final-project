#ifndef ADC_H
#define ADC_H
#include "LPC11xx.h"
void ADC_Init(void);
void delay(uint32_t ulTime);
void ADC_read(void );
float   CaculTwoPoint(float   x1, float y1, float x2, float y2, float x);
#endif