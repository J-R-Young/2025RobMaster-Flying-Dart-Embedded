#include "DeviceCheck.h"

DeviceCheckHandle_TypeDef DeviceList[Device_CheckNum] = {DeviceID_NULL};

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
)
{
    Device_HandleInit(&DeviceList[0], DeviceID_1);
    Device_HandleInit(&DeviceList[1], DeviceID_2);
    Device_HandleInit(&DeviceList[2], DeviceID_3);
    Device_HandleInit(&DeviceList[3], DeviceID_4);
    Device_HandleInit(&DeviceList[4], DeviceID_5);
    Device_HandleInit(&DeviceList[5], DeviceID_6);
    Device_HandleInit(&DeviceList[6], DeviceID_7);
    Device_HandleInit(&DeviceList[7], DeviceID_8);
    Device_HandleInit(&DeviceList[8], DeviceID_9);
    Device_HandleInit(&DeviceList[9], DeviceID_10);
    Device_HandleInit(&DeviceList[10], DeviceID_11);
    Device_HandleInit(&DeviceList[11], DeviceID_12);

}


void Device_HandleInit(DeviceCheckHandle_TypeDef *DeviceCheckHandle, uint32_t DeviceID)
{
    if(DeviceCheckHandle == NULL)
    {
        return;
    }
    
    DeviceCheckHandle->DeviceID     = DeviceID;
    DeviceCheckHandle->DeviceState  = DeviceState_Offline;
    DeviceCheckHandle->HealthValue  = Device_HealthDefault;
}

void Device_OnlineCheck_ALL(void)
{
    for(uint8_t i = 0; i < Device_CheckNum; i++)
    {
        if(DeviceList[i].DeviceID == DeviceID_NULL)
        {
            continue;
        }
        
        DeviceList[i].HealthValue = DeviceList[i].HealthValue - 2 <= 0 ? -1 : DeviceList[i].HealthValue - 2;
        
        if(DeviceList[i].HealthValue == -1)
        {
            DeviceList[i].DeviceState = DeviceState_Offline;
        }
        else
        {
            DeviceList[i].DeviceState = DeviceState_Online;
        }
    }
}

void Device_Update(DeviceID_enum DeviceID)
{
    if(DeviceID == DeviceID_NULL)
    {
        return;
    }
    
    for(uint8_t i = 0; i < Device_CheckNum; i++)
    {
        if(DeviceList[i].DeviceID == DeviceID)
        {
            DeviceList[i].HealthValue += 50;
            
            DeviceList[i].HealthValue = DeviceList[i].HealthValue > Device_HealthMax ? Device_HealthMax : DeviceList[i].HealthValue;
            
            return;
        }
    }
}

DeviceState_enum Device_GetState(DeviceID_enum DeviceID)
{
     if(DeviceID == DeviceID_NULL)
    {
        return DeviceState_Offline;
    }
    
    for(uint8_t i = 0; i < Device_CheckNum; i++)
    {
        if(DeviceList[i].DeviceID == DeviceID)
        {
            return DeviceList[i].DeviceState;
        }
    }
}