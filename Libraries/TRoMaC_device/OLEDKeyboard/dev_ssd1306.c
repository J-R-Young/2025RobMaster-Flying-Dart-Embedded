#include "dev_ssd1306.h"

//uint8_t_t guIMG_Buffer[64][128] = {0};
//uint8_t_t guIMG_display[8][128] = {0};

/*************************************************
Function: OLED_Cmd
Description:IIC写命令
Details：写命令首先传输（0x78）8位地址码，然后传输
        （0x00）8位命令信号，然后传输OLED_Cmd8位命
         令，具体开始、应答、结束信号产生时刻见如
         下程序
*************************************************/
void OLED_Cmd(uint8_t cmd)
{
    Simiic_Start(&Simiic_OLEDkeyboard);
    
    Simiic_Send_Byte(&Simiic_OLEDkeyboard, Simiic_Addr_W(0x78U));
    Simiic_WaitACK(&Simiic_OLEDkeyboard);
    
    Simiic_Send_Byte(&Simiic_OLEDkeyboard, 0x00U);
    Simiic_WaitACK(&Simiic_OLEDkeyboard);
    
    Simiic_Send_Byte(&Simiic_OLEDkeyboard, cmd);
    Simiic_WaitACK(&Simiic_OLEDkeyboard);
    
    Simiic_Stop(&Simiic_OLEDkeyboard);
    
    
//    simi2c_Start();
//    simi2c_Write_OneByte(0x78);    
//    simi2c_Wait_Ask();
//    simi2c_Write_OneByte(0x00);                            //0x00=0000 0000
//    simi2c_Wait_Ask();
//    simi2c_Write_OneByte(cmd);
//    simi2c_Wait_Ask();
//    simi2c_Stop();
}

/*************************************************
Function: OLED_Dat
Description:IIC写数据
Details：写命令首先传输（0x78）8位地址码，然后传输
        （0x40）8位数据信号，然后传输OLED_Cmd8位命
         令，具体开始、应答、结束信号产生时刻见如
         下程序
*************************************************/
void OLED_Dat(uint8_t data)
{
    Simiic_Start(&Simiic_OLEDkeyboard);
    
    Simiic_Send_Byte(&Simiic_OLEDkeyboard, Simiic_Addr_W(0x78U));
    Simiic_WaitACK(&Simiic_OLEDkeyboard);
    
    Simiic_Send_Byte(&Simiic_OLEDkeyboard, 0x40U);
    Simiic_WaitACK(&Simiic_OLEDkeyboard);
    
    Simiic_Send_Byte(&Simiic_OLEDkeyboard, data);
    Simiic_WaitACK(&Simiic_OLEDkeyboard);
    
    Simiic_Stop(&Simiic_OLEDkeyboard);
    
    
//    simi2c_Start();
//    simi2c_Write_OneByte(0x78);                            //OLED从器件地址
//    simi2c_Wait_Ask();
//    simi2c_Write_OneByte(0x40);                            //器件内单元地址  0x40=0100 0000
//    simi2c_Wait_Ask();
//    simi2c_Write_OneByte(data);                        //将数据写入指定存储单元
//    simi2c_Wait_Ask();
//    simi2c_Stop();
}
/*************************************************
Function: OLED_Init
Description:OLED初始化
Details：
*************************************************/
void OLED_Init(void)
{
    OLED_Cmd(0xae); //--turn off oled panel
    OLED_Cmd(0x00); //--set low column address
    OLED_Cmd(0x10); //--set high column address
    OLED_Cmd(0x40); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_Cmd(0x81); //--set contrast control register
    OLED_Cmd(0xcf); //--Set SEG Output Current Brightness 0xcf
    OLED_Cmd(0xa0); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_Cmd(0xc0); //--Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_Cmd(0xa6); //--set normal display
    OLED_Cmd(0xa8); //--set multiplex ratio(1 to 64)
    OLED_Cmd(0x3f); //--1/64 duty
    OLED_Cmd(0xd3); //--set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    OLED_Cmd(0x00); //--not offset
    OLED_Cmd(0xd5); //--set display clock divide ratio/oscillator frequency
    OLED_Cmd(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_Cmd(0xd9); //--set pre-charge period
    OLED_Cmd(0xf1); //--Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_Cmd(0xda); //--set com pins hardware configuration
    OLED_Cmd(0x12);
    OLED_Cmd(0xdb); //--set vcomh
    OLED_Cmd(0x40); //--Set VCOM Deselect Level
    OLED_Cmd(0x20); //--Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_Cmd(0x02); 
    OLED_Cmd(0x8d); //--set Charge Pump enable/disable
    OLED_Cmd(0x14); //--set(0x10) disable
    OLED_Cmd(0xa4); //--Disable Entire Display On (0xa4/0xa5)
    OLED_Cmd(0xa6); //--Disable Inverse Display On (0xa6/a7)
    OLED_Cmd(0xaf); //--turn on oled panel
}
/*************************************************
Function: OLED_SetPos
Description:定位一个坐标点
Details：定位一个坐标点作为显示一个字符最左上方像素
         点的坐标，x为横坐标（0-127）,y由上至下为纵
         坐标（0-7），每一纵坐标对应8个纵向分辨率。
*************************************************/
void OLED_SetPos(unsigned char x,unsigned char y)
{
    OLED_Cmd(0xb0+y);
    OLED_Cmd(((x&0xf0)>>4)|0x10);
    OLED_Cmd((x&0x0f)|0x01);
}

/*************************************************
Function: OLED_Fill
Description:填充数据
Detail:将整个OLED屏上的128*64个像素点填满fill_Data
       这个数据
*************************************************/
void OLED_Fill(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        OLED_Cmd(0xb0+m);
        OLED_Cmd(0x00);
        OLED_Cmd(0x10);
        for(n=0;n<128;n++)
        {
            OLED_Dat(fill_Data);
        }
    }
}


