/********************************************************************************************************************* 
* 文件名称          rc_SBUS.c
* 版本信息          v1.1.0

* 修改记录
* 日期              作者                备注
* 2025-03-23        Raze            second version
* 新增对FS-i6x的sbus解析和调用，可在rc_SBUS.h中修改RC_FRAME_LENGTH的值进行遥控器的一键更换，不影响图传链路键鼠
********************************************************************************************************************/

#include "rc_SBUS.h"
#include "monitoring_key.h" 
#define RC_huart    huart2
#define RC_UART		USART2
#define RC_dma		hdma_usart2_rx
RC_ctrl_t rc_ctrl;

//接收原始数据，为25个字节，给了36个字节长度，防止DMA传输越界
static uint8_t sbus_rx_buf[2][SBUS_RX_BUF_NUM];
static void sbus_to_rc(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl);
static void RC_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
static int16_t RC_abs(int16_t value);
static void RC_restart(uint16_t dma_buf_num);

uint8_t RC_Flag = 0;
//主函数初始化调用
void remote_control_init(void)
{
    RC_init(sbus_rx_buf[0], sbus_rx_buf[1], SBUS_RX_BUF_NUM);
	Key_MonitoringInit( Mouse_Left, Mouse_Right,
                        Key_W, Key_A, Key_S, Key_D, Key_Q, Key_E, Key_Shift, Key_Ctrl,
                        Key_R, Key_F, Key_G, Key_Z, Key_X, Key_C, Key_V, Key_B
                      );
}

static void RC_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
		//enable the DMA transfer for the receiver request
		//使能 DMA 串口接收
		SET_BIT(RC_huart.Instance->CR3, USART_CR3_DMAR);
		//enalbe idle interrupt
		//使能空闲中断
		__HAL_UART_ENABLE_IT(&RC_huart, UART_IT_IDLE);
		//disable DMA
		//失效 DMA
		__HAL_DMA_DISABLE(&RC_dma);
	
		while(RC_dma.Instance->CR & DMA_SxCR_EN)
		{
				__HAL_DMA_DISABLE(&RC_dma);
		}
		
		RC_dma.Instance->PAR = (uint32_t) & (RC_UART->DR);
		//memory buffer 1
		//内存缓冲区 1
		RC_dma.Instance->M0AR = (uint32_t)(rx1_buf);
		//memory buffer 2
		//内存缓冲区 2
		RC_dma.Instance->M1AR = (uint32_t)(rx2_buf);
		//data length
		//数据长度
		RC_dma.Instance->NDTR = dma_buf_num;
		//enable double memory buffer
		//使能双缓冲区
		SET_BIT(RC_dma.Instance->CR, DMA_SxCR_DBM);
		//enable DMA
		//使能 DMA
		__HAL_DMA_ENABLE(&RC_dma);
}

//外部调用
const RC_ctrl_t *get_remote_control_point(void)
{
    return &rc_ctrl;
}


//判断遥控器数据是否出错，
uint8_t RC_data_is_error(void)
{
	if(RC_Flag == 1)
	{
		//使用了go to语句 方便出错统一处理遥控器变量数据归零
		if (RC_abs(rc_ctrl.rc.ch[0]) > 1024)
		{
			goto error;
		}
		if (RC_abs(rc_ctrl.rc.ch[1]) > 1024)
		{
			goto error;
		}
		if (RC_abs(rc_ctrl.rc.ch[2]) > 1024)
		{
			goto error;
		}
		if (RC_abs(rc_ctrl.rc.ch[3]) > 1024)
		{
			goto error;
		}
		
		if (RC_abs(rc_ctrl.rc.ch[0]) < 30)
		{
			rc_ctrl.rc.ch[0] = 0;
		}
		if (RC_abs(rc_ctrl.rc.ch[1]) < 30)
		{
			rc_ctrl.rc.ch[1] = 0;
		}
		if (RC_abs(rc_ctrl.rc.ch[2]) < 30)
		{
			rc_ctrl.rc.ch[2] = 0;
		}
		if (RC_abs(rc_ctrl.rc.ch[3]) < 30)
		{
			rc_ctrl.rc.ch[3] = 0;
		}
	}
	else if(RC_Flag == 2)
	{
		//使用了go to语句 方便出错统一处理遥控器变量数据归零
		if (RC_abs(rc_ctrl.rc.ch[0]) > 660)
		{
			goto error;
		}
		if (RC_abs(rc_ctrl.rc.ch[1]) > 660)
		{
			goto error;
		}
		if (RC_abs(rc_ctrl.rc.ch[2]) > 660)
		{
			goto error;
		}
		if (RC_abs(rc_ctrl.rc.ch[3]) > 660)
		{
			goto error;
		}
		if (rc_ctrl.rc.s[0] == 0)
		{
			goto error;
		}
		if (rc_ctrl.rc.s[1] == 0)
		{
			goto error;
		}
	}

    return 0;

error:
    rc_ctrl.rc.ch[0] = 0;
    rc_ctrl.rc.ch[1] = 0;
    rc_ctrl.rc.ch[2] = 0;
    rc_ctrl.rc.ch[3] = 0;
    rc_ctrl.rc.ch[4] = 0;
    rc_ctrl.rc.s[0] = 2;
    rc_ctrl.rc.s[1] = 2;
    rc_ctrl.mouse.x = 0;
    rc_ctrl.mouse.y = 0;
    rc_ctrl.mouse.z = 0;
    rc_ctrl.mouse.press_l = 0;
    rc_ctrl.mouse.press_r = 0;
    rc_ctrl.key.v = 0;
    return 1;
}

