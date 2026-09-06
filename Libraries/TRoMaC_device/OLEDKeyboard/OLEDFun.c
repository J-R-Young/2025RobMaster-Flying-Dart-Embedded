#include "stm32f4xx_hal.h"
#include "OLEDkeyboard_Config.h"
#include "OLEDFun.h"


/*************************************************
Function: KeyboardInput
Description:键盘输入    (最多5位)
Details：无
*************************************************/
int32_t KeyboardInput(uint16_t x,uint16_t y, TextSize_TypeDef TextSize) //（without Flash）
{
    int KeyValue = KEY_BLANK;   //输入的新数字
    int KeyValue_old;
    int NewVlaue = 0;           //输入区显示出来的数字
    
    /*先在要输入信息的位置显示一行0*/
    OLED_Show_Numbers(x, y, 0, TextSize);
    
    while(KeyValue != KEY_ENTER)
    {
        KeyValue_old = KeyValue;
        KeyValue = CH455_GetOneKey();
        
        /*当前不对长按进行特殊处理*/
        if(KeyValue > 0x0F && KeyValue != KEY_BLANK)
        {
            KeyValue = (KeyValue - 0x0F) >> 4;
        }
        
        /* 直接跳过的情况取 0 */
        if(KeyValue_old == KEY_BLANK && (KeyValue == KEY_ENTER || KeyValue == KEY_ENTER_Long))
        {
            NewVlaue = 0;
        }
        
        /****************************输入数字可退格****************************/
        if(KeyValue != KEY_BACK && KeyValue != KEY_ENTER && KeyValue != KEY_BLANK)   //输入数值
        {
            if(NewVlaue / 1000 != 0)       //只保留四位
            {
                NewVlaue = NewVlaue % 1000;
            }
            
            NewVlaue = NewVlaue * 10 + KeyValue;      //新输入的数字放到个位
        }
        else if(KeyValue == KEY_BACK)    //退格
        {
            NewVlaue = NewVlaue / 10;     //最后一位去掉
        }
        
        OLED_Show_Numbers(x, y, NewVlaue, TextSize);   //显示当前值
    }
    
    return NewVlaue;
}

/*************************************************
Function: KeyboardInput_with_Flash
Description:键盘输入并存入Flash    (最多5位)
Details：大号字体显示四位数，小号字体显示五位
*************************************************/
int32_t KeyboardInput_with_Flash(uint16_t x,uint16_t y, TextSize_TypeDef TextSize, uint16_t sector, uint16_t page)
{
    int KeyValue = KEY_BLANK;    //输入的新数字
    int KeyValue_old;
    int NewVlaue = 0;           //输入区显示出来的数字
    
    /*先在要输入信息的位置显示一行0*/
    OLED_Show_Numbers(x, y, 0, TextSize);
    
    while(KeyValue != KEY_ENTER)
    {
        KeyValue_old = KeyValue;
        KeyValue = CH455_GetOneKey();
        
        /* 当前不对长按进行特殊处理 */
        if(KeyValue > 0x0F && KeyValue != KEY_BLANK)
        {
            KeyValue = (KeyValue - 0x0F) >> 4;
        }
        
        /* 直接跳过的情况保留原值 */
        if(KeyValue_old == KEY_BLANK && (KeyValue == KEY_ENTER || KeyValue == KEY_ENTER_Long))
        {
            NewVlaue = Flash_Read_Word(FLASH_BACKUP_SECTOR, sector, page);
        }
        
        
        
        /****************************输入数字可退格****************************/
        if(KeyValue != KEY_BACK && KeyValue != KEY_ENTER && KeyValue != KEY_BLANK)   //输入数值
        {
            if(NewVlaue / 10000 != 0)       //只保留5位
            {
                NewVlaue = NewVlaue % 10000;
            }
            
            NewVlaue = NewVlaue * 10 + KeyValue;      //新输入的数字放到个位
        }
        else if(KeyValue == KEY_BACK)    //退格
        {
            NewVlaue = NewVlaue / 10;     //最后一位去掉
        }
        
        OLED_Show_Numbers(x, y, NewVlaue, TextSize);   //显示当前值
    }
    
    Flash_write_Word(FLASH_OLEDKBD_SECTOR, sector, page, NewVlaue);
    
    return NewVlaue;
}