/**************************************************
Function: OLED_Show_Char
Description:OLED显示一个字符
Details:x、y为坐标，str为要输出的字符，size为字号
**************************************************/
void OLED_Show_Char(uint16_t x, uint16_t y, char str, TextSize_TypeDef TextSize)
{
    switch(TextSize)
    {
        case TextSize_F6x8 :
        {
            if(str != '\0')
            {
                str -= 32;
                OLED_SetPos(x,y);
                for(uint8_t i = 0; i < 6; i++)
                {
                    if(x + i >= 127)    return; //超出屏幕范围的部分不再显示
                    OLED_Dat(F6x8[str][i]);
                }
            }
            break;
        }

        case TextSize_F8x16 :
        {
            if(str != '\0')
            {
                str -= 32;  //转换到F8X16[]中对应的行数

                OLED_SetPos(x,y);//给定坐标（上半部分）
                for(uint8_t i = 0; i < 8; i++)//先显示上半部分
                {
                    if(x + i >= 127)    break; //超出屏幕范围的部分不再显示
                    OLED_Dat(F8x16[str*16+i]);
                }

                OLED_SetPos(x,y+1);//给定坐标（下半部分）
                for(uint8_t i = 0; i < 8; i++)//再显示下半部分
                {
                    if(x + i >= 127)    return; //超出屏幕范围的部分不再显示
                    OLED_Dat(F8x16[str*16+i+8]);
                }
            }
            break;
        }
        default :
        {
            if(str != '\0')
            {
                str -= 32;
                OLED_SetPos(x,y);
                for(uint8_t i = 0; i < 6; i++)
                {
                    if(x + i >= 127)    return; //超出屏幕范围的部分不再显示
                    OLED_Dat(F6x8[str][i]);
                }
            }
            break;
        }
    }/*switch(TextSize)*/
}


/**************************************************
Function: OLED_ShowStr
Description:OLED显示一串字符
Details:x、y为坐标（x为行，y为列），ch[]为字符，TextSize为字符尺寸
        仅使用2号即8X16尺寸。
**************************************************/
void OLED_Show_Str(uint16_t x, uint16_t y, uint8_t str[], TextSize_TypeDef TextSize)
{
    uint16_t i=0;

    while(str[i] !='\0')
    {
        /* 二选一 */
        /*到行末停止输出*/
//        if(x > 127) break;
        /*到行末换行*/
        if(x > 127)
        {
            x = 0;

            if(   (y == 7 && TextSize == TextSize_F6x8)
               || (y >= 5 && TextSize == TextSize_F8x16)
              )
            {
                break;
            }
            else
            {
                TextSize == TextSize_F6x8 ? (y += 1) : (y += 2);
            }
        }

        OLED_Show_Char(x, y, str[i], TextSize);
        switch(TextSize)
        {
            case TextSize_F6x8:  {x += 6; break;}
            case TextSize_F8x16: {x += 8; break;}
        }
        i++;
    }/*while(str[i] !='\0')*/
}