//写入stm32f4xx_it.c串口中断
//void USART2_IRQHandler(void)
void SBUS_callback(void)
{
    if(RC_huart.Instance->SR & UART_FLAG_RXNE)//接收到数据
    {
        __HAL_UART_CLEAR_PEFLAG(&RC_huart);
    }
    else if(RC_UART->SR & UART_FLAG_IDLE)
    {
            static uint16_t this_time_rx_len = 0;

        __HAL_UART_CLEAR_PEFLAG(&RC_huart);
    
        if ((RC_dma.Instance->CR & DMA_SxCR_CT) == RESET)
        {
            /* Current memory buffer used is Memory 0 */
            //disable DMA
            //失效 DMA
            __HAL_DMA_DISABLE(&RC_dma);
            //get receive data length, length = set_data_length - remain_length
            //获取接收数据长度,长度 = 设定长度 - 剩余长度
            this_time_rx_len = SBUS_RX_BUF_NUM - RC_dma.Instance->NDTR;
            //reset set_data_lenght
            //重新设定数据长度
            RC_dma.Instance->NDTR = SBUS_RX_BUF_NUM;
            //set memory buffer 1
            //设定缓冲区 1
            RC_dma.Instance->CR |= DMA_SxCR_CT;
            //enable DMA
            //使能 DMA
            __HAL_DMA_ENABLE(&RC_dma);
        
            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                sbus_to_rc(sbus_rx_buf[0], &rc_ctrl);
                if(RC_data_is_error()==1)
                {
                    RC_restart(SBUS_RX_BUF_NUM);
                }
				Key_UpdateStateAll();

            }
        }
        else
        {
            /* Current memory buffer used is Memory 1 */
            //disable DMA
            //失效 DMA
            __HAL_DMA_DISABLE(&RC_dma);
            //get receive data length, length = set_data_length - remain_length
            //获取接收数据长度,长度 = 设定长度 - 剩余长度
            this_time_rx_len = SBUS_RX_BUF_NUM - RC_dma.Instance->NDTR;
            //reset set_data_lenght
            //重新设定数据长度
            RC_dma.Instance->NDTR = SBUS_RX_BUF_NUM;
            //set memory buffer 0
            //设定缓冲区 0
            DMA1_Stream1->CR &= ~(DMA_SxCR_CT);
            //enable DMA
            //使能 DMA
            __HAL_DMA_ENABLE(&RC_dma);
        
            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                //处理遥控器数据
                sbus_to_rc(sbus_rx_buf[1], &rc_ctrl);
                if(RC_data_is_error()==1)
                {
                    RC_restart(SBUS_RX_BUF_NUM);
                }

                Key_UpdateStateAll();
            }
        }
    }
}

static void RC_restart(uint16_t dma_buf_num)
{
    //disable UART
    __HAL_UART_DISABLE(&RC_huart);
    //disable DMA
    __HAL_DMA_DISABLE(&RC_dma);
    //reset set_data_lenght
    RC_dma.Instance->NDTR = SBUS_RX_BUF_NUM;

    __HAL_UART_CLEAR_IDLEFLAG(&RC_huart);

    //DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
    __HAL_DMA_CLEAR_FLAG(&RC_dma,DMA_FLAG_TCIF2_6);
    //ensable UART
    __HAL_UART_ENABLE(&RC_huart);
    //ensable DMA
    __HAL_DMA_ENABLE(&RC_dma);
}



//取正函数
static int16_t RC_abs(int16_t value)
{
    if (value > 0)
    {
        return value;
    }
    else
    {
        return -value;
    }
}

