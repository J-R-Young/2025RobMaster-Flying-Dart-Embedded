#include "Simiic.h"
#include "tim.h"

#define Delay_htim      htim10


SimiicHandle_TypeDef IICBus_OLEDkeyboard = {0};


void Simiic_Delay_ns(uint32_t ns)
{
	Delay_htim.Instance->CNT = 0;
    
    ns = ns * 12 / 1000;
    
	while(Delay_htim.Instance->CNT < ns);
}

//void Simiic_Delay_us(float us)
//{
//    Simiic_Delay_ns(us * 1000);
//}

/**
  * @name   Simiic_HandleInit
  * @brief  对模拟IIC句柄进行初始化
  * @call   Internal or External
  * @param  SimiicHandle        要操作的模拟IIC句柄
  * @param  SDA_GPIO_Port       模拟IIC总线的SDA通道
  * @param  SDA_GPIO_Pin        模拟IIC总线的SDA引脚
  * @param  SCL_GPIO_Port       模拟IIC总线的SCL通道
  * @param  SCL_GPIO_Pin        模拟IIC总线的SCL引脚
  * @param  Freq                模拟IIC总线的频率
  * @RetVal NULL
  */
void Simiic_HandleInit(SimiicHandle_TypeDef *SimiicHandle,
                       GPIO_TypeDef*        SDA_GPIO_Port, uint32_t             SDA_GPIO_Pin,
                       GPIO_TypeDef*        SCL_GPIO_Port, uint32_t             SCL_GPIO_Pin,
                       uint32_t             Freq         , uint8_t              BitOrder)
{
    SimiicHandle->SDA_GPIO_Port = SDA_GPIO_Port;
    SimiicHandle->SDA_GPIO_Pin  = SDA_GPIO_Pin;
    SimiicHandle->SCL_GPIO_Port = SCL_GPIO_Port;
    SimiicHandle->SCL_GPIO_Pin  = SCL_GPIO_Pin;
    SimiicHandle->Freq          = Freq;
    SimiicHandle->Period        = 1000000.0f / Freq;
    SimiicHandle->BitOrder      = BitOrder;
    
    Simiic_HandleMspInit(SimiicHandle);
}/* Simiic_HandleInit() */

/**
  * @name   Simiic_HandleMspInit
  * @brief  初始化模拟IIC句柄对应的IO口
  * @call   Internal or External
  * @param  SimiicHandle        要操作的模拟IIC句柄
  * @RetVal NULL
  */
void Simiic_HandleMspInit(SimiicHandle_TypeDef *SimiicHandle)
{
    if(SimiicHandle->SDA_GPIO_Port == NULL) return;
    if(SimiicHandle->SDA_GPIO_Pin  == NULL) return;
    if(SimiicHandle->SCL_GPIO_Port == NULL) return;
    if(SimiicHandle->SCL_GPIO_Pin  == NULL) return;
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
	GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull    = GPIO_PULLUP;
	GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_VERY_HIGH;
    
    SimiicHandle->SDA_GPIO_Port->ODR |= SimiicHandle->SDA_GPIO_Pin;
    GPIO_InitStruct.Pin     = SimiicHandle->SDA_GPIO_Pin;
    HAL_GPIO_Init(SimiicHandle->SDA_GPIO_Port, &GPIO_InitStruct);
    
    SimiicHandle->SCL_GPIO_Port->ODR |= SimiicHandle->SCL_GPIO_Pin;
    GPIO_InitStruct.Pin     = SimiicHandle->SCL_GPIO_Pin;
    HAL_GPIO_Init(SimiicHandle->SCL_GPIO_Port, &GPIO_InitStruct);
}/* Simiic_HandleMspInit() */


/**
  * @name   Simiic_SDA_In
  * @brief  将模拟IIC总线的SDA引脚设置为上拉输入模式
  * @call   Internal or External
  * @param  SimiicHandle        要操作的模拟IIC句柄
  * @RetVal NULL
  */
