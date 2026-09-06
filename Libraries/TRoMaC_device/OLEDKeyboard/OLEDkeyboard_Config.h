#ifndef __OLEDKEYBOARD_CONFIG_H
#define __OLEDKEYBOARD_CONFIG_H

#include "stm32f4xx_hal.h"
#include "main.h"

#include "Simiic.h"
#include "monitoring_key.h"

#include "Font.h"
#include "dev_ssd1306.h"
#include "dev_CH455.h"
#include "FlashFun.h"
#include "OLEDFun.h"
#include "OLEDkeyboard.h"

/*================宏定义================*/

typedef enum
{
    false = 0,
    true  = !false
}boolean;

typedef enum
{
    Switch_OFF = 0,
    Switch_ON  = !Switch_OFF,
}SwitchStatus_typeDef;

#define OLEDkeyboard_SDA_Port       GPIOB
#define OLEDkeyboard_SDA_Pin        GPIO_PIN_11
#define OLEDkeyboard_SCL_Port       GPIOB
#define OLEDkeyboard_SCL_Pin        GPIO_PIN_10

typedef struct
{
    struct   
    {
        int8_t      Freq;               // 显示的刷新率
        int16_t     Period;             // 每一场图像的周期
        
        void        (*Interface_Parent)(void);      // 父界面函数的指针
        void        (*Interface_Current)(void);     // 当前界面函数的指针
        void        (*Interface_Child)(void);       // 子界面函数的指针
    }OLED;
    
    struct 
    {
        uint8_t     KeyCode[2];         // 原始键盘代码 [0]-当前 [1]-上一次
        
        KeyEvent_enum     KeyEvent;           // 键盘事件
        
        KeyState_enum     RealState;          // 按下的状态
        KeyState_enum     KeyState;           // 按键状态
        
        uint8_t     ActiveKey[2];       // 有效按键     [0]-当前  [1]-上一次
        
        struct                          // 超时单位是ms
        {   
            int16_t DoublePress;        // 双击超时
            int16_t LongPress;          // 长按超时
            
            int16_t DoublePress_Reload; // 双击超时重装载值
            int16_t LongPress_Reload;   // 长按超时重装载值
        }Timeout;
        
        int16_t     Period;             // 每一次查询的周期
    }keyboard;
    
    
}OLEDkeyboardHandle_TypeDef;

/*===============外部变量===============*/
extern SimiicHandle_TypeDef             Simiic_OLEDkeyboard;
extern OLEDkeyboardHandle_TypeDef       OLEDkeyboardHandle;

/*===============函数声明===============*/
void OLEDkeyboard_Config(void);
void OLEDkeyboard_HandleInit(void);
uint8_t OLEDkeyboard_KeyUpdate(void);


#endif  /*__OLEDKEYBOARD_CONFIG_H*/

