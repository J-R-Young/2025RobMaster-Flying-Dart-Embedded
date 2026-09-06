#include "debugTask.h"
#define   huart_DBG  huart3

static void MasterPC_send(UART_HandleTypeDef *huart);
static void VOFA_JustFloat(UART_HandleTypeDef *huart);
int dbg[40] = {0};
uint8_t Navigation_UART_Disconnect = 0;
extern void Navigation_Upload(void);


void debugTask(void const * argument)
{
    portTickType currentTime;
	currentTime = xTaskGetTickCount();//当前系统时间
    
    while(1)
    {
		VOFA_JustFloat(&huart4);
        vTaskDelayUntil(&currentTime,2);
    }
    
}

extern int hcgskgcsgbck;

static void VOFA_JustFloat(UART_HandleTypeDef *huart)
{
    float data[VOFA_JustFloat_index] = {0.0};
    const uint8_t end[4] = {0x00, 0x00, 0x80, 0x7f};

	data[0] = DartCtrlHandle.Auto_Load_Run_Flag;
	data[1] = DartCtrlHandle.Lift_Position;
	data[2] = DartCtrlHandle.Load_Switch;
	data[3] = motor_data.Load.sumPosition;//DartCtrlHandle.Trigger_Detect;
	data[4] = motor_data.Load.positionTarget;//DartCtrlHandle.Fire_State;
	data[5] = DartCtrlHandle.Load_Num;//artCtrlHandle.Pull_Spring_Position;
	data[6] = DartCtrlHandle.Load_Finish;
	data[7] = DartCtrlHandle.Auto_Chambered_Count;//DartCtrlHandle.Load_Num;
	data[8] = DartCtrlHandle.Shoot_Num;//DartCtrlHandle.Fric_Left.SpdTarget;
	data[9] = DartCtrlHandle.Fire_State;//DartCtrlHandle.Press_In_After_Time;
	
    Usart_SendArr(huart, (uint8_t*)data, sizeof(float) * VOFA_JustFloat_index);

    Usart_SendArr(huart, (uint8_t*)end, 4);
}

static void MasterPC_send(UART_HandleTypeDef *huart)
{
    signed int SendData_Temp = 0;
    unsigned char SendData_Temp_High,SendData_Temp_Low;

    Usart_SendByte(huart,0xFF);
    Usart_SendByte(huart,0x00);

    //曲线1
//    SendData_Temp = (signed int)PID_body_Omega.pOut;
    SendData_Temp_High = SendData_Temp >> 8;
    SendData_Temp_Low = SendData_Temp & 0xff;
    Usart_SendByte(huart,SendData_Temp_Low);
    Usart_SendByte(huart,SendData_Temp_High);

    //曲线2
//    SendData_Temp = (signed int)PID_body_Omega.iOut;
    SendData_Temp_High = SendData_Temp >> 8;
    SendData_Temp_Low = SendData_Temp & 0xff;
    Usart_SendByte(huart,SendData_Temp_Low);
    Usart_SendByte(huart,SendData_Temp_High);

    //曲线3
//    SendData_Temp = (signed int)PID_body_Angle.pOut;
    SendData_Temp_High = SendData_Temp >> 8;
    SendData_Temp_Low = SendData_Temp & 0xff;
    Usart_SendByte(huart,SendData_Temp_Low);
    Usart_SendByte(huart,SendData_Temp_High);

    //曲线4
//    SendData_Temp = (signed int)PID_body_Angle.iOut;
    SendData_Temp_High = SendData_Temp >> 8;
    SendData_Temp_Low = SendData_Temp & 0xff;
    Usart_SendByte(huart,SendData_Temp_Low);
    Usart_SendByte(huart,SendData_Temp_High);

    //曲线5
//    SendData_Temp = (signed int)PID_body_Angle.dOut;
    SendData_Temp_High = SendData_Temp >> 8;
    SendData_Temp_Low = SendData_Temp & 0xff;
    Usart_SendByte(huart,SendData_Temp_Low);
    Usart_SendByte(huart,SendData_Temp_High);

    Usart_SendByte(huart,0xAA);
    Usart_SendByte(huart,0x55);
}