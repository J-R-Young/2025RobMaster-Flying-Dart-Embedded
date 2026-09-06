#include "Ctrl.h"

/* ============ 全局变量声明 ============= */
CtrlHandle_TypeDef hCtrl = {0};

int Mark_Zero_CMD = 0;	

int Dart_State = 0;

/* ============ 局部变量声明 ============= */


/* ============ 内部函数声明 ============= */


/* ============ 函数定义 ================= */

void CtrlHandle_Init(CtrlHandle_TypeDef *CtrlHandle)
{
    CtrlHandle->RobotID                 = RobotID_Blue_7_Sentinel;
    CtrlHandle->CtrlWay                 = CtrlWay_Safe;
    
    CtrlHandle->icmReadyFlag            = 0;
    
    CtrlHandle->Launch.Fric_Switch      = 0;

    CtrlHandle->Launch.Fric_Level       = Fric_Level1;
    
    CtrlHandle->UIreflashFLag           = 0;
    
    CtrlHandle->NavigationSwitch            = Navigation_OFF;
}

void CtrlHandle_Update(CtrlHandle_TypeDef *CtrlHandle)
{
	
	/*-------控制模式判断-------*/

	if (rc_ctrl.rc.ch[4] == LEVER_UP_VALUE)  						// 左上 安全模式
	{
		CtrlHandle->CtrlWay = CtrlWay_Safe;
	}
	else if (rc_ctrl.rc.ch[4] == LEVER_DOWN_VALUE)  				// 左下 非安全
	{		
		if (rc_ctrl.rc.ch[5] == LEVER_DOWN_VALUE && (DartCtrlHandle.Race_Mode == 0 || DartCtrlHandle.Race_Mode == 2 ||DartCtrlHandle.Race_Mode == 3 ))     				// 左中下 遥控模式
		{
			CtrlHandle->CtrlWay = CtrlWay_RC;  
		}
		else if (rc_ctrl.rc.ch[7] == LEVER_DOWN_VALUE && DartCtrlHandle.Race_Mode == 0)				// 右下 初始化
		{
			DartCtrlHandle.Reset_AutoHome = 1;
		}
		else if (DartCtrlHandle.Race_Mode == 1)
		{
			CtrlHandle->CtrlWay = CtrlWay_Copt;
		}
		else
		{
			CtrlHandle->CtrlWay = CtrlWay_Safe;
		}
		
	}
	else 
	{
		CtrlHandle->CtrlWay = CtrlWay_Safe;
	}
	
	if (DartCtrlHandle.Race_Mode == 2)
	{
		DartCtrlHandle.Fire_Detect = 0;
	}
		


    // 根据控制模式按照不同的条件改变各个标志位
    switch(CtrlHandle->CtrlWay)
    {
        case CtrlWay_Safe:  /* 安全模式 */
        {		
			
            break;
        }
        case CtrlWay_RC:    /* 遥控器模式 */   
        {
			
			break;
        }
        case CtrlWay_Copt:   
        {    
			
			break;
        }	
    }
    
}

void icmReadyChack(void)
{
    static uint16_t count = 0;
    static float pitch = 0.0;
    if(hCtrl.icmReadyFlag == 0)
    {
        count += 1;
        if((count & 0x200) == 0x200)
        {
            count = 0;
            
        }
    }
}
