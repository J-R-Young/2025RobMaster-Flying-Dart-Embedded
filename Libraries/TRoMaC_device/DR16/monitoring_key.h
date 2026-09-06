#ifndef __MONITORING_KEY
#define __MONITORING_KEY
#include "main.h"


#define MONITORING_KEY_NUM_MAX 18


#define KEY_TIMEOUT_DOUBLE  ((int16_t)20u)
#define KEY_TIMEOUT_LONG    ((int16_t)25u)

typedef enum
{
    Key_W               = (0x0001 << 0),
    Key_S               = (0x0001 << 1),
    Key_A               = (0x0001 << 2),
    Key_D               = (0x0001 << 3),
    Key_Shift           = (0x0001 << 4),
    Key_Ctrl            = (0x0001 << 5),
    Key_Q          	    = (0x0001 << 6),
    Key_E               = (0x0001 << 7),
    
    Key_R               = (0x0001 << 8),
    Key_F               = (0x0001 << 9),
    Key_G               = (0x0001 << 10),
    Key_Z               = (0x0001 << 11),
    Key_X               = (0x0001 << 12),
    Key_C               = (0x0001 << 13),
    Key_V               = (0x0001 << 14),
    Key_B               = (0x0001 << 15),
    
    Mouse_Left          = 0x00AA,
    Mouse_Right         = 0x00BB,
}KeyID_enum;

typedef enum
{
    // 基础状态
    KeyState_Raised                 = 0x00,     // 0000 0000
    KeyState_Pressed                = 0x01,     // 0000 0001
    
    // 有效状态
    KeyState_SinglePressed          = 0x10,     // 0001 0000
    KeyState_SinglePressedLong      = 0x21,     // 0010 0001
    KeyState_DoublePressed          = 0x40,     // 0100 0000
    KeyState_DoublePressedLong      = 0x81,     // 1000 0001
    
    KeyState_Err                    = 0xEE,
}KeyState_enum;

typedef enum
{
    // KeyEvent_LastNow
    KeyEvent_RaisedRaised       = (KeyState_Raised  << 1 | KeyState_Raised),
    KeyEvent_RaisedPressed      = (KeyState_Raised  << 1 | KeyState_Pressed),
    KeyEvent_PressedPressed     = (KeyState_Pressed << 1 | KeyState_Pressed),
    KeyEvent_PressedRaised      = (KeyState_Pressed << 1 | KeyState_Raised),
	KeyEvent_Change             = (0x07U),

}KeyEvent_enum;

typedef struct
{
    KeyID_enum          KeyID;
    
    KeyState_enum       RealState;
    KeyEvent_enum       KeyEvent;
    KeyState_enum       KeyState;  
    
    struct
    {
        int16_t DoublePress;    // 双击超时
        int16_t LongPress;      // 长按超时
    }Timeout;
    
}KeyHandle_TypeDef;

extern KeyHandle_TypeDef hKey[MONITORING_KEY_NUM_MAX];

void Key_MonitoringInit(uint16_t Key_0,
                        uint16_t Key_1, 
                        uint16_t Key_2, 
                        uint16_t Key_3, 
                        uint16_t Key_4, 
                        uint16_t Key_5, 
                        uint16_t Key_6, 
                        uint16_t Key_7, 
                        uint16_t Key_8, 
                        uint16_t Key_9, 
                        uint16_t Key_10, 
                        uint16_t Key_11, 
                        uint16_t Key_12, 
                        uint16_t Key_13, 
                        uint16_t Key_14, 
                        uint16_t Key_15, 
                        uint16_t Key_16, 
                        uint16_t Key_17);
void Key_HandleInit(KeyHandle_TypeDef *KeyHandle, KeyID_enum KeyID);
void Key_UpdateStateAll(void);

KeyState_enum Key_GetRealState_hKey(KeyHandle_TypeDef *KeyHandle);
KeyState_enum Key_GetRealState_KeyID(KeyID_enum Key_ID);
KeyState_enum Key_GetState_hKey(KeyHandle_TypeDef *KeyHandle);
KeyState_enum Key_GetState_KeyID(KeyID_enum Key_ID);
KeyEvent_enum Key_GetEvent_hKey(KeyHandle_TypeDef *KeyHandle);
KeyEvent_enum Key_GetEvent_KeyID(KeyID_enum Key_ID);

#endif

