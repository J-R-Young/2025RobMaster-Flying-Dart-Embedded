#include "OLEDkeyboard.h"
#include "Simiic.h"

#define First_X                 45  //上次参数的X坐标           原值的显示位置
#define Second_X                85  //本次输入参数的X坐标       要设置的新值显示位置


uint8_t oled_display_option = 0;    //决定键显在运行时显示的内容

uint8_t *menu_1[6] = {"1.list1"    ,"3.list3"    ,"5.list5",
                      "2.list2"    ,"4.list4"    ,"6.list6"};

uint8_t *menu_2[12] = {"Switch1"  ,"Switch2"  ,"Switch3"  ,"Switch4",
                       "Switch5"  ,"Switch6"  ,"Switch7"  ,"Switch8",
                       "Switch9"  ,"Switch10" ,"Switch11" ,"Switch12"};

uint8_t *menu_3[12] = {"Vlaue 1"  ,"Vlaue 2"  ,"Vlaue 3"  ,"Vlaue 4",
                       "Vlaue 5"  ,"Vlaue 6"  ,"Vlaue 7"  ,"Vlaue 8",
                       "Vlaue 9"  ,"Vlaue 10" ,"Vlaue 11" ,"Vlaue 12"};

uint32_t test_arr_uint8[12]  = {0};
int16_t  test_arr_int16[12]  = {0};
uint32_t test_arr_uint32[12] = {0};
uint8_t  test_arr_switch[8]  = {0};


                       
///*************************************************************
//**Function: Oled_Input
//**Description: 键显主菜单显示
//**Others: 无
//************************************************************/
void Oled_Input(void) //开机显示
{
    uint16_t SectorNum = 0;     //初始化扇区
    uint16_t Flash_value = 0;
    uint16_t Oled_menu_choose = 0;
    
    CH455_Init();          //键盘扫描芯片初始化
    OLED_CLS();

    //首先进行数据救援
    Flash_Data_Recover();

	OLED_Show_Str(0,0,"TYUT",TextSize_F8x16);   //x、y为坐标（x为行，y为列），ch[]为字符，TextSize为字符尺寸
	OLED_Show_Str(0,2,"TEAM",TextSize_F8x16);
	OLED_Show_Str(0,4,"TRoMaC",TextSize_F8x16);	
	
    Oled_menu_choose = KeyboardInput(80, 6, TextSize_F8x16);    //屏幕右下角   获取要选择的选项序号
    OLED_CLS();
    
    if(Oled_menu_choose == 0)
    {
        return;
    }

    switch(Oled_menu_choose)
    {
        case 1:      //Menu 1
        {
            OLED_CLS();
            
            break;
        }
        
        case 2:      //Menu 2
        {
            uint8_t i = 0;

            /* PAGE 1*/
            for(i = 0; i < 4; i++)
            {
                OLED_Show_Str(0, 2*i, menu_2[i], TextSize_F6x8);                            //显示标题名
                ShowSwitch(90, 2*i, TextSize_GUI_switch_16x8, SectorNum, i);                //显示当前状态
            }
            test_arr_switch[0] =     ShowSwitch_with_Flash(95, 0, TextSize_GUI_switch_16x8, SectorNum, 0);
            test_arr_switch[1] =     ShowSwitch_with_Flash(95, 2, TextSize_GUI_switch_16x8, SectorNum, 1);
            test_arr_switch[2] =     ShowSwitch_with_Flash(95, 4, TextSize_GUI_switch_16x8, SectorNum, 2);
            test_arr_switch[3] =     ShowSwitch_with_Flash(95, 6, TextSize_GUI_switch_16x8, SectorNum, 3);
            OLED_CLS();
            
            /* PAGE 2*/
            for(i = 0; i < 4; i++)
            {
                OLED_Show_Str(0, 2*i, menu_2[i+4], TextSize_F6x8);                          //显示标题名
                ShowSwitch(90, 2*i, TextSize_GUI_switch_22x16, SectorNum, i+4);              //显示数值
            }
            test_arr_switch[4] =     ShowSwitch_with_Flash(95, 0, TextSize_GUI_switch_22x16, SectorNum, 4);
            test_arr_switch[5] =     ShowSwitch_with_Flash(95, 2, TextSize_GUI_switch_22x16, SectorNum, 5);
            test_arr_switch[6] =     ShowSwitch_with_Flash(95, 4, TextSize_GUI_switch_22x16, SectorNum, 6);
            test_arr_switch[7] =     ShowSwitch_with_Flash(95, 6, TextSize_GUI_switch_22x16, SectorNum, 7);
            OLED_CLS();
            
            flash_load_all();
            break;
        }
        
        case 3:
        {
            uint8_t i = 0;

            /* PAGE 1*/
            for(i = 0; i < 4; i++)
            {
                OLED_Show_Str(0, 2*i, menu_3[i], TextSize_F6x8);                        //显示标题名
                Flash_value = flash_read(FLASH_BACKUP_SECTOR, SectorNum, i, uint32_t);     //读取数值
                OLED_Show_Numbers(First_X, 2*i, Flash_value, TextSize_F6x8);            //显示数值
            }
            test_arr_uint8[0] =     KeyboardInput_with_Flash(Second_X, 0, TextSize_F6x8, SectorNum, 0);
            test_arr_uint8[1] =     KeyboardInput_with_Flash(Second_X, 2, TextSize_F6x8, SectorNum, 1);
            test_arr_uint8[2] =     KeyboardInput_with_Flash(Second_X, 4, TextSize_F6x8, SectorNum, 2);
            test_arr_uint8[3] =     KeyboardInput_with_Flash(Second_X, 6, TextSize_F6x8, SectorNum, 3);
            OLED_CLS();
            
            /* PAGE 2*/
            for(i = 0; i < 4; i++)
            {
                OLED_Show_Str(0, 2*i , menu_3[i+4], TextSize_F6x8);                     //显示标题名
                Flash_value = flash_read(FLASH_BACKUP_SECTOR, SectorNum, i+4, uint32_t);   //读取数值
                OLED_Show_Numbers(First_X, 2*i, Flash_value, TextSize_F6x8);            //显示数值
            }
            test_arr_uint8[4] =     KeyboardInput_with_Flash(Second_X, 0, TextSize_F6x8, SectorNum, 4);
            test_arr_uint8[5] =     KeyboardInput_with_Flash(Second_X, 2, TextSize_F6x8, SectorNum, 5);
            test_arr_uint8[6] =     KeyboardInput_with_Flash(Second_X, 4, TextSize_F6x8, SectorNum, 6);
            test_arr_uint8[7] =     KeyboardInput_with_Flash(Second_X, 6, TextSize_F6x8, SectorNum, 7);
            OLED_CLS();
            
            flash_load_all();
            break;
        }

        default:
        {
            flash_load_all();
            break;
        }
    }
}

