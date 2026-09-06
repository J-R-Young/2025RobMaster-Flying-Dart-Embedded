#include "bsp_iic.h"
#include "tim.h"
/*
* @author  路少鹏
* @data    2020.1.13
* @brief   兼容hal库的软件IIC 需要提供DELAY_us()函数
					 实测396kHZ,修复上次通信bug
*/
/*IIC驱动代码，无需更改*/


void DELAY_us(uint32_t us)  //最大780us
{
	//delay_us(us);
	htim10.Instance->CNT=0;
	while(htim10.Instance->CNT<us);
}

void IIC_Init(IIC_HandleTypeDef* iic)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = iic->SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(iic->SCL_GPIO, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = iic->SDA_PIN;
	HAL_GPIO_Init(iic->SDA_GPIO, &GPIO_InitStruct);


}
void SDA_Out(IIC_HandleTypeDef* iic)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = iic->SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD ;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(iic->SDA_GPIO, &GPIO_InitStruct);
}
void SDA_In(IIC_HandleTypeDef* iic)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = iic->SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(iic->SDA_GPIO, &GPIO_InitStruct);
}
void IIC_Start(IIC_HandleTypeDef* iic)
{
	SDA_Out(iic);
	HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(iic->SDA_GPIO,iic->SDA_PIN,GPIO_PIN_SET);
	DELAY_us(2);
	HAL_GPIO_WritePin(iic->SDA_GPIO,iic->SDA_PIN,GPIO_PIN_RESET);
	DELAY_us(2);
	HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_RESET);
}
void IIC_Stop(IIC_HandleTypeDef* iic)
{
	SDA_Out(iic);
	HAL_GPIO_WritePin(iic->SDA_GPIO,iic->SDA_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_RESET);
	DELAY_us(1);
	//HAL_GPIO_WritePin(iic->SDA_GPIO,iic->SDA_PIN,GPIO_PIN_RESET);
	//DELAY_us(1);
	HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_SET);
	DELAY_us(1);
	HAL_GPIO_WritePin(iic->SDA_GPIO,iic->SDA_PIN,GPIO_PIN_SET);
	DELAY_us(1);
}
void IIC_Send_Byte(IIC_HandleTypeDef* iic,uint8_t data)
{
	uint8_t i;
	SDA_Out(iic);
	
	for(i=0;i<8;i++)
	{
		HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_RESET);
			(data&0x80)?HAL_GPIO_WritePin(iic->SDA_GPIO,iic->SDA_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(iic->SDA_GPIO,iic->SDA_PIN,GPIO_PIN_RESET);
			data <<= 1; 
		DELAY_us(1);
		HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_SET);
			DELAY_us(1);


	}
HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_RESET);
}
uint8_t IIC_Read_Byte(IIC_HandleTypeDef* iic,uint8_t ack)
{
	uint8_t i=0,data=0;
	SDA_In(iic);
	data=0;
	for(i=0;i<8;i++)
	{	
		
		HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_RESET);
		DELAY_us(1);
		HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_SET);
		data<<=1;
		DELAY_us(2);
		data|=HAL_GPIO_ReadPin(iic->SDA_GPIO,iic->SDA_PIN);
		DELAY_us(1);
	}
	if (!ack)
        IIC_ack(iic,0);//·￠?ínACK
    else
        IIC_ack(iic,1); //·￠?íACK   
	return data;
}
uint8_t IIC_Wait_Ack(IIC_HandleTypeDef* iic)			//0有应答  
{
	uint8_t ack=1;
	HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_RESET);
	SDA_In(iic);
	DELAY_us(1);
	HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_SET);
	DELAY_us(1);
	ack=HAL_GPIO_ReadPin(iic->SDA_GPIO,iic->SDA_PIN);
	HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_RESET);
	return ack;
}
void IIC_ack(IIC_HandleTypeDef* iic,uint8_t ack)//0应答
{
	HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_RESET);
	
	SDA_Out(iic);

	if(ack==0)HAL_GPIO_WritePin(iic->SDA_GPIO,iic->SDA_PIN,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(iic->SDA_GPIO,iic->SDA_PIN,GPIO_PIN_SET);
	DELAY_us(1);
	HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_SET);
	DELAY_us(1);
	HAL_GPIO_WritePin(iic->SCL_GPIO,iic->SCL_PIN,GPIO_PIN_RESET);
}

uint8_t IIC_WriteAddr(IIC_HandleTypeDef* iic, uint8_t dev_addr, uint8_t reg_addr)
{
    IIC_Start(iic);
    IIC_Send_Byte(iic, dev_addr);
    IIC_Wait_Ack(iic);
    IIC_Send_Byte(iic, reg_addr);
    IIC_Wait_Ack(iic);
    IIC_Stop(iic);
}

uint8_t IIC_Read_Len(IIC_HandleTypeDef* iic,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
 	IIC_Start(iic); 
	IIC_Send_Byte(iic,(addr<<1)|0);//·￠?í?÷?tμ??·+D′?üá?	
	if(IIC_Wait_Ack(iic))	//μè′yó|′e
	{
		IIC_Stop(iic);		 
		return 1;		
	}
    IIC_Send_Byte(iic,reg);	//D′??′??÷μ??·
    IIC_Wait_Ack(iic);		//μè′yó|′e
    IIC_Start(iic);
	IIC_Send_Byte(iic,(addr<<1)|1);//·￠?í?÷?tμ??·+?á?üá?	
    IIC_Wait_Ack(iic);		//μè′yó|′e 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(iic,0);//?áêy?Y,·￠?ínACK 
		else *buf=IIC_Read_Byte(iic,1);		//?áêy?Y,·￠?íACK  
		len--;
		buf++; 
	}    
    IIC_Stop(iic);	//2úéúò???í￡?1ì??t 
	return 0;	
}

