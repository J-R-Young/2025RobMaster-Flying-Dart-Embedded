#include "ChassisTask.h"

status_data_t status_data;
extern float Move_TriggerCtrl_SpeedCtrl(void);


void ControlTask(void const * argument)
{
    portTickType currentTime;
	currentTime = xTaskGetTickCount();//当前系统时间
    
    osDelay(10);
    uint8_t CanSentCount = 0;
	
	int close_left = 0;      //3508链条开关
	int close_right = 0;
	
	 HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	
	Dart_AutoHome();
	
	
    while(1)
    {	
        CanSentCount += 1;
        
        if(CanSentCount > 2)
        {
            CanSentCount = 0;
		}
		
		DartCtrlHandle.ShootTarget =  (Dart_info.dart_info >> 6) & 0X03;
		
		Usart_SendByte(&huart1,DartCtrlHandle.ShootTarget); //和自瞄通信  1：固定靶 2：随机固定靶  3：随机移动靶
		
		Dart_Control();   //飞镖控制
		
        //以下为最终发送调用处
		CAN_TX_CAN1_0x200(DartCtrlHandle.Move_Trigger.PidOut, DartCtrlHandle.Pull_Spring_Left.PidOut, DartCtrlHandle.Pull_Spring_Right.PidOut, 0);
		
		CAN_TX_CAN2_0x200(DartCtrlHandle.Small_Fric.PidOut, DartCtrlHandle.Fric_Left.PidOut, DartCtrlHandle.Fric_Right.PidOut, DartCtrlHandle.Lift.PidOut);

		CAN_TX_CAN2_0x1FF(DartCtrlHandle.Load.PidOut, 0, 0, 0);
		
		CAN_TX_CAN2_0x2FF(0, DartCtrlHandle.Yaw.PidOut, 0, 0);
		
        vTaskDelayUntil(&currentTime,2);
    }
    
    
    
    
}/* ChassisTask() */






