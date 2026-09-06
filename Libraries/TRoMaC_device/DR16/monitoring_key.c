#include "rc_SBUS.h"
#include "monitoring_key.h"


static void Key_TimeoutReload(KeyHandle_TypeDef *KeyHandle);

KeyHandle_TypeDef hKey[MONITORING_KEY_NUM_MAX];

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
                        uint16_t Key_17)
{
    Key_HandleInit(&hKey[0], Key_0);
    Key_HandleInit(&hKey[1], Key_1);
    Key_HandleInit(&hKey[2], Key_2);
    Key_HandleInit(&hKey[3], Key_3);
    Key_HandleInit(&hKey[4], Key_4);
    Key_HandleInit(&hKey[5], Key_5);
    Key_HandleInit(&hKey[6], Key_6);
    Key_HandleInit(&hKey[7], Key_7);
    Key_HandleInit(&hKey[8], Key_8);
    Key_HandleInit(&hKey[9], Key_9);
    Key_HandleInit(&hKey[10], Key_10);
    Key_HandleInit(&hKey[11], Key_11);
    Key_HandleInit(&hKey[12], Key_12);
    Key_HandleInit(&hKey[13], Key_13);
    Key_HandleInit(&hKey[14], Key_14);
    Key_HandleInit(&hKey[15], Key_15);
    Key_HandleInit(&hKey[16], Key_16);
    Key_HandleInit(&hKey[17], Key_17);
} /* Key_MonitoringInit() */

static void Key_TimeoutReload(KeyHandle_TypeDef *KeyHandle)
{
    KeyHandle->Timeout.DoublePress  = KEY_TIMEOUT_DOUBLE;
    KeyHandle->Timeout.LongPress    = KEY_TIMEOUT_LONG;
}/* Key_TimeoutReload() */

void Key_HandleInit(KeyHandle_TypeDef *KeyHandle, KeyID_enum KeyID)
{
    KeyHandle->KeyID            = KeyID;
    KeyHandle->KeyEvent         = KeyEvent_RaisedRaised;
    KeyHandle->KeyState         = KeyState_Raised;
    KeyHandle->RealState        = KeyState_Raised;
    Key_TimeoutReload(KeyHandle);
} /* Key_HandleInit() */

void Key_UpdateState(KeyHandle_TypeDef *KeyHandle)
{
    switch(KeyHandle->KeyID)
    {
        case Mouse_Left:
        {
            KeyHandle->RealState = rc_ctrl.mouse.press_l;
            break;
        }
        case Mouse_Right:
        {
            KeyHandle->RealState = rc_ctrl.mouse.press_r;
            break;
        }
		
        default:
        {
            KeyHandle->RealState = ((rc_ctrl.key.v & KeyHandle->KeyID) == KeyHandle->KeyID);
            break;
        }
    }
    
    KeyHandle->KeyEvent = (((KeyHandle->KeyEvent << 1) | KeyHandle->RealState) & 0x03);
    
    
    if(   KeyHandle->KeyState == KeyState_Raised            // 第一次按下
       && KeyHandle->KeyEvent == KeyEvent_RaisedPressed)  
    {
        KeyHandle->KeyState = KeyState_SinglePressed;       // 状态记为单击
    }
    
    // 当前为单击或双击状态
    if(KeyHandle->KeyState == KeyState_SinglePressed || KeyHandle->KeyState == KeyState_DoublePressed)
    {
        KeyHandle->Timeout.DoublePress--;                   // 双击超时自减
        
        if(KeyHandle->Timeout.DoublePress <= 0)    
        {
            if(KeyHandle->RealState == KeyState_Raised)     // 双击超时期间没有发生新的点击
            {
                KeyHandle->KeyState = KeyState_Raised;      // 状态记为抬起
            }
            else
            {
                KeyHandle->Timeout.DoublePress = -1;        
            }
        }
        else    // 超时期间
        {
            if(KeyHandle->KeyEvent == KeyEvent_RaisedPressed && KeyHandle->Timeout.DoublePress < (KEY_TIMEOUT_DOUBLE - 1))       // 超时期间发生了按下事件
            {
                if(KeyHandle->KeyState == KeyState_SinglePressed)
                {
                    KeyHandle->KeyState = KeyState_DoublePressed;       // 状态记为双击
                    KeyHandle->Timeout.LongPress = KEY_TIMEOUT_LONG;    // 长按超时复位
                }
            }
        }
    }
    
    if(KeyHandle->KeyState != KeyState_Raised)              // 只要按键不在抬起状态，则长按的超时自减
    {
        KeyHandle->Timeout.LongPress--;
    }
    else
    {
        Key_TimeoutReload(KeyHandle);
    }
    
    if(KeyHandle->Timeout.LongPress <= 0)                   // 长按超时
    {
        if((KeyHandle->KeyState & 0x01) == 0)
        {
            // 0001 0000 => 0010 0001   0100 0000 => 1000 0001
            KeyHandle->KeyState <<= 1;  
            KeyHandle->KeyState |= 0x01;
        }
        else
        {
            KeyHandle->Timeout.LongPress = -1;
            
            if(   KeyHandle->RealState == KeyState_Raised            // 长按状态下检测到抬起动作
               && KeyHandle->KeyEvent  == KeyEvent_PressedRaised)
            {
                KeyHandle->KeyState = KeyState_Raised;
                
            }
        }
    }
} /* Key_UpdateState() */

