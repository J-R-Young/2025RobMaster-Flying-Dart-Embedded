#ifndef __DEV_SSD1306_H
#define __DEV_SSD1306_H

#include "stm32f4xx_hal.h"
#include "OLEDkeyboard_Config.h"
#include "Fun.h"

/*===============宏定义===============*/

#define num2ascll(x)    ('0'+(x))

#define OLED_CLS(void)  OLED_Fill(0x00)     //清屏


/*===============外部变量===============*/
/*===============函数声明===============*/
void OLED_Cmd(uint8_t cmd);
void OLED_Dat(uint8_t data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x,unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_Show_Char(uint16_t x, uint16_t y,char str, TextSize_TypeDef TextSize);
void OLED_Show_Str(uint16_t x, uint16_t y,uint8_t ch[], TextSize_TypeDef TextSize);
void OLED_Show_OneNumber(uint16_t x,uint16_t y,uint8_t num, TextSize_TypeDef TextSize);
void OLED_Show_Numbers(uint16_t x, uint16_t y,int32_t Value, TextSize_TypeDef TextSize);
void OLED_Show_Numbers_2(uint16_t x, uint16_t y,int32_t Value, TextSize_TypeDef TextSize);
void OLED_Show_float(uint16_t x, uint16_t y,double dat, uint8_t pointnum, TextSize_TypeDef TextSize);

void OLED_test(void);
    
#endif