void flash_load_all(void)
{
    /* MENU 1 */
    oled_display_option          = flash_read(FLASH_OLEDKBD_SECTOR, 0, 0, uint8_t);
    
    /* MENU 2 */
    test_arr_switch[0]           = flash_read(FLASH_OLEDKBD_SECTOR, 1, 0, boolean);
    test_arr_switch[1]           = flash_read(FLASH_OLEDKBD_SECTOR, 1, 1, boolean);
    test_arr_switch[2]           = flash_read(FLASH_OLEDKBD_SECTOR, 1, 2, boolean);
    test_arr_switch[3]           = flash_read(FLASH_OLEDKBD_SECTOR, 1, 3, boolean);
  
    test_arr_switch[4]           = flash_read(FLASH_OLEDKBD_SECTOR, 1, 4, boolean);
    test_arr_switch[5]           = flash_read(FLASH_OLEDKBD_SECTOR, 1, 5, boolean);
    test_arr_switch[6]           = flash_read(FLASH_OLEDKBD_SECTOR, 1, 6, boolean);
    test_arr_switch[7]           = flash_read(FLASH_OLEDKBD_SECTOR, 1, 7, boolean);
    
    
    /* MENU 3 */
    test_arr_uint8[0]            = flash_read(FLASH_OLEDKBD_SECTOR, 2, 0, uint32_t);
    test_arr_uint8[1]            = flash_read(FLASH_OLEDKBD_SECTOR, 2, 1, uint32_t);
    test_arr_uint8[2]            = flash_read(FLASH_OLEDKBD_SECTOR, 2, 2, uint32_t);
    test_arr_uint8[3]            = flash_read(FLASH_OLEDKBD_SECTOR, 2, 3, uint32_t);
   
    test_arr_uint8[4]            = flash_read(FLASH_OLEDKBD_SECTOR, 2, 4, uint32_t);
    test_arr_uint8[5]            = flash_read(FLASH_OLEDKBD_SECTOR, 2, 5, uint32_t);
    test_arr_uint8[6]            = flash_read(FLASH_OLEDKBD_SECTOR, 2, 6, uint32_t);
    test_arr_uint8[7]            = flash_read(FLASH_OLEDKBD_SECTOR, 2, 7, uint32_t);
    
    test_arr_uint8[8]            = flash_read(FLASH_OLEDKBD_SECTOR, 2, 8, uint32_t);
    test_arr_uint8[9]            = flash_read(FLASH_OLEDKBD_SECTOR, 2, 9, uint32_t);
    test_arr_uint8[10]           = flash_read(FLASH_OLEDKBD_SECTOR, 2, 10, uint32_t);
    test_arr_uint8[11]           = flash_read(FLASH_OLEDKBD_SECTOR, 2, 11, uint32_t);
    
    
    /* MENU N */
}


