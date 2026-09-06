#include "Vision.h"
#include "CRC_Check.h"
//#include "Gimbal.h"
#include "can.h"
#include "rtc.h"

#define  Vision_huart   huart_Vision
#define  Vision_UART    USART1
#define  Vision_rx_dma	hdma_usart1_rx
#define  Vision_tx_dma	hdma_usart1_tx

VisionFrameRX_structTypedef  VisionData = {0};
VisionFrameTX_unionTypeDef TXFrame;

static uint8_t vision_rx_buffer[2][VISION_BUFFER_LENGTH_RX] = {0};
static uint8_t vision_tx_buffer[VISION_BUFFER_LENGTH_TX] = {0};

static void Vision_Analyze(VisionFrameRX_unionTypeDef *VisionMsg);
static void Vision_uart_Init(uint8_t *rxBuffer0, uint8_t *rxBuffer1);
static void Vision_UploadMsg_Pack(VisionFrameTX_unionTypeDef* Tdata);


//主函数初始化调用
void Vision_Init(void)
{
    Vision_uart_Init(vision_rx_buffer[0], vision_rx_buffer[1]);
}

void Vision_Upload(void)
{    
	Vision_UploadMsg_Pack(&TXFrame);
    
    Usart_SendArr(&Vision_huart, &TXFrame.u8arr[0], VISION_BUFFER_LENGTH_TX);
}
void Vision_Clear(void)
{
    VisionData.IdentifyFlag        = 0;
    VisionData.x_pixel_err         = 0;
    VisionData.x_distance_err      = 0;
    VisionData.tail        		   = 0;
}

static void Vision_uart_Init(uint8_t *Buffer0, uint8_t *Buffer1)
{
    //使能 DMA 串口接收
    SET_BIT(Vision_huart.Instance->CR3, USART_CR3_DMAR);
    
    
    
	__HAL_DMA_DISABLE(&Vision_rx_dma);
    //失效 DMA
    while(Vision_rx_dma.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&Vision_rx_dma);
    }
    
    
    
    // DMA源地址设为 rx接收缓冲区
    Vision_rx_dma.Instance->PAR = (uint32_t) & (Vision_UART->DR);
    // 内存缓冲区 0
    Vision_rx_dma.Instance->M0AR = (uint32_t)(Buffer0);
    // 内存缓冲区 1
    Vision_rx_dma.Instance->M1AR = (uint32_t)(Buffer1);
    // DMA传输次数
    Vision_rx_dma.Instance->NDTR = VISION_BUFFER_LENGTH_RX;
    //使能双缓冲区
    SET_BIT(Vision_rx_dma.Instance->CR, DMA_SxCR_DBM);
    //使能 DMA
    __HAL_DMA_ENABLE(&Vision_rx_dma);
    
    HAL_Delay(300);
    
    //使能空闲中断
    __HAL_UART_ENABLE_IT(&Vision_huart, UART_IT_IDLE);
}

// 报文解析
static void Vision_Analyze(VisionFrameRX_unionTypeDef *VisionMsg)
{
    if(VisionMsg == NULL)
    {
        return;
    }
	
	DartCtrlHandle.Vision_Last_State = VisionData.IdentifyFlag;
	
    VisionData.IdentifyFlag    = VisionMsg->VisionFrameRX.IdentifyFlag;
    VisionData.x_pixel_err     = VisionMsg->VisionFrameRX.x_pixel_err;
    VisionData.x_distance_err  = VisionMsg->VisionFrameRX.x_distance_err;
    VisionData.tail  		   = VisionMsg->VisionFrameRX.tail;
     
    if(VisionData.IdentifyFlag == 1)
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1);
    }
    else
    {
       HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,0);
    }
}


static void Vision_UploadMsg_Pack(VisionFrameTX_unionTypeDef* Tdata)
{
//    Tdata->VisionFrameTX.Pitch          = 1;
//    Tdata->VisionFrameTX.Yaw            = 1;
//    Tdata->VisionFrameTX.RobotID        = 1;
//    Tdata->VisionFrameTX.VisionMode     = 1;
//    Tdata->VisionFrameTX.ShooterSpeed   = 1;
//    Tdata->VisionFrameTX.Exposure       = 0;
    
//	
//    Tdata->VisionFrameTX.Pitch          = icmData.pitch_mahony;
//    Tdata->VisionFrameTX.Yaw            = icmData.yaw;
//    Tdata->VisionFrameTX.RobotID        = hCtrl.RobotID;
//    Tdata->VisionFrameTX.VisionMode     = hCtrl.VisionMode;
//    Tdata->VisionFrameTX.Exposure       = 0;
//    Tdata->VisionFrameTX.TimeStamp      = RTCtime.Hours * 60 * 60 + RTCtime.Minutes * 60 + RTCtime.Seconds \
//                                        + (float)(3199 - RTCtime.SubSeconds) / 3199.0;
	
}
void VisionRx_callback(void)
{ 
    static uint16_t this_time_rx_len = 0;
	
    if(Vision_UART->SR & UART_FLAG_IDLE)
    {
               
        __HAL_UART_CLEAR_PEFLAG(&Vision_huart);
        
//        // 失能DMA
//        __HAL_DMA_DISABLE(&Vision_rx_dma);
        
        if((Vision_rx_dma.Instance->CR & DMA_SxCR_CT) == RESET) // 当前帧接收在 buffer0
        {
            // 失能DMA
            __HAL_DMA_DISABLE(&Vision_rx_dma);
            
            // 记录本次传输的数据量
            this_time_rx_len = VISION_BUFFER_LENGTH_RX - Vision_rx_dma.Instance->NDTR;
            
            // 重新设定数据长度
            Vision_rx_dma.Instance->NDTR = VISION_BUFFER_LENGTH_RX;
        
            // 将接收地址设定为 buffer1
            Vision_rx_dma.Instance->CR |= DMA_SxCR_CT;
        
            // 使能DMA
            __HAL_DMA_ENABLE(&Vision_rx_dma);
            
            if(this_time_rx_len == VISION_FRAME_LENGTH_RX)
            {
                // 处理视觉信息
                Vision_Analyze((VisionFrameRX_structTypedef *)(vision_rx_buffer[0]));
            }
            
            // 将已经用过的数据清空
            for(uint8_t i = 0; i < VISION_BUFFER_LENGTH_RX; i++)
            {
                vision_rx_buffer[0][i] = 0;
            }
            
        }
        else    // 当前帧接收在 buffer1
        {
            
            // 失能DMA
             __HAL_DMA_DISABLE(&Vision_rx_dma);
            
            // 记录本次传输的数据量
            this_time_rx_len = VISION_BUFFER_LENGTH_RX - Vision_rx_dma.Instance->NDTR;
            
            
            // 重新设定数据长度
            Vision_rx_dma.Instance->NDTR = VISION_BUFFER_LENGTH_RX;
            
            // 将接收地址设定为 buffer0
            Vision_rx_dma.Instance->CR &= ~(DMA_SxCR_CT);
            
            // 使能DMA
            __HAL_DMA_ENABLE(&Vision_rx_dma);
         
            if(this_time_rx_len == VISION_FRAME_LENGTH_RX)
            {
                // 处理视觉信息
                Vision_Analyze((VisionFrameRX_structTypedef *)(vision_rx_buffer[1]));
            }

            // 将已经用过的数据清空
            for(uint8_t i = 0; i < VISION_BUFFER_LENGTH_RX; i++)
            {
                vision_rx_buffer[1][i] = 0;
            }
        }
    }
}

