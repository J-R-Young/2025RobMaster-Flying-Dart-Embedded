#ifndef _DEV_CH455_H
#define _DEV_CH455_H

#include "OLEDkeyboard_Config.h"



/*===============宏定义===============*/

typedef enum
{
    KEY_0           = 0x00,
    KEY_1           = 0x01,
    KEY_2           = 0x02,
    KEY_3           = 0x03,
    KEY_4           = 0x04,
    KEY_5           = 0x05,
    KEY_6           = 0x06,
    KEY_7           = 0x07,
    KEY_8           = 0x08,
    KEY_9           = 0x09,
    KEY_BACK        = 0x0A,
    KEY_ENTER       = 0x0B,
                      
    KEY_0_Long      = (KEY_0 | 0x10U),
    KEY_1_Long      = (KEY_1 | 0x10U),
    KEY_2_Long      = (KEY_2 | 0x10U),
    KEY_3_Long      = (KEY_3 | 0x10U),
    KEY_4_Long      = (KEY_4 | 0x10U),
    KEY_5_Long      = (KEY_5 | 0x10U),
    KEY_6_Long      = (KEY_6 | 0x10U),
    KEY_7_Long      = (KEY_7 | 0x10U),
    KEY_8_Long      = (KEY_8 | 0x10U),
    KEY_9_Long      = (KEY_9 | 0x10U),
    KEY_BACK_Long   = (KEY_BACK | 0x10U),
    KEY_ENTER_Long  = (KEY_ENTER | 0x10U),
    
    KEY_BLANK       = 0xFF
}KeyValue_TypeDef;

typedef struct
{
    uint8_t             CurrentKeyCode;
    
    KeyValue_TypeDef    CurrentKey;
    KeyValue_TypeDef    LastKey;
    
    KeyHandle_TypeDef   KeyStateHandle;
}KeyboardHandle_Typedef;

/*===============外部变量===============*/
//KeyboardHandle_Typedef  Keyboard;

/*===============函数声明===============*/
void CH455_Init(void);
uint8_t CH455_Read(void);
uint8_t CH455_GetOneKey(void);
uint8_t CH455_QueryOneKey(void);
uint8_t CH455_KeyCode2KeyValue(uint8_t KeyCode);

uint8_t CH455_GetActiveKey(void);
uint8_t CH455_GetKeyState(void);
uint8_t CH455_GetRealState(void);
uint8_t CH455_GetKeyEvent(void);


#endif /* _DEV_CH455_H */