void Simiic_SDA_In(SimiicHandle_TypeDef *SimiicHandle)
{
    uint32_t position;
    uint32_t temp;
    
    // 得到引脚对应的寄存器位
    for(position = 0U; position < 16U; position++)
    {
        if(SimiicHandle->SDA_GPIO_Pin == (0x01U << position))
        {
            break;
        }
    }
    
    // 设置为输入模式
    temp = SimiicHandle->SDA_GPIO_Port->MODER;
    temp &= ~(0x03U << (position * 2U));
    SimiicHandle->SDA_GPIO_Port->MODER = temp;
    
    // 设置为上拉模式
    temp = SimiicHandle->SDA_GPIO_Port->PUPDR;
    temp &= ~(0x03U << (position * 2U));        // 先将相应位清零
    temp |=  (0x01U << (position * 2U));        // 后写入有效数据
    SimiicHandle->SDA_GPIO_Port->PUPDR = temp;
}/* Simiic_SDA_In */

/**
  * @name   Simiic_SDA_Out
* @brief  将模拟IIC总线的SDA引脚设置为推挽输出模式
  * @call   Internal or External
  * @param  SimiicHandle        要操作的模拟IIC句柄
  * @RetVal NULL
  */
void Simiic_SDA_Out(SimiicHandle_TypeDef *SimiicHandle)
{
    uint32_t position;
    uint32_t temp;
    
    // 得到引脚对应的寄存器位
    for(position = 0U; position < 16U; position++)
    {
        if(SimiicHandle->SDA_GPIO_Pin == (0x01U << position))
        {
            break;
        }
    }
    
    // 设为输出模式
    temp = SimiicHandle->SDA_GPIO_Port->MODER;
    temp &= ~(0x03U << (position * 2U));        // 先将相应位清零
    temp |=  (0x01U << (position * 2U));        // 后写入有效数据
    SimiicHandle->SDA_GPIO_Port->MODER = temp;
    
    // 设置为推挽输出
    temp = SimiicHandle->SDA_GPIO_Port->OTYPER;
    temp &= ~SimiicHandle->SDA_GPIO_Pin;
    SimiicHandle->SDA_GPIO_Port->OTYPER = temp;
    
}/* Simiic_SDA_Out */

/**
  * @name   Simiic_Start
  * @brief  向模拟IIC总线发送起始信号 (SCL高时的SDA下降沿)
  * @call   Internal or External
  * @param  Description of the parameters
  * @RetVal Return value
  */
void Simiic_Start(SimiicHandle_TypeDef *SimiicHandle)
{
    Simiic_SDA_Out(SimiicHandle);
    
    Simiic_SCL_H(SimiicHandle);
    Simiic_SDA_H(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 4);
    
    Simiic_SDA_L(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 4);
    
    Simiic_SCL_L(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 4);
    
}/* Simiic_Start */

/**
  * @name   Simiic_Stop
  * @brief  向模拟IIC总线发送结束信号 (SCL高时的SDA上升沿)
  * @call   Internal or External
  * @param  Description of the parameters
  * @RetVal Return value
  */
void Simiic_Stop(SimiicHandle_TypeDef *SimiicHandle)
{
    Simiic_SDA_Out(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 2);
    
    Simiic_SCL_H(SimiicHandle);
    Simiic_SDA_L(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 2);
    
    Simiic_SDA_H(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 2);
    
}/* Simiic_Stop */


/**
  * @name   Simiic_WaitACK
  * @brief  从模拟IIC总线读取一个应答信号
  * @call   Internal or External
  * @param  SimiicHandle        要操作的模拟IIC句柄
  * @RetVal ack                 从模拟IIC总线读回的应答信号
  */
uint8_t Simiic_WaitACK(SimiicHandle_TypeDef *SimiicHandle)
{
    uint8_t ack = 1;
    
    Simiic_SDA_H(SimiicHandle);     // 使SDA快速变为高电平
    Simiic_SDA_In(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 4);
    
    Simiic_SCL_H(SimiicHandle);
    
    ack = Simiic_SDA_Get(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 2);
    
    if(ack == SimiicAck_NACK)    // 再给一次机会
    {
        ack = Simiic_SDA_Get(SimiicHandle);
    }
    
    Simiic_SCL_L(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 4);
    
    return ack;
}/* Simiic_WaitACK() */

/**
  * @name   Simiic_Send_Ack
  * @brief  向模拟IIC总线发送一个应答信号
  * @call   Internal or External
  * @param  SimiicHandle        要操作的模拟IIC句柄
  * @param  ack                 要发送的应答信号
  * @RetVal NULL
  */
