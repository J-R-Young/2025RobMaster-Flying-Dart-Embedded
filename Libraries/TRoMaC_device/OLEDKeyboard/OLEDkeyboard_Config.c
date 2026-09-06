#include "OLEDkeyboard_Config.h"


SimiicHandle_TypeDef            Simiic_OLEDkeyboard = {0};
OLEDkeyboardHandle_TypeDef      OLEDkeyboardHandle  = {0};
//KeyHandle_TypeDef       Keyboard;

void OLEDkeyboard_Config(void)
{
    // 先初始化模拟IIC总线句柄
    Simiic_HandleInit(&Simiic_OLEDkeyboard, 
                      OLEDkeyboard_SDA_Port, OLEDkeyboard_SDA_Pin,
                      OLEDkeyboard_SCL_Port, OLEDkeyboard_SCL_Pin,
                      600000               , BitOrder_MSB);
    
    OLEDkeyboard_HandleInit();
    
    OLED_Init();
    
    CH455_Init();
	
	OLED_CLS();
}

void OLEDkeyboard_HandleInit(void)
{
    OLEDkeyboardHandle.OLED.Freq                = 30;
    OLEDkeyboardHandle.OLED.Period              = 1000 / OLEDkeyboardHandle.OLED.Freq;
    OLEDkeyboardHandle.OLED.Interface_Parent    = NULL;
    OLEDkeyboardHandle.OLED.Interface_Current   = NULL;
    OLEDkeyboardHandle.OLED.Interface_Child     = NULL;
    
    OLEDkeyboardHandle.keyboard.Period          = 10;
    OLEDkeyboardHandle.keyboard.ActiveKey[0]    = KEY_BLANK;
    OLEDkeyboardHandle.keyboard.Timeout.DoublePress_Reload  = 320;
    OLEDkeyboardHandle.keyboard.Timeout.LongPress_Reload    = 350;
    
}/* OLEDkeyboard_HandleInit() */