/*************************************************
Function: KeyboardInput_Query
Description:键盘输入    (最多5位)
Details：无
*************************************************/
int32_t KeyboardInput_Query(uint16_t x,uint16_t y, uint8_t *CheckFlag, TextSize_TypeDef TextSize)
{
    static uint8_t InputState = 0;
    static uint8_t KeyLock = 0;
    static uint8_t KeyCode = KEY_BLANK;
    static int KeyValue = KEY_BLANK;   //输入的新数字
    static int KeyValue_old;
    static int NewVlaue = 0;           //输入区显示出来的数字
    int returntmp = 0;
    
    switch(InputState)
    {
        case 0:
            /*先在要输入信息的位置显示一行0*/
            OLED_Show_Numbers(x, y, 0, TextSize);
            InputState = 1;
            break;
        case 1:
            KeyCode = CH455_QueryOneKey();
        
            if(KeyCode != KEY_ENTER)
            {
//                KeyValue_old = KeyValue;
                
                if(KeyLock == 0 && (KeyCode & 0xF0) == 0xF0 && KeyCode != KEY_BLANK)
                {
                    KeyValue = 0xFF - KeyCode;
                    KeyLock = 1;
                }
                
                // 锁定状态且当前没有按键按下    // 检测到抬起
                if(KeyLock == 1 && KeyCode == KEY_BLANK)    
                {
                    KeyLock = 0;
                    
                    if(KeyValue != KEY_BACK && KeyValue != KEY_ENTER)
                    {
                        if(NewVlaue / 1000 != 0)       //只保留四位
                        {
                            NewVlaue = NewVlaue % 1000;
                        }
                        
                        NewVlaue = NewVlaue * 10 + (KeyValue - 1);      //新输入的数字放到个位
                    }
                    else if(KeyValue == KEY_BACK)   // 退格
                    {
                        NewVlaue = NewVlaue / 10;     //最后一位去掉
                    }
                }
                
                OLED_Show_Numbers(x, y, NewVlaue, TextSize);   //显示当前值
                
            }
            else
            {
                InputState = 2;
            }
            break;
        case 2:
            KeyCode = KEY_BLANK;
            returntmp = NewVlaue;
            NewVlaue = 0;
            InputState = 0;
            *CheckFlag = 1;
            return returntmp;
            
    }
    
    *CheckFlag = 0;
    return 0;
}

/*************************************************
Function: ShowSwitch
Description:显示开关量的状态
Details：无
*************************************************/
uint8_t ShowSwitch(uint16_t x,uint16_t y, TextSize_TypeDef TextSize, uint16_t sector, uint16_t page)
{
    uint8_t switch_status;              //变量开关标志
    switch_status = flash_read(FLASH_BACKUP_SECTOR, sector, page, boolean) ? Switch_ON : Switch_OFF;

    switch(TextSize)
    {
        case TextSize_GUI_switch_16x8:  /*小号*/
        {
            OLED_SetPos(x,y);       //给定坐标
            for(int i=0;i<16;i++)   //先显示上半部分
                OLED_Dat(GUI_switch_16x8[switch_status][i]);
            break;
        }
        case TextSize_GUI_switch_22x16: /*大号*/
        {
            OLED_SetPos(x,y);       //给定坐标（上半部分）
            for(int i=0;i<22;i++)   //先显示上半部分
                OLED_Dat(GUI_switch_22x16[switch_status*2 + 0][i]);
            OLED_SetPos(x,y+1);     //给定坐标（下半部分）
            for(int i=0;i<22;i++)   //再显示下半部分
                OLED_Dat(GUI_switch_22x16[switch_status*2 + 1][i]);
            break;
        }
        default:
        {
            OLED_SetPos(x,y);       //给定坐标
            for(int i=0;i<16;i++)   //先显示上半部分
                OLED_Dat(GUI_switch_16x8[switch_status][i]);
            break;
        }
    }
    
    
    return switch_status;
}

/*************************************************
Function: ShowSwitch_with_Flash
Description:更改并保存开关变量的状态
Details：无
*************************************************/
uint8_t ShowSwitch_with_Flash(uint16_t x,uint16_t y, TextSize_TypeDef TextSize, uint16_t sector, uint16_t page)
{
                     
    uint8_t KeyNum = KEY_BLANK;         //键盘检测
    uint8_t switch_status = Switch_OFF;        //变量开关标志

    switch_status = flash_read(FLASH_BACKUP_SECTOR, sector, page, boolean) ? Switch_ON : Switch_OFF;


    while(KeyNum != KEY_ENTER && KeyNum != KEY_ENTER_Long)
    {
        /*******************************状态显示*******************************/
        switch(TextSize)
        {
            case TextSize_GUI_switch_16x8:  /*小号*/
            {
                OLED_SetPos(x,y);       //给定坐标
                for(int i=0;i<16;i++)   //先显示上半部分
                    OLED_Dat(GUI_switch_16x8[switch_status][i]);
                break;
            }
            case TextSize_GUI_switch_22x16: /*大号*/
            {
                OLED_SetPos(x,y);       //给定坐标（上半部分）
                for(int i=0;i<22;i++)   //先显示上半部分
                    OLED_Dat(GUI_switch_22x16[switch_status*2 + 0][i]);
                OLED_SetPos(x,y+1);     //给定坐标（下半部分）
                for(int i=0;i<22;i++)   //再显示下半部分
                    OLED_Dat(GUI_switch_22x16[switch_status*2 + 1][i]);
                break;
            }
            default:
            {
                OLED_SetPos(x,y);       //给定坐标
                for(int i=0;i<16;i++)   //先显示上半部分
                    OLED_Dat(GUI_switch_16x8[switch_status][i]);
                break;
            }
        }
        /*******************************按键识别*******************************/
        
        KeyNum = CH455_GetOneKey();
        
        /* 确认键 */
        if(KeyNum != KEY_ENTER && KeyNum != KEY_ENTER_Long)
        {
            switch_status = 1 - switch_status;
        }
        
    }/* 按键控制 */
    
    Flash_write_Word(FLASH_OLEDKBD_SECTOR, sector, page, switch_status);

    return switch_status;
}
