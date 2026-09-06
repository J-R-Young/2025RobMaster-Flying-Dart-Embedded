#include "dev_CH455.h"


/*************************************************
Function: CH455_Init
Description:配置CH455的工作方式
Details：无
*************************************************/
void CH455_Init(void)
{
    Simiic_Start(&Simiic_OLEDkeyboard);
    
    Simiic_Send_Byte(&Simiic_OLEDkeyboard, Simiic_Addr_W(0x48U));
    Simiic_WaitACK(&Simiic_OLEDkeyboard);
    
    Simiic_Send_Byte(&Simiic_OLEDkeyboard, 0x01U);
    Simiic_WaitACK(&Simiic_OLEDkeyboard);
    
    Simiic_Stop(&Simiic_OLEDkeyboard);
}


/*************************************************
Function: CH455_Read
Description:从CH455的寄存器获取当前的按键状态
Details：无
*************************************************/
uint8_t CH455_Read(void)
{
    uint8_t keycode = 0;
    
    Simiic_Start(&Simiic_OLEDkeyboard);
    
    Simiic_Send_Byte(&Simiic_OLEDkeyboard, Simiic_Addr_R(0x4FU));
    Simiic_WaitACK(&Simiic_OLEDkeyboard);
    
    keycode = Simiic_Read_Byte(&Simiic_OLEDkeyboard, SimiicAck_NACK);
    
    Simiic_Stop(&Simiic_OLEDkeyboard);
    
    return keycode;
}

uint8_t CH455_KeyCode2KeyValue(uint8_t KeyCode)
{
    uint8_t ValuableKeyCode = KEY_BLANK;
    
    switch(KeyCode)
    {
        case 0x57 :     ValuableKeyCode = KEY_1;       break;
        case 0x4f :     ValuableKeyCode = KEY_2;       break;
        case 0x47 :     ValuableKeyCode = KEY_3;       break;
        case 0x56 :     ValuableKeyCode = KEY_4;       break;
        case 0x4e :     ValuableKeyCode = KEY_5;       break;
        case 0x46 :     ValuableKeyCode = KEY_6;       break;
        case 0x55 :     ValuableKeyCode = KEY_7;       break;
        case 0x4d :     ValuableKeyCode = KEY_8;       break;
        case 0x45 :     ValuableKeyCode = KEY_9;       break;
        case 0x4c :     ValuableKeyCode = KEY_0;       break;
        case 0x54 :     ValuableKeyCode = KEY_BACK;    break;   //退格键
        case 0x44 :     ValuableKeyCode = KEY_ENTER;   break;   //确认键
        default:        ValuableKeyCode = KEY_BLANK;   break;
    }
    
    return ValuableKeyCode;
}

/*************************************************
Function: CH455_GetOneKey
Description:从CH455获取一个按键抬起事件    （等待式）
Details：进入函数后一直获取CH455寄存器的值，直到检测到一个有效的按键抬起事件
*************************************************/
uint8_t CH455_GetOneKey(void)
{
    uint8_t  KeyCodeOld = KEY_BLANK;
    uint8_t  KeyCode = CH455_Read();
    uint16_t KeyValue = KEY_BLANK;  //短按时记录在低4位 长按记录在高4位
    uint16_t timeout = 15000;   
    
    do{
        KeyCodeOld = CH455_Read();
    }while(KeyCodeOld < 0x40); /*等待按键按下*/
    
    while(KeyValue == KEY_BLANK && timeout > 0)
    {
        KeyCode = CH455_Read();
        
        if(KeyCodeOld - KeyCode == 0x40)    //检测到按键抬起
        {
            switch(KeyCode)
            {
                case 0x17 :     KeyValue = KEY_1;       break;
                case 0x0f :     KeyValue = KEY_2;       break;
                case 0x07 :     KeyValue = KEY_3;       break;
                case 0x16 :     KeyValue = KEY_4;       break;
                case 0x0e :     KeyValue = KEY_5;       break;
                case 0x06 :     KeyValue = KEY_6;       break;
                case 0x15 :     KeyValue = KEY_7;       break;
                case 0x0d :     KeyValue = KEY_8;       break;
                case 0x05 :     KeyValue = KEY_9;       break;
                case 0x0c :     KeyValue = KEY_0;       break;
                case 0x14 :     KeyValue = KEY_BACK;    break;   //退格键
                case 0x04 :     KeyValue = KEY_ENTER;   break;   //确认键
                default:                                break;
            }
        }
        
        timeout--;
    }
    
    if(KeyValue != KEY_BLANK && timeout <= 100)
    {
        KeyValue <<= 4;
        KeyValue |= 0x0F; 
    }
    
    if(KeyValue == KEY_BLANK && timeout == 0)  //由于超时跳出循环
    {
        /*根据当前按下的键返回数值*/
        switch(KeyCode)
        {
            case 0x57 :     KeyValue = KEY_1_Long;       break;
            case 0x4f :     KeyValue = KEY_2_Long;       break;
            case 0x47 :     KeyValue = KEY_3_Long;       break;
            case 0x56 :     KeyValue = KEY_4_Long;       break;
            case 0x4e :     KeyValue = KEY_5_Long;       break;
            case 0x46 :     KeyValue = KEY_6_Long;       break;
            case 0x55 :     KeyValue = KEY_7_Long;       break;
            case 0x4d :     KeyValue = KEY_8_Long;       break;
            case 0x45 :     KeyValue = KEY_9_Long;       break;
            case 0x4c :     KeyValue = KEY_0_Long;       break;
            case 0x54 :     KeyValue = KEY_BACK_Long;    break;   //退格键
            case 0x44 :     KeyValue = KEY_ENTER_Long;   break;   //确认键
            default:                                     break;
        }
    }
    
    return KeyValue;
}