/**************************************************
Function: OLED_Show_Str_Count
Description:OLED显示一串字符,显示的数量可以指定
Details:x、y为坐标（x为行，y为列），ch[]为字符，TextSize为字符尺寸
        仅使用2号即8X16尺寸。
**************************************************/
void OLED_Show_Str_Count(uint16_t x, uint16_t y, uint8_t *str, uint16_t leng, TextSize_TypeDef TextSize)
{
    uint16_t i = 0;
    while(i < leng)
    {
        // 遇'\r' '\n' "\r\n"执行换行
        if(str[i] == '\r' || str[i] == '\n')
        {
            if(str[i] == '\r' && str[i+1] == '\n')
            {
                if(   (TextSize == TextSize_F6x8  && y != 7)
                   || (TextSize == TextSize_F8x16 && y != 6)
                  )
                {
                    x = 0;
                    y += TextSize;
                    i += 2;
                }
                else// 最后一行执行换行则停止打印
                {
                    break;
                }

            }
            else
            {
                if(   (TextSize == TextSize_F6x8  && y != 7)   /* 小号字显示不下 */
                   || (TextSize == TextSize_F8x16 && y != 6)   /* 大号字显示不下 */
                  )
                {
                    x = 0;
                    y += TextSize;
                    i += 1;
                }
                else// 最后一行执行换行则停止打印
                {
                    break;
                }
            }
        }

        /*到行末换行*/
        if(   (TextSize == TextSize_F6x8  && (127 - x) < 6 && y != 7)   /* 小号字显示不下 */
           || (TextSize == TextSize_F8x16 && (127 - x) < 8 && y != 6)   /* 大号字显示不下 */
          )
        {
            x = 0;
            y += TextSize;
        }

        OLED_Show_Char(x, y, str[i], TextSize);
        switch(TextSize)
        {
            case TextSize_F6x8:  {x += 6; break;}
            case TextSize_F8x16: {x += 8; break;}
        }
        i++;
    }/*while(i < leng)*/
}


/**************************************************
Function: OLED_Show_OneNumber
Description:OLED显示一个数字
Details:x、y为坐标，num为数字
**************************************************/
void OLED_Show_OneNumber(uint16_t x, uint16_t y, uint8_t num, TextSize_TypeDef TextSize)
{
    num = (num < 0) ? 0 : (num > 9 ? 9 : num);
    num = '0' + num;    //为跳过前15行字符显示
    OLED_Show_Char(x, y, num, TextSize);
}


/**************************************************
Function: OLED_Show_Numbers
Description:OLED显示一串数字 一个符号位加5或4个数字位    大号字体显示四位数，小号字体显示五位
Details:x、y为坐标，Value为数字
**************************************************/
void OLED_Show_Numbers(uint16_t x, uint16_t y,int32_t Value, TextSize_TypeDef TextSize)
{
    uint8_t str[7]={0};

    if(Value >= 0)                              //正数
    {
        str[0] = ' ';
    }
    else                                        //负数
    {
        str[0] = '-';
    }

    Value = Value >= 0 ? Value : -Value;

    switch(TextSize)
    {
        case TextSize_F6x8:
        {
            str[1] = num2ascll((Value % 100000) / 10000);   //万位
            str[2] = num2ascll((Value % 10000) / 1000);     //千位
            str[3] = num2ascll((Value % 1000) / 100);       //百位
            str[4] = num2ascll((Value % 100) / 10);         //百位
            str[5] = num2ascll(Value % 10);                 //个位
            str[6] = '\0';
            break;
        }
        case TextSize_F8x16:
        {
            str[1] = num2ascll((Value % 10000) / 1000);     //千位
            str[2] = num2ascll((Value % 1000) / 100);       //百位
            str[3] = num2ascll((Value % 100) / 10);         //百位
            str[4] = num2ascll(Value % 10);                 //个位
            str[5] = '\0';
            break;
        }
        default:
        {
            str[1] = num2ascll((Value % 100000) / 10000);   //万位
            str[2] = num2ascll((Value % 10000) / 1000);     //千位
            str[3] = num2ascll((Value % 1000) / 100);       //百位
            str[4] = num2ascll((Value % 100) / 10);         //百位
            str[5] = num2ascll(Value % 10);                 //个位
            str[6] = '\0';
            break;
        }
    }

    OLED_Show_Str(x, y, str, TextSize);
}

