#ifndef __OLEDKEYBOARD_H
#define __OLEDKEYBOARD_H

#include "stm32f4xx_hal.h"
#include "OLEDkeyboard_Config.h"



typedef struct 
{
    uint8_t     name[20];
    uint32_t    local_addr;
    uint32_t    flash_addr;
    /*此处添加用于显示的函数指针*/

}index_Typedef;

typedef enum
{
    MENU_LEVEL_0    = 0,
    MENU_LEVEL_1    = 1,
    MENU_LEVEL_2    = 2,
    MENU_LEVEL_3    = 3,
}MENU_LEVEL_Typedef;

typedef struct
{
    uint8_t                 name[20];
    MENU_LEVEL_Typedef      Level;
    
}MENU_Typedef;


/*===============外部变量===============*/
//extern OLED_Data_StructTypedef OLED_Data;
/*===============函数声明===============*/
void Oled_Input(void); //开机显示
void flash_load_all(void);
void OLED_display(void);


#endif /*  */