static void sbus_to_rc(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl)
{
	//
	DartCtrlHandle.Record_Right_Num = rc_ctrl->rc.ch[1];
    if (sbus_buf == NULL || rc_ctrl == NULL)
    {
        return;
    }
	if(sbus_buf[0] == 0x0f && sbus_buf[24] == 0x00 && RC_FRAME_LENGTH == 25u)   //富斯i6x
	{
		RC_Flag = 1;
		
		rc_ctrl->rc.ch_last[7] = rc_ctrl->rc.ch[7];
		
		rc_ctrl->rc.ch[0] = (sbus_buf[1] | (sbus_buf[2] << 8)) & 0x07ff; //!< Channel 0
		rc_ctrl->rc.ch[1] = ((sbus_buf[2] >> 3) | (sbus_buf[3] << 5)) & 0x07ff; //!< Channel 1
		rc_ctrl->rc.ch[2] = ((sbus_buf[3] >> 6) | (sbus_buf[4] << 2) | (sbus_buf[5] << 10)) &0x07ff;//!< Channel 2
		rc_ctrl->rc.ch[3] = ((sbus_buf[5] >> 1) | (sbus_buf[6] << 7)) & 0x07ff; //!< Channel 3
		rc_ctrl->rc.ch[4] = (sbus_buf[6] >> 4 | ((int16_t)sbus_buf[7] << 4 )) & 0x07FF;
		rc_ctrl->rc.ch[5] = (sbus_buf[7] >> 7 | ((int16_t)sbus_buf[8] << 1 )  | (int16_t)sbus_buf[9] <<  9 ) & 0x07FF;
		rc_ctrl->rc.ch[6] = (sbus_buf[9] >> 2 | ((int16_t)sbus_buf[10] << 6 )) & 0x07FF;
		rc_ctrl->rc.ch[7] = (sbus_buf[10] >> 5 | ((int16_t)sbus_buf[11] << 3 )) & 0x07FF;
		rc_ctrl->rc.ch[8] = (sbus_buf[12] << 0 | ((int16_t)sbus_buf[13] << 8 )) & 0x07FF;
		rc_ctrl->rc.ch[9] = (sbus_buf[13] >> 3 | ((int16_t)sbus_buf[14] << 5 )) & 0x07FF;    
		
		
		
		rc_ctrl->rc.ch[0] -= 1024;
		rc_ctrl->rc.ch[1] -= 1024;
		rc_ctrl->rc.ch[2] -= 1024;
		rc_ctrl->rc.ch[3] -= 1024;
		
		rc_ctrl->rc.ch[4] -= 240;
		rc_ctrl->rc.ch[5] -= 240;
		rc_ctrl->rc.ch[6] -= 240;
		rc_ctrl->rc.ch[7] -= 240;
		rc_ctrl->rc.ch[8] -= 240;
		rc_ctrl->rc.ch[9] -= 240;
		
	}
	else
	{
		RC_Flag = 2;
		rc_ctrl->rc.ch[0] = (sbus_buf[0] | (sbus_buf[1] << 8)) & 0x07ff; //!< Channel 0
		rc_ctrl->rc.ch[1] = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff; //!< Channel 1
		rc_ctrl->rc.ch[2] = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) | (sbus_buf[4] << 10)) &0x07ff;//!< Channel 2
		rc_ctrl->rc.ch[3] = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff; //!< Channel 3
		
		rc_ctrl->rc.s_last[0] = rc_ctrl->rc.s[0];                                   // 左拨杆
		rc_ctrl->rc.s[0] = ((sbus_buf[5] >> 4) & 0x0003);
		
		rc_ctrl->rc.s_last[1] = rc_ctrl->rc.s[1];                                   // 右拨杆
		rc_ctrl->rc.s[1] = ((sbus_buf[5] >> 4) & 0x000C) >> 2; 
		
		rc_ctrl->mouse.x_last = rc_ctrl->mouse.x;                                   // 鼠标X轴
		rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8); 
		
		rc_ctrl->mouse.y_last = rc_ctrl->mouse.y;                                   // 鼠标Y轴
		rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8); 
		
		rc_ctrl->mouse.z_last = rc_ctrl->mouse.z;                                   // 鼠标Z轴
		rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8); 
		
		rc_ctrl->mouse.press_l = sbus_buf[12];                                      // 鼠标左键
		rc_ctrl->mouse.press_r = sbus_buf[13];                                      // 鼠标右键
		
		rc_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);                        // 键盘值
		
		rc_ctrl->rc.ch[4] = sbus_buf[16] | (sbus_buf[17] << 8);                     // 肩部拨盘
		
		rc_ctrl->rc.ch[0] -= 1024;
		rc_ctrl->rc.ch[1] -= 1024;
		rc_ctrl->rc.ch[2] -= 1024;
		rc_ctrl->rc.ch[3] -= 1024;
		rc_ctrl->rc.ch[4] -= 1024;

	}

//    rc_ctrl->rc.ch[4] -= 1024;
}
