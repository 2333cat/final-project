#ifndef LCD1602_h
#define LCD1602_h
/***************************************************************************************

LCD 1602 跳线设置：

4:RES  0.2

5:R/W  2.7

6:E    2.8

7:DB0  2.1

8:DB1  0.3

9:DB2  1.9

10:DB3 2.4

11:DB4 2.5

12:DB5 0.6

13:DB6 0.7

14:DB7 2.9

*****************************************************************************************/

 

/************************************************************************************************************************************************************************ *源程序部分*************************************************

 */

#include "LPC11xx.h"

//#include "Urat.h"

 

#define TDELAY 2

void LCDInit( void );

void LCDputchar(uint32_t ,uint32_t  ,char );

void LCDputstr(uint32_t ,uint32_t  ,char * );

void ChanChar(char *,int  );

void ChanChar2(char *,int  );

void ChanChar3(char *,int  );

 

 

static void SetPIOOut( void );

static void SetPIOIn( void );

static void SetPIOData( uint32_t );

static void ReadPIOData (uint32_t *);

static void ReadLCDPIO(uint32_t *);

void WriteLCDPIO(uint32_t );

static void LCDWrite(uint32_t ,uint32_t );

static void LCDRead(uint32_t *,uint32_t );

static void lcdwriteinst(uint32_t );

static void lcdwritedata(uint32_t );

static void lcdreaddata(uint32_t *);

static void lcdIntial( void );



#endif