#ifndef __FONT_H
#define __FONT_H

#include "stm32f4xx_hal.h"
#include "OLEDkeyboard_Config.h"

typedef enum 
{
    TextSize_F6x8   = 1,
    TextSize_F8x16  = 2,
    TextSize_GUI_switch_22x16   = 3,
    TextSize_GUI_switch_16x8    = 4,
}TextSize_TypeDef;


extern const uint8_t F6x8[][6];
extern const uint8_t F8x16[];
extern const uint8_t GUI_switch_22x16[4][22]; 
extern const uint8_t GUI_switch_16x8[2][16];

#endif  /*__FONT_H*/