/**************************************************
Function: OLED_Show_Numbers_2
Description:OLED显示一串数字 一个符号位加5或4个数字位    大号字体显示四位数，小号字体显示五位
Details:x、y为坐标，Value为数字
**************************************************/
void OLED_Show_Numbers_2(uint16_t x, uint16_t y,int32_t Value, TextSize_TypeDef TextSize)
{
    uint8_t str[5]={0};

    if(Value >= 0)                              //正数
    {
        str[0] = ' ';
    }
    else                                        //负数
    {
        str[0] = '-';
    }

    Value = Value >= 0 ? Value : -Value;

    switch(TextSize)
    {
//        case TextSize_F6x8:
//        {
//            str[1] = num2ascll((Value % 100000) / 10000);   //万位
//            str[2] = num2ascll((Value % 10000) / 1000);     //千位
//            str[3] = num2ascll((Value % 1000) / 100);       //百位
//            str[4] = num2ascll((Value % 100) / 10);         //百位
//            str[5] = num2ascll(Value % 10);                 //个位
//            str[6] = '\0';
//            break;
//        }
        case TextSize_F8x16:
        {
//            str[1] = num2ascll((Value % 10000) / 1000);     //千位
//            str[2] = num2ascll((Value % 1000) / 100);       //百位
            str[1] = num2ascll((Value % 100) / 10);         //十位
            str[2] = num2ascll(Value % 10);                 //个位
            str[3] = '\0';
            break;
        }
//        default:
//        {
//            str[1] = num2ascll((Value % 100000) / 10000);   //万位
//            str[2] = num2ascll((Value % 10000) / 1000);     //千位
//            str[3] = num2ascll((Value % 1000) / 100);       //百位
//            str[4] = num2ascll((Value % 100) / 10);         //百位
//            str[5] = num2ascll(Value % 10);                 //个位
//            str[6] = '\0';
//            break;
//        }
    }

    OLED_Show_Str(x, y, str, TextSize);
}

/**************************************************
Function: OLED_Show_float
Description:OLED显示浮点数
Details:    x、y为坐标（x为行，y为列）
            dat为要输出的浮点数(float 或 double)
            整数部分默认4位
            pointnum为小数部分位数     最多六位
            TextSize为字符尺寸

note:   特别注意当发现小数部分显示的值与你写入的值不一样的时候，
        可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
        有关问题的详情，请自行百度学习   浮点数精度丢失问题。
        负数会显示一个 ‘-’号   正数显示一个空格
**************************************************/
void OLED_Show_float(uint16_t x, uint16_t y, double dat, uint8_t pointnum, TextSize_TypeDef TextSize)
{
    uint32_t NUM[2];
    uint64_t tmp;

	uint8_t str[] = "-00.000000";

    NUM[0] = (uint32_t)abs(dat);
    tmp    = ABS(dat * 1000000);
    NUM[1] = tmp % 1000000;

    str[0] = SignOf(dat) == 1 ? ' ' : '-';
    str[1] = num2ascll((NUM[0] % 100) / 10);                //百位
    str[2] = num2ascll( NUM[0] % 10);                       //个位

    str[4]  = num2ascll((NUM[1] % 1000000) / 100000);       //十分位
    str[5]  = num2ascll((NUM[1] % 100000) / 10000);         //百分位
    str[6]  = num2ascll((NUM[1] % 10000) / 1000);           //千分位
    str[7] = num2ascll((NUM[1] % 1000) / 100);             //万分位
    str[8] = num2ascll((NUM[1] % 100) / 10);               //十万分位
    str[9] = num2ascll( NUM[1] % 10);                      //百万分位


    pointnum = pointnum > 6 ? 6 : pointnum; 

    str[4+pointnum] = '\0';
    OLED_Show_Str(x, y, str, TextSize);
}

/**************************************************
Function: OLED_Show_hex
Description:OLED显示浮点数
Details:x、y为坐标，Value为数字
**************************************************/
void OLED_Show_hex(uint16_t x, uint16_t y, uint16_t dat, TextSize_TypeDef TextSize)
{
    static const uint8_t dec2hex[] = "0123456789ABCDEF";
    uint8_t str[] = "0xHHHH";

    for(uint8_t i = 0; i < 4; i++)
    {
        str[2+i] = dec2hex[(dat & (0x0F << (4 * (3-i))) ) >> (4*(3-i))];
    }

    OLED_Show_Str(x, y, str, TextSize);
}


void OLED_test(void)
{
    OLED_CLS();
    OLED_Show_Str(47, 0, "(>_<)", TextSize_F8x16);
    OLED_Show_Str(0, 5, "Have a nice day!", TextSize_F8x16);
}
