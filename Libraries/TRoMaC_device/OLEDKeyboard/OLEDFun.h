#ifndef __OLEDFun_H
#define __OLEDFun_H

#include "stm32f4xx_hal.h"
#include "OLEDkeyboard_Config.h"

/*================????================*/


/*===============??????===============*/

/*===============????????===============*/
int32_t KeyboardInput(uint16_t x,uint16_t y, TextSize_TypeDef TextSize);
int32_t KeyboardInput_with_Flash(uint16_t x,uint16_t y, TextSize_TypeDef TextSize, uint16_t sector, uint16_t page);
int32_t KeyboardInput_Query(uint16_t x,uint16_t y, uint8_t *CheckFlag, TextSize_TypeDef TextSize);
uint8_t ShowSwitch(uint16_t x,uint16_t y, TextSize_TypeDef TextSize, uint16_t sector, uint16_t page);
uint8_t ShowSwitch_with_Flash(uint16_t x,uint16_t y, TextSize_TypeDef TextSize, uint16_t sector, uint16_t page);


#endif /*__OLEDFun_H*/