uint8_t CH455_QueryOneKey(void)
{
    // 实现思路 每次进此函数则与CH455通信一次，并记录通信得到的按键代码，得到按键代码后与历史按键代码做对比，检测抬起动作

    static uint8_t ValuableKeyCode = KEY_BLANK;
    static uint8_t LastKeyCode = KEY_BLANK;
    static uint16_t Count = 0;

    uint8_t ThisKeyCode = CH455_Read();


    if(KEY_BLANK == ValuableKeyCode)    // 当前没有有效按键
    {
        if(ThisKeyCode <= 0x40)
        {
            return KEY_BLANK;
        }
        else
        {
            switch(ThisKeyCode)
            {
                case 0x57 :     ValuableKeyCode = KEY_1;       break;
                case 0x4f :     ValuableKeyCode = KEY_2;       break;
                case 0x47 :     ValuableKeyCode = KEY_3;       break;
                case 0x56 :     ValuableKeyCode = KEY_4;       break;
                case 0x4e :     ValuableKeyCode = KEY_5;       break;
                case 0x46 :     ValuableKeyCode = KEY_6;       break;
                case 0x55 :     ValuableKeyCode = KEY_7;       break;
                case 0x4d :     ValuableKeyCode = KEY_8;       break;
                case 0x45 :     ValuableKeyCode = KEY_9;       break;
                case 0x4c :     ValuableKeyCode = KEY_0;       break;
                case 0x54 :     ValuableKeyCode = KEY_BACK;    break;   //退格键
                case 0x44 :     ValuableKeyCode = KEY_ENTER;   break;   //确认键
                
                default:        ValuableKeyCode = KEY_BLANK;   break;
            }
            Count = 0;

            LastKeyCode = ThisKeyCode;
            return 0xFF - ValuableKeyCode;
        }
    }/* KEY_BLANK == ValuableKeyCode */
    else
    {
        if(ThisKeyCode < 0x40 && LastKeyCode - ThisKeyCode == 0x40)  // 本次查询按键抬起
        {
            uint8_t temp;

            if(Count > 23)  // 根据时长判断是否为长按
            {
                temp = (ValuableKeyCode | 0x10U);
            }
            else
            {
                temp = ValuableKeyCode;
            }

            Count = 0;
            LastKeyCode = KEY_BLANK;
            ValuableKeyCode = KEY_BLANK;

            return temp;
        }
        else    // 本次查询按键还在按下状态
        {
            uint8_t temp;
            Count++;
            LastKeyCode = ThisKeyCode;

            if(Count > 25)
            {
                temp = (ValuableKeyCode << 4) | 0x0F;     // 转换为长按
                temp = (ValuableKeyCode | 0x10U);
//                Count = 0;
                LastKeyCode = ThisKeyCode;
            }
            else
            {
                temp = 0xFF - ValuableKeyCode;
            }

            return temp;

        }/* ThisKeyCode >= 0x40 */


    }/* KEY_BLANK != ValuableKeyCode */

}

uint8_t CH455_GetActiveKey(void)
{
    return OLEDkeyboardHandle.keyboard.ActiveKey[0];
}

uint8_t CH455_GetKeyState(void)
{
    return OLEDkeyboardHandle.keyboard.KeyState;
}

uint8_t CH455_GetRealState(void)
{
    return OLEDkeyboardHandle.keyboard.RealState;
}

uint8_t CH455_GetKeyEvent(void)
{
    return OLEDkeyboardHandle.keyboard.KeyEvent;
}


