/*************************************************
Copyright (C), 2016-2023, TYUT JBD TRoMaC
File name: 
Author: Li_Jiang
Version: 0.1        
Date: 2023-6-25
Description:  直接操作寄存器
Others:      
Function List:
History:
<author>    <time>          <version>       <desc>
**************************************************/
#ifndef __simiic_h
#define __simiic_h

#include "stm32f4xx_hal.h"
#include "tim.h"

/* =========== 宏定义 ================ */
#define Simiic_Delay_us(us)         Simiic_Delay_ns(us * 100)

//#define Simiic_SDA_In(SimiicHandle)     {
//                                            
//                                        }

#define Simiic_SDA_Get(SimiicHandle)    ((SimiicHandle->SDA_GPIO_Port->IDR & (SimiicHandle->SDA_GPIO_Pin)) == (SimiicHandle->SDA_GPIO_Pin) ? 1 : 0)

#define Simiic_SDA_H(SimiicHandle)      SimiicHandle->SDA_GPIO_Port->ODR |=  (SimiicHandle->SDA_GPIO_Pin)
#define Simiic_SDA_L(SimiicHandle)      SimiicHandle->SDA_GPIO_Port->ODR &= ~(SimiicHandle->SDA_GPIO_Pin)

#define Simiic_SCL_H(SimiicHandle)      SimiicHandle->SCL_GPIO_Port->ODR |=  (SimiicHandle->SCL_GPIO_Pin)
#define Simiic_SCL_L(SimiicHandle)      SimiicHandle->SCL_GPIO_Port->ODR &= ~(SimiicHandle->SCL_GPIO_Pin)

#define Simiic_Addr_W(addr)             ((uint8_t)addr & ~(0x01U))
#define Simiic_Addr_R(addr)             ((uint8_t)addr |  (0x01U))

typedef enum
{
    BitOrder_MSB       = 0,    // 高位先行
    BitOrder_LSB       ,       // 低位先行
}BitOrder_enum;

typedef enum
{
    SimiicAck_ACK       = 0,    // 应答
    SimiicAck_NACK      = 1,    // 非应答
}SimiicAck_enum;

typedef struct
{
	GPIO_TypeDef*       SDA_GPIO_Port;
	uint32_t            SDA_GPIO_Pin;
	GPIO_TypeDef*       SCL_GPIO_Port;
	uint32_t            SCL_GPIO_Pin;
    
    uint32_t            Freq;           // simiic的通信频率 决定延时大小 
    float               Period;         // 时钟信号的周期
    uint8_t             BitOrder;       // simiic的比特序 MSB|LSB
}SimiicHandle_TypeDef;

/* =========== 全局变量声明 ========== */
//extern SimiicHandle_TypeDef IICBus0;

/* =========== 函数声明 ============== */
void Simiic_Delay_ns(uint32_t ns);

void Simiic_HandleInit(SimiicHandle_TypeDef *SimiicHandle,
                       GPIO_TypeDef*        SDA_GPIO_Port, uint32_t             SDA_GPIO_Pin,
                       GPIO_TypeDef*        SCL_GPIO_Port, uint32_t             SCL_GPIO_Pin,
                       uint32_t             Freq         , uint8_t              BitOrder);
void Simiic_HandleMspInit(SimiicHandle_TypeDef *SimiicHandle);

void Simiic_SDA_In(SimiicHandle_TypeDef *SimiicHandle);
void Simiic_SDA_Out(SimiicHandle_TypeDef *SimiicHandle);

void Simiic_Start(SimiicHandle_TypeDef *SimiicHandle);
void Simiic_Stop(SimiicHandle_TypeDef *SimiicHandle);
uint8_t Simiic_WaitACK(SimiicHandle_TypeDef *SimiicHandle);
void Simiic_Send_Ack(SimiicHandle_TypeDef *SimiicHandle, SimiicAck_enum ack);
void Simiic_Send_Byte(SimiicHandle_TypeDef *SimiicHandle, uint8_t Data);
uint8_t Simiic_Read_Byte(SimiicHandle_TypeDef *SimiicHandle, uint8_t ack);



#endif /* __simiic_h */


