/*************************************************
Copyright (C), 2016-2023, TYUT JBD TRoMaC
File name: 
Author: 
Version:               
Date: 
Description:  
Others:      
Function List:
History:
<author>    <time>          <version>       <desc>
**************************************************/
#ifndef __DeviceCheck_h
#define __DeviceCheck_h

#include "headfiles.h"

/* =========== 宏定义 ================ */

#define Device_HealthMax        300
#define Device_HealthDefault    200
#define Device_CheckNum         13

typedef enum
{
    // 空设备
    DeviceID_NULL                   = 0x000,
    // 接收机
    DeviceID_DR16                   = 0x001,
    
    // 视觉串口连接
    DeviceID_VisionNUC              ,
    
    // 陀螺仪
    DeviceID_ICM20602               ,
    
    // 电管user口
    DeviceID_Referee                ,
    
    // Can通信设备
    DeviceID_Lift            = 0x100,
    DeviceID_Yaw              		,
    DeviceID_Small_Fric             ,
    DeviceID_Pull_Spring_Left       ,
    DeviceID_Pull_Spring_Right      ,
    DeviceID_Move_Trigger      		,
    DeviceID_Fric_Left         	 	,
    DeviceID_Fric_Right           	,
    DeviceID_Load           		,
    
    DeviceID_freeRTOS               = 0xE00,
    
                      
}DeviceID_enum;

typedef enum
{
    DeviceState_Offline             = 0,
    DeviceState_Online              = 1,
    
}DeviceState_enum;

typedef struct
{
    DeviceID_enum                   DeviceID;
    DeviceState_enum                DeviceState;
    
    int16_t                         HealthValue;    // 设备健康值
}DeviceCheckHandle_TypeDef;

/* =========== 全局变量声明 ========== */
extern DeviceCheckHandle_TypeDef DeviceList[Device_CheckNum];

/* =========== 函数声明 ============== */
void Device_MonitoringInit(
    uint32_t DeviceID_1 ,
    uint32_t DeviceID_2 ,
    uint32_t DeviceID_3 ,
    uint32_t DeviceID_4 ,
    uint32_t DeviceID_5 ,
    uint32_t DeviceID_6 ,
    uint32_t DeviceID_7 ,
    uint32_t DeviceID_8 ,
    uint32_t DeviceID_9 ,
    uint32_t DeviceID_10,
    uint32_t DeviceID_11,
    uint32_t DeviceID_12
);
void Device_OnlineCheck_ALL(void);

void Device_HandleInit(DeviceCheckHandle_TypeDef *DeviceCheckHandle, uint32_t DeviceID);
void Device_Update(DeviceID_enum DeviceID);
DeviceState_enum Device_GetState(DeviceID_enum DeviceID);

#endif /* __DeviceCheck_h */