void Key_UpdateStateAll(void)
{
    for(uint8_t i = 0; i < MONITORING_KEY_NUM_MAX; i++)
    {
        Key_UpdateState(&hKey[i]);
    }
} /* Key_UpdateStateAll() */



/**
  * @name   Key_GetRealState_hKey
  * @brief  通过 @param KeyHandle 获得相应按键的实际状态
  * @call   External
  * @param  KeyHandle 要查询的按键的句柄
  * @RetVal KeyHandle 对应按键的实际状态
  */
KeyState_enum Key_GetRealState_hKey(KeyHandle_TypeDef *KeyHandle)
{
    return KeyHandle->RealState;
}

/**
  * @name   Key_GetRealState_KeyID
  * @brief  通过 @param KeyID 获得相应按键的实际状态
  * @call   External
  * @param  Key_ID      要查询的按键的ID
  * @RetVal Key_ID 对应按键的实际状态
  */
KeyState_enum Key_GetRealState_KeyID(KeyID_enum Key_ID)
{
    for(uint8_t i = 0; i < MONITORING_KEY_NUM_MAX; i++)
    {
        if(hKey[i].KeyID == Key_ID)
        {
            return hKey[i].RealState;
        }
    }
    return KeyState_Err;
}

/**
  * @name   Key_GetState_hKey
  * @brief  通过 @param KeyHandle 获得相应按键的状态
  * @call   External
  * @param  KeyHandle 要查询的按键的句柄
  * @RetVal KeyHandle 对应按键的状态
  */
KeyState_enum Key_GetState_hKey(KeyHandle_TypeDef *KeyHandle)
{
    return KeyHandle->KeyState;
}

/**
  * @name   Key_GetState_KeyID
  * @brief  通过 @param Key_ID 获得相应按键的状态
  * @call   External
  * @param  Key_ID 要查询的按键的ID
  * @RetVal Key_ID 对应按键的状态
  */
KeyState_enum Key_GetState_KeyID(KeyID_enum Key_ID)
{
    for(uint8_t i = 0; i < MONITORING_KEY_NUM_MAX; i++)
    {
        if(hKey[i].KeyID == Key_ID)
        {
            return hKey[i].KeyState;
        }
    }
    
    return KeyState_Err;
}


/**
  * @name   Key_GetEvent_hKey
  * @brief  通过 @param KeyHandle 获得相应按键的事件
  * @call   External
  * @param  KeyHandle 要查询的按键的句柄
  * @RetVal KeyHandle 对应按键的事件
  */
KeyEvent_enum Key_GetEvent_hKey(KeyHandle_TypeDef *KeyHandle)
{
    return KeyHandle->KeyEvent;
}

/**
  * @name   Key_GetEvent_KeyID
  * @brief  通过 @param Key_ID 获得相应按键的事件
  * @call   External
  * @param  Key_ID 要查询的按键的ID
  * @RetVal Key_ID 对应按键的事件
  */
KeyEvent_enum Key_GetEvent_KeyID(KeyID_enum Key_ID)
{
    for(uint8_t i = 0; i < MONITORING_KEY_NUM_MAX; i++)
    {
        if(hKey[i].KeyID == Key_ID)
        {
            return hKey[i].KeyEvent;
        }
    }
    
    return KeyState_Err;
}

