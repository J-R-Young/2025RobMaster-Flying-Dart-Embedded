#ifndef __bsp_iic_H
#define __bsp_iic_H

//#include "bsp_tim.h"							

#include "stm32f4xx_hal.h"

typedef struct
{
	GPIO_TypeDef*  SDA_GPIO;
	uint32_t       SDA_PIN;
	GPIO_TypeDef*  SCL_GPIO;
	uint32_t       SCL_PIN;
}IIC_HandleTypeDef;

/*IIC驱动核心代码*/
void IIC_Init(IIC_HandleTypeDef* iic);
void SDA_Out(IIC_HandleTypeDef* iic);
void SDA_In(IIC_HandleTypeDef* iic);
void IIC_Start(IIC_HandleTypeDef* iic);
void IIC_Stop(IIC_HandleTypeDef* iic);
void IIC_Send_Byte(IIC_HandleTypeDef* iic,uint8_t data);
uint8_t IIC_Read_Byte(IIC_HandleTypeDef* iic,uint8_t ack);
uint8_t IIC_Wait_Ack(IIC_HandleTypeDef* iic);
void IIC_ack(IIC_HandleTypeDef* iic,uint8_t ack);
/*不允许更改*/
uint8_t IIC_WriteAddr(IIC_HandleTypeDef* iic, uint8_t dev_addr, uint8_t reg_addr);
uint8_t IIC_Read_Len(IIC_HandleTypeDef* iic,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
void DELAY_us(uint32_t us);
#endif 