void Simiic_Send_Ack(SimiicHandle_TypeDef *SimiicHandle, SimiicAck_enum ack)
{
    Simiic_SDA_Out(SimiicHandle);
    
    Simiic_SCL_L(SimiicHandle);
    
    if(ack == SimiicAck_NACK)
    {
        Simiic_SDA_H(SimiicHandle);
    }
    else
    {
        Simiic_SDA_L(SimiicHandle);
    }
    
    Simiic_Delay_us(SimiicHandle->Period / 4);
    
    Simiic_SCL_H(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 2);
    
    Simiic_SCL_L(SimiicHandle);
    
    Simiic_Delay_us(SimiicHandle->Period / 4);
}/* Simiic_Send_Ack */

/**
  * @name   Simiic_Send_Byte
  * @brief  向模拟IIC总线发送一个字节的数据
  * @call   Internal or External
  * @param  SimiicHandle        要操作的模拟IIC句柄
  * @param  Data                要发送的数据
  * @RetVal NULL
  */
void Simiic_Send_Byte(SimiicHandle_TypeDef *SimiicHandle, uint8_t Data)
{
    
    Simiic_SDA_Out(SimiicHandle);       
    
    Simiic_SCL_L(SimiicHandle);
    
    if(SimiicHandle->BitOrder == BitOrder_MSB)
    {
        for(uint8_t i = 0; i < 8; i++)
        {            
            if(Data & 0x80)
            {
                Simiic_SDA_H(SimiicHandle);
            }
            else
            {
                Simiic_SDA_L(SimiicHandle);
            }
            Data <<= 1;
            
            Simiic_Delay_us(SimiicHandle->Period / 4);
            
            Simiic_SCL_H(SimiicHandle);
            
            Simiic_Delay_us(SimiicHandle->Period / 2);
            
            Simiic_SCL_L(SimiicHandle);
            
            Simiic_Delay_us(SimiicHandle->Period / 4);
        }
    }/* MSB */
    else
    {
        for(uint8_t i = 0; i < 8; i++)
        {            
            if(Data & 0x01)
            {
                Simiic_SDA_H(SimiicHandle);
            }
            else
            {
                Simiic_SDA_L(SimiicHandle);
            }
            Data >>= 1;
            
            Simiic_Delay_us(SimiicHandle->Period / 4);
            
            Simiic_SCL_H(SimiicHandle);
            
            Simiic_Delay_us(SimiicHandle->Period / 2);
            
            Simiic_SCL_L(SimiicHandle);
            
            Simiic_Delay_us(SimiicHandle->Period / 4);
        }
    }/* LSB */
}/* Simiic_Send_Byte() */


/**
  * @name   Simiic_Read_Byte
  * @brief  从模拟IIC总线上读取一个字节
  * @call   Internal or External
  * @param  SimiicHandle        要操作的模拟IIC句柄
  * @param  ack                 要发送到总线上的应答
  * @RetVal data                从模拟IIC总线上读取的数据
  */
uint8_t Simiic_Read_Byte(SimiicHandle_TypeDef *SimiicHandle, uint8_t ack)
{
    uint8_t data;
    
    Simiic_SDA_In(SimiicHandle);
    
    if(SimiicHandle->BitOrder == BitOrder_MSB)
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            Simiic_Delay_us(SimiicHandle->Period / 4);
            
            Simiic_SCL_H(SimiicHandle);
            
            data <<= 1;
            data |= Simiic_SDA_Get(SimiicHandle);
            
            Simiic_Delay_us(SimiicHandle->Period / 2);
            
            Simiic_SCL_L(SimiicHandle);
            
            Simiic_Delay_us(SimiicHandle->Period / 4);
        }
    }/* MSB */
    else
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            Simiic_Delay_us(SimiicHandle->Period / 4);
            
            Simiic_SCL_H(SimiicHandle);
            
            data |= (Simiic_SDA_Get(SimiicHandle) << 0x07U);
            data >>= 1;
            
            Simiic_Delay_us(SimiicHandle->Period / 2);
            
            Simiic_SCL_L(SimiicHandle);
            
            Simiic_Delay_us(SimiicHandle->Period / 4);
        }
    }/* LSB */
    
    Simiic_Send_Ack(SimiicHandle, ack);
    
    return data;
}/* Simiic_Read_Byte() */