uint8_t OLEDkeyboard_KeyUpdate(void)
{
    // 迭代KeyCode
    OLEDkeyboardHandle.keyboard.KeyCode[1] = OLEDkeyboardHandle.keyboard.KeyCode[0];
    OLEDkeyboardHandle.keyboard.KeyCode[0] = CH455_Read();
    
    // 根据KeyCode判断当前有没有按键按下
    if(OLEDkeyboardHandle.keyboard.KeyCode[0] <= 0x40U) 
    {
        OLEDkeyboardHandle.keyboard.RealState = KeyState_Raised;
    }
    else
    {
        OLEDkeyboardHandle.keyboard.RealState = KeyState_Pressed;
    }
    
    // 得到KeyEvent
    OLEDkeyboardHandle.keyboard.KeyEvent = (((OLEDkeyboardHandle.keyboard.KeyEvent << 1) | OLEDkeyboardHandle.keyboard.RealState) & 0x03U);
    
    // 由于监测对象为键盘，则对按键的改变做判断
    if(OLEDkeyboardHandle.keyboard.KeyEvent == KeyEvent_PressedPressed)
    {
        // 前一个按键还没检测到抬起就按下下一个的情况
        if(OLEDkeyboardHandle.keyboard.KeyCode[1] != OLEDkeyboardHandle.keyboard.KeyCode[0])
        {
            OLEDkeyboardHandle.keyboard.KeyEvent = KeyEvent_Change;
            
            // 对状态进行清空以及对超时做复位
            OLEDkeyboardHandle.keyboard.Timeout.DoublePress = OLEDkeyboardHandle.keyboard.Timeout.DoublePress_Reload;   // 双击超时复位
            OLEDkeyboardHandle.keyboard.Timeout.LongPress = OLEDkeyboardHandle.keyboard.Timeout.LongPress_Reload;       // 长按超时复位
            /* 函数 */
        }
    }

    // 更新按键状态
    
    // 按键按下
    if(   (   OLEDkeyboardHandle.keyboard.KeyState == KeyState_Raised
           && OLEDkeyboardHandle.keyboard.KeyEvent == KeyEvent_RaisedPressed)
       || OLEDkeyboardHandle.keyboard.KeyEvent == KeyEvent_Change
      )
    {
        // 获取当前的有效按键
        OLEDkeyboardHandle.keyboard.ActiveKey[1] = OLEDkeyboardHandle.keyboard.ActiveKey[0];
        switch(OLEDkeyboardHandle.keyboard.KeyCode[0])
        {
            case 0x57 :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_1;       break;
            case 0x4f :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_2;       break;
            case 0x47 :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_3;       break;
            case 0x56 :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_4;       break;
            case 0x4e :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_5;       break;
            case 0x46 :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_6;       break;
            case 0x55 :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_7;       break;
            case 0x4d :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_8;       break;
            case 0x45 :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_9;       break;
            case 0x4c :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_0;       break;
            case 0x54 :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_BACK;    break;   //退格键
            case 0x44 :     OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_ENTER;   break;   //确认键
            default:        OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_BLANK;   break;
        }
        
        if(OLEDkeyboardHandle.keyboard.ActiveKey[0] == KEY_BLANK)   
        {
            OLEDkeyboardHandle.keyboard.KeyState = KeyState_Raised;
            
            // 这种情况有可能是两个按键同时按下，之后根据需要来考虑要不要对这种情况做处理
        }
        else
        {
            OLEDkeyboardHandle.keyboard.KeyState = KeyState_SinglePressed;
            
            OLEDkeyboardHandle.keyboard.Timeout.DoublePress = OLEDkeyboardHandle.keyboard.Timeout.DoublePress_Reload;   // 双击超时复位
            OLEDkeyboardHandle.keyboard.Timeout.LongPress = OLEDkeyboardHandle.keyboard.Timeout.LongPress_Reload;       // 长按超时复位
        }
    }
    
    // 双击超时结算
    if(   OLEDkeyboardHandle.keyboard.KeyState == KeyState_SinglePressed
       || OLEDkeyboardHandle.keyboard.KeyState == KeyState_DoublePressed)
    {
        OLEDkeyboardHandle.keyboard.Timeout.DoublePress -= OLEDkeyboardHandle.keyboard.Period;
        
        if(OLEDkeyboardHandle.keyboard.Timeout.DoublePress <= 0)    // 到达超时时间
        {
            if(OLEDkeyboardHandle.keyboard.RealState == KeyState_Raised)    // 真正抬起
            {
                OLEDkeyboardHandle.keyboard.KeyState = KeyState_Raised;
                
                OLEDkeyboardHandle.keyboard.ActiveKey[1] = OLEDkeyboardHandle.keyboard.ActiveKey[0];
                OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_BLANK;
                
            }
            else                                                            // 有按键按下
            {
                OLEDkeyboardHandle.keyboard.Timeout.DoublePress = -1;
            }
        }
        else    // 超时期间
        {
            if(   OLEDkeyboardHandle.keyboard.KeyEvent == KeyEvent_RaisedPressed
               && OLEDkeyboardHandle.keyboard.Timeout.DoublePress < (OLEDkeyboardHandle.keyboard.Timeout.DoublePress_Reload - OLEDkeyboardHandle.keyboard.Period))
            {
                if(OLEDkeyboardHandle.keyboard.KeyState == KeyState_SinglePressed)
                {
                    // 判断是否是按下了别的按键
                    if(CH455_KeyCode2KeyValue(OLEDkeyboardHandle.keyboard.KeyCode[0]) == OLEDkeyboardHandle.keyboard.ActiveKey[0])
                    {
                        OLEDkeyboardHandle.keyboard.KeyState = KeyState_DoublePressed;  // 记录状态为双击
//                        OLEDkeyboardHandle.keyboard.Timeout.DoublePress = OLEDkeyboardHandle.keyboard.Timeout.DoublePress_Reload;   // 双击超时复位
                        OLEDkeyboardHandle.keyboard.Timeout.LongPress = OLEDkeyboardHandle.keyboard.Timeout.LongPress_Reload;       // 长按超时复位
                    }
                    else
                    {
                        OLEDkeyboardHandle.keyboard.KeyState = KeyState_SinglePressed;  // 记录状态为单击
                        OLEDkeyboardHandle.keyboard.Timeout.DoublePress = OLEDkeyboardHandle.keyboard.Timeout.DoublePress_Reload;   // 双击超时复位
                        OLEDkeyboardHandle.keyboard.Timeout.LongPress = OLEDkeyboardHandle.keyboard.Timeout.LongPress_Reload;       // 长按超时复位
                        
                        OLEDkeyboardHandle.keyboard.ActiveKey[1] = OLEDkeyboardHandle.keyboard.ActiveKey[0];
                        OLEDkeyboardHandle.keyboard.ActiveKey[0] = CH455_KeyCode2KeyValue(OLEDkeyboardHandle.keyboard.KeyCode[0]);
                    }
                    
                }
            }
        }
    }/* 双击超时结算 */
    
    
    // 长按超时结算
    if(OLEDkeyboardHandle.keyboard.KeyState != KeyState_Raised)   // 当前有按键按下
    {
       OLEDkeyboardHandle.keyboard.Timeout.LongPress -= OLEDkeyboardHandle.keyboard.Period;
    }
    else    // 长按超时结束，对超时进行复位
    {
        OLEDkeyboardHandle.keyboard.Timeout.DoublePress = OLEDkeyboardHandle.keyboard.Timeout.DoublePress_Reload;   // 双击超时复位
        OLEDkeyboardHandle.keyboard.Timeout.LongPress = OLEDkeyboardHandle.keyboard.Timeout.LongPress_Reload;       // 长按超时复位
    }
    
    if(OLEDkeyboardHandle.keyboard.Timeout.LongPress <= 0)
    {
        if((OLEDkeyboardHandle.keyboard.KeyState & 0x01U) == 0) // 状态记录为长按
        {   
            // 0001 0000 => 0010 0001   0100 0000 => 1000 0001
            OLEDkeyboardHandle.keyboard.KeyState <<= 1;
            OLEDkeyboardHandle.keyboard.KeyState |= 0x01U;
        }
        else
        {
            OLEDkeyboardHandle.keyboard.Timeout.LongPress = -1;
            
            // 长按状态下检测抬起动作
            if(   OLEDkeyboardHandle.keyboard.RealState == KeyState_Raised
               && OLEDkeyboardHandle.keyboard.KeyEvent  == KeyEvent_PressedRaised)
            {
                OLEDkeyboardHandle.keyboard.KeyState = KeyState_Raised;
                
                OLEDkeyboardHandle.keyboard.ActiveKey[1] = OLEDkeyboardHandle.keyboard.ActiveKey[0];
                OLEDkeyboardHandle.keyboard.ActiveKey[0] = KEY_BLANK;
            }
        }
    }/* 长按超时结算 */
    
    return OLEDkeyboardHandle.keyboard.ActiveKey[0];
}/* OLEDkeyboard_KeyUpdate() */