void OLED_display(void)
{
    switch(OLEDkeyboardHandle.keyboard.ActiveKey[0]/*oled_display_option*/)
    {
        case 1:     /*list1*/
        {
			OLED_CLS();
			
			OLED_Show_Str(0, 0, "Race", 2);
			OLED_Show_Str(0, 2, "Read", 2);
			OLED_Show_Str(0, 4, "Blue", 2);
				
			DartCtrlHandle.Race_Mode = (uint16_t)KeyboardInput(Second_X,0,TextSize_F8x16);
			DartCtrlHandle.Red = (uint16_t)KeyboardInput(Second_X,2,TextSize_F8x16);
			DartCtrlHandle.Blue	 = (uint16_t)KeyboardInput(Second_X,4,TextSize_F8x16);
		
			OLED_CLS();
		
			// 清空选择有效值
			OLEDkeyboardHandle.keyboard.ActiveKey[0] = 0;
			// 清空选择有效值
			OLEDkeyboardHandle.keyboard.ActiveKey[0] = 0;
			
            break;
        }
		
		case 2:
		{
			OLED_CLS();
			
			OLED_Show_Str(0, 0, "turn0", 2);
			OLED_Show_Str(0, 2, "turn1", 2);
			OLED_Show_Str(0, 4, "turn2", 2);
			OLED_Show_Str(0, 6, "turn3", 2);
				
			DartCtrlHandle.Screw_Target_Num[0] = 0.1 * (uint16_t)KeyboardInput(Second_X,0,TextSize_F8x16);
			DartCtrlHandle.Screw_Target_Num[1] = 0.1 *  (uint16_t)KeyboardInput(Second_X,2,TextSize_F8x16);
			DartCtrlHandle.Screw_Target_Num[2] = 0.1 * (uint16_t)KeyboardInput(Second_X,4,TextSize_F8x16);
			DartCtrlHandle.Screw_Target_Num[3] = 0.1 * (uint16_t)KeyboardInput(Second_X,6,TextSize_F8x16);	
		
			
			OLED_CLS();
		

			// 清空选择有效值
			OLEDkeyboardHandle.keyboard.ActiveKey[0] = 0;
			
			break;
		
		}
		
		case 3:
		{
			OLED_CLS();

			OLED_Show_Str(0, 0, "Offset0", 2);
			OLED_Show_Str(0, 2, "Offset1", 2);
			OLED_Show_Str(0, 4, "Offset2", 2);
			OLED_Show_Str(0, 6, "Offset3", 2);

			DartCtrlHandle.Offset[0] = (uint16_t)KeyboardInput(Second_X,0,TextSize_F8x16);
			DartCtrlHandle.Offset[1] = (uint16_t)KeyboardInput(Second_X,2,TextSize_F8x16);
			DartCtrlHandle.Offset[2] = (uint16_t)KeyboardInput(Second_X,4,TextSize_F8x16);
			DartCtrlHandle.Offset[3] = (uint16_t)KeyboardInput(Second_X,6,TextSize_F8x16);	

			OLED_CLS();

			// 清空选择有效值
			OLEDkeyboardHandle.keyboard.ActiveKey[0] = 0;
			
			break;

		}
        default:
        {
//            OLED_CLS();
						
			OLED_Show_Str(0, 0, "60t", 2);
			OLED_Show_Str(0, 2, "20s", 2);
			OLED_Show_Str(0, 4, "err", 2);
			
			OLED_Show_float(24, 0, motor_data.Yaw.turns , 3, TextSize_F8x16);
			OLED_Show_float(24, 2, DartCtrlHandle.screw_2006_num , 3, TextSize_F8x16);
			OLED_Show_float(24, 4, VisionData.x_pixel_err, 3, TextSize_F8x16);	
			OLED_Show_Numbers_2(95, 0, Dart_Client_Cmd.dart_launch_opening_status, TextSize_F8x16);
            break;
        }
    
    }
}



void OLEDkeyboardTask(void const * argument)
{
    portTickType currentTime;
	currentTime = xTaskGetTickCount();//当前系统时间
    
    while(1)
    {
        OLEDkeyboard_KeyUpdate();
        
        OLED_display();
		
        vTaskDelayUntil(&currentTime,OLEDkeyboardHandle.keyboard.Period);
    } 
}

