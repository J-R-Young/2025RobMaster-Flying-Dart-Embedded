/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
//#include "headfiles.h"
//#include "Motor.h"

CAN_TxHeaderTypeDef can_tx_message;
static uint8_t 	can_tx_data[8];

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_2TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_7TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_6TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}
/* CAN2 init function */
void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 3;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

}

static uint32_t HAL_RCC_CAN1_CLK_ENABLED=0;

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* CAN2 clock enable */
    __HAL_RCC_CAN2_CLK_ENABLE();
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN2 interrupt Init */
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
  /* USER CODE BEGIN CAN2_MspInit 1 */

  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* CAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/************************************
** Function:    can_filter_init
** Description: CAN�˲���ʼ��
** Others:      NULL
************************************/
void can_filter_init(void)
{
    CAN_FilterTypeDef can_filter_st;
    
    can_filter_st.FilterActivation      = ENABLE;
    can_filter_st.FilterMode            = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale           = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh          = 0x0000;
    can_filter_st.FilterIdLow           = 0x0000;
    can_filter_st.FilterMaskIdHigh      = 0x0000;
    can_filter_st.FilterMaskIdLow       = 0x0000;
    can_filter_st.FilterBank            = 0;
    can_filter_st.FilterFIFOAssignment  = CAN_RX_FIFO0;

    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    
	can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterBank = 14;
    
    HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/************************************
** Function:    HAL_CAN_RxFifo0MsgPendingCallback
** Description: CAN���ջص��жϺ���
** Others:      NULL
************************************/ // Online_State
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t	can_rx_data[8];
    float tmp;
    
    if(hcan==&hcan1)
    {
        HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&rx_header,can_rx_data);
    
        switch(rx_header.StdId)
        {
			case MAIN_PULL_MOTOR_CAN_ID://0x201   //3��29�� �����2006Ų��can1 
			{
				motor_data.Move_Trigger.lastPosition = motor_data.Move_Trigger.position;
				
				motor_data.Move_Trigger.position = (int16_t) can_rx_data[0]<<8|can_rx_data[1];
				motor_data.Move_Trigger.speed = (int16_t) can_rx_data[2]<<8|can_rx_data[3];
				motor_data.Move_Trigger.current = (int16_t) can_rx_data[4]<<8|can_rx_data[5];
			
				motor_data.Move_Trigger.turns = (motor_data.Move_Trigger.lastPosition - motor_data.Move_Trigger.position)>4096 ? 
																										motor_data.Move_Trigger.turns+1 : 
																										motor_data.Move_Trigger.turns;
				motor_data.Move_Trigger.turns = (motor_data.Move_Trigger.lastPosition - motor_data.Move_Trigger.position)<-4096 ? 
																										motor_data.Move_Trigger.turns-1 : 
																										motor_data.Move_Trigger.turns;
				motor_data.Move_Trigger.sumPosition = motor_data.Move_Trigger.turns * 8192 + motor_data.Move_Trigger.position;
				
				Device_Update(DeviceID_Move_Trigger);
				break;
			}
			case PUSH_R_MOTOR_CAN_ID:
			{
				motor_data.Pull_Spring_Right.lastPosition = motor_data.Pull_Spring_Right.position;
				
				motor_data.Pull_Spring_Right.position = (int16_t) can_rx_data[0]<<8|can_rx_data[1];
				motor_data.Pull_Spring_Right.speed = (int16_t) can_rx_data[2]<<8|can_rx_data[3];							
				motor_data.Pull_Spring_Right.current = (int16_t) can_rx_data[4]<<8|can_rx_data[5];
			
				motor_data.Pull_Spring_Right.turns = (motor_data.Pull_Spring_Right.lastPosition - motor_data.Pull_Spring_Right.position)>4096 ? 
																										motor_data.Pull_Spring_Right.turns+1 : 
																										motor_data.Pull_Spring_Right.turns;
				motor_data.Pull_Spring_Right.turns = (motor_data.Pull_Spring_Right.lastPosition - motor_data.Pull_Spring_Right.position)<-4096 ? 
																										motor_data.Pull_Spring_Right.turns-1 : 
																										motor_data.Pull_Spring_Right.turns;
				motor_data.Pull_Spring_Right.sumPosition = motor_data.Pull_Spring_Right.turns * 8192 + motor_data.Pull_Spring_Right.position;
				
				Device_Update(DeviceID_Pull_Spring_Right);
				break;
			}
			case PUSH_L_MOTOR_CAN_ID:
			{
				motor_data.Pull_Spring_Left.lastPosition = motor_data.Pull_Spring_Left.position;
				
				motor_data.Pull_Spring_Left.position = (int16_t) can_rx_data[0]<<8|can_rx_data[1];
				motor_data.Pull_Spring_Left.speed = (int16_t) can_rx_data[2]<<8|can_rx_data[3];							
				motor_data.Pull_Spring_Left.current = (int16_t) can_rx_data[4]<<8|can_rx_data[5];
			
				motor_data.Pull_Spring_Left.turns = (motor_data.Pull_Spring_Left.lastPosition - motor_data.Pull_Spring_Left.position)>4096 ? 
																										motor_data.Pull_Spring_Left.turns+1 : 
																										motor_data.Pull_Spring_Left.turns;
				motor_data.Pull_Spring_Left.turns = (motor_data.Pull_Spring_Left.lastPosition - motor_data.Pull_Spring_Left.position)<-4096 ? 
																										motor_data.Pull_Spring_Left.turns-1 : 
																										motor_data.Pull_Spring_Left.turns;
				motor_data.Pull_Spring_Left.sumPosition = motor_data.Pull_Spring_Left.turns * 8192 + motor_data.Pull_Spring_Left.position;
				
				Device_Update(DeviceID_Pull_Spring_Left);
				
				break;
			}

			default:
				break;	
		}
    }
    
    if(hcan == &hcan2)
    {
        HAL_CAN_GetRxMessage(&hcan2,CAN_RX_FIFO0,&rx_header,can_rx_data);
		
        switch(rx_header.StdId)
        {
			case Small_Friction_CAN_ID://0x201
			{
				motor_data.Small_Fric .lastPosition = motor_data.Small_Fric.position;
				
				motor_data.Small_Fric.position = (int16_t) can_rx_data[0]<<8|can_rx_data[1];
				motor_data.Small_Fric.speed = (int16_t) can_rx_data[2]<<8|can_rx_data[3];
				motor_data.Small_Fric.current = (int16_t) can_rx_data[4]<<8|can_rx_data[5];
			
				motor_data.Small_Fric.turns = (motor_data.Small_Fric.lastPosition - motor_data.Small_Fric.position)>4096 ? 
																										motor_data.Small_Fric.turns+1 : 
																										motor_data.Small_Fric.turns;
				motor_data.Small_Fric.turns = (motor_data.Small_Fric.lastPosition - motor_data.Small_Fric.position)<-4096 ? 
																										motor_data.Small_Fric.turns-1 : 
																										motor_data.Small_Fric.turns;
				motor_data.Small_Fric.sumPosition = motor_data.Small_Fric.turns * 8192 + motor_data.Small_Fric.position;
//				PULL_Position = motor_data.MAIN_PULL.turns;
				break;
			}
			case YAW_MOTOR_CAN_ID:
			{
				motor_data.Yaw.lastPosition = motor_data.Yaw.position;
				
				motor_data.Yaw.position = (int16_t) can_rx_data[0]<<8|can_rx_data[1];
				motor_data.Yaw.speed = (int16_t) can_rx_data[2]<<8|can_rx_data[3];							
				motor_data.Yaw.current = (int16_t) can_rx_data[4]<<8|can_rx_data[5];
			
				motor_data.Yaw.turns = (motor_data.Yaw.lastPosition - motor_data.Yaw.position)>4096 ? 
																										motor_data.Yaw.turns+1 : 
																										motor_data.Yaw.turns;
				motor_data.Yaw.turns = (motor_data.Yaw.lastPosition - motor_data.Yaw.position)<-4096 ? 
																										motor_data.Yaw.turns-1 : 
																										motor_data.Yaw.turns;
				motor_data.Yaw.sumPosition = motor_data.Yaw.turns * 8192 
																												+ motor_data.Yaw.position;
				
				Device_Update(DeviceID_Yaw);
				break;
			}
			case FRIC_L_MOTOR_CAN_ID://0x202
			{
				motor_data.Fric_Left.lastPosition = motor_data.Fric_Left.position;
				
				motor_data.Fric_Left.position = (int16_t) can_rx_data[0]<<8|can_rx_data[1];
				motor_data.Fric_Left.speed = (int16_t) can_rx_data[2]<<8|can_rx_data[3];
				motor_data.Fric_Left.current = (int16_t) can_rx_data[4]<<8|can_rx_data[5];
			
				motor_data.Fric_Left.turns = (motor_data.Fric_Left.lastPosition - motor_data.Fric_Left.position)>4096 ? 
																										motor_data.Fric_Left.turns+1 : 
																										motor_data.Fric_Left.turns;
				motor_data.Fric_Left.turns = (motor_data.Fric_Left.lastPosition - motor_data.Fric_Left.position)<-4096 ? 
																										motor_data.Fric_Left.turns-1 : 
																										motor_data.Fric_Left.turns;
				motor_data.Fric_Left.sumPosition = motor_data.Fric_Left.turns * 8192 
																												+ motor_data.Fric_Left.position;
				break;
			}
			case FRIC_R_MOTOR_CAN_ID://0x203
			{
				motor_data.Fric_Right.lastPosition = motor_data.Fric_Right.position;
				
				motor_data.Fric_Right.position = (int16_t) can_rx_data[0]<<8|can_rx_data[1];
				motor_data.Fric_Right.speed = (int16_t) can_rx_data[2]<<8|can_rx_data[3];
				motor_data.Fric_Right.current = (int16_t) can_rx_data[4]<<8|can_rx_data[5];
			
				motor_data.Fric_Right.turns = (motor_data.Fric_Right.lastPosition - motor_data.Fric_Right.position)>4096 ? 
																										motor_data.Fric_Right.turns+1 : 
																										motor_data.Fric_Right.turns;
				motor_data.Fric_Right.turns = (motor_data.Fric_Right.lastPosition - motor_data.Fric_Right.position)<-4096 ? 
																										motor_data.Fric_Right.turns-1 : 
																										motor_data.Fric_Right.turns;
				motor_data.Fric_Right.sumPosition = motor_data.Fric_Right.turns * 8192 + motor_data.Fric_Right.position;
				break;
			}
			case LIFT_L_MOTOR_CAN_ID://0x204
			{
				motor_data.Lift.lastPosition = motor_data.Lift.position;
				
				motor_data.Lift.position = (int16_t) can_rx_data[0]<<8|can_rx_data[1];
				motor_data.Lift.speed = (int16_t) can_rx_data[2]<<8|can_rx_data[3];
				motor_data.Lift.current = (int16_t) can_rx_data[4]<<8|can_rx_data[5];
			
				motor_data.Lift.turns = (motor_data.Lift.lastPosition - motor_data.Lift.position)>4096 ? 
																										motor_data.Lift.turns+1 : 
																										motor_data.Lift.turns;
				motor_data.Lift.turns = (motor_data.Lift.lastPosition - motor_data.Lift.position)<-4096 ? 
																										motor_data.Lift.turns-1 : 
																										motor_data.Lift.turns;
				motor_data.Lift.sumPosition = motor_data.Lift.turns * 8192 + motor_data.Lift.position;
				
				Device_Update(DeviceID_Lift);
				
				break;
			}
			case LOAD_MOTOR_CAN_ID://0x205
			{
				motor_data.Load.lastPosition = motor_data.Load.position;
				
				motor_data.Load.position = (int16_t) can_rx_data[0]<<8|can_rx_data[1];
				motor_data.Load.speed = (int16_t) can_rx_data[2]<<8|can_rx_data[3];
				motor_data.Load.current = (int16_t) can_rx_data[4]<<8|can_rx_data[5];
			
				motor_data.Load.turns = (motor_data.Load.lastPosition - motor_data.Load.position)>4096 ? 
																										motor_data.Load.turns+1 : 
																										motor_data.Load.turns;
				motor_data.Load.turns = (motor_data.Load.lastPosition - motor_data.Load.position)<-4096 ? 
																										motor_data.Load.turns-1 : 
																										motor_data.Load.turns;
				motor_data.Load.sumPosition = motor_data.Load.turns * 8192 + motor_data.Load.position;
				
				Device_Update(DeviceID_Load);
				break;
			}
		}
    }
}

void CAN_TX_CAN1_0x200(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;

    can_tx_message.StdId = 0x200;
    can_tx_message.IDE = CAN_ID_STD;
    can_tx_message.RTR = CAN_RTR_DATA;
    can_tx_message.DLC = 0x08;

    can_tx_data[0] = motor1 >> 8;
    can_tx_data[1] = motor1 & 0xFF;
    can_tx_data[2] = motor2 >> 8;
    can_tx_data[3] = motor2 & 0xff;
    can_tx_data[4] = motor3 >> 8;
    can_tx_data[5] = motor3 & 0xFF;
    can_tx_data[6] = motor4 >> 8;
    can_tx_data[7] = motor4 & 0xFF;

    HAL_CAN_AddTxMessage(&hcan1 , &can_tx_message , can_tx_data , &send_mail_box);
}

void CAN_TX_CAN1_0x1FF(int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8)
{
    uint32_t send_mail_box;

    can_tx_message.StdId = 0x1ff;
    can_tx_message.IDE = CAN_ID_STD;
    can_tx_message.RTR = CAN_RTR_DATA;
    can_tx_message.DLC = 0x08;

    can_tx_data[0] = motor5 >> 8;
    can_tx_data[1] = motor5 & 0xFF;
    can_tx_data[2] = motor6 >> 8;
    can_tx_data[3] = motor6 & 0xff;
    can_tx_data[4] = motor7 >> 8;
    can_tx_data[5] = motor7 & 0xFF;
    can_tx_data[6] = motor8 >> 8;
    can_tx_data[7] = motor8 & 0xFF;

    HAL_CAN_AddTxMessage(&hcan1 , &can_tx_message , can_tx_data , &send_mail_box);
}


void CAN_TX_CAN2_0x200(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;

    can_tx_message.StdId = 0x200;
    can_tx_message.IDE = CAN_ID_STD;
    can_tx_message.RTR = CAN_RTR_DATA;
    can_tx_message.DLC = 0x08;

    can_tx_data[0] = motor1 >> 8;
    can_tx_data[1] = motor1 & 0xFF;
    can_tx_data[2] = motor2 >> 8;
    can_tx_data[3] = motor2 & 0xff;
    can_tx_data[4] = motor3 >> 8;
    can_tx_data[5] = motor3 & 0xFF;
    can_tx_data[6] = motor4 >> 8;
    can_tx_data[7] = motor4 & 0xFF;

    HAL_CAN_AddTxMessage(&hcan2,&can_tx_message,can_tx_data,&send_mail_box);
}    


void CAN_TX_CAN2_0x1FF(int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8)
{
    uint32_t send_mail_box;

    can_tx_message.StdId = 0x1FF;
    can_tx_message.IDE = CAN_ID_STD;
    can_tx_message.RTR = CAN_RTR_DATA;
    can_tx_message.DLC = 0x08;

    can_tx_data[0] = motor5 >> 8;
    can_tx_data[1] = motor5 & 0xFF;
    can_tx_data[2] = motor6 >> 8;
    can_tx_data[3] = motor6 & 0xff;
    can_tx_data[4] = motor7 >> 8;
    can_tx_data[5] = motor7 & 0xFF;
    can_tx_data[6] = motor8 >> 8;
    can_tx_data[7] = motor8 & 0xFF;

    HAL_CAN_AddTxMessage(&hcan2, &can_tx_message, can_tx_data, &send_mail_box);
}

void CAN_TX_CAN2_0x2FF(int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8)
{
    uint32_t send_mail_box;

    can_tx_message.StdId = 0x2FF;
    can_tx_message.IDE = CAN_ID_STD;
    can_tx_message.RTR = CAN_RTR_DATA;
    can_tx_message.DLC = 0x08;

    can_tx_data[0] = motor5 >> 8;
    can_tx_data[1] = motor5 & 0xFF;
    can_tx_data[2] = motor6 >> 8;
    can_tx_data[3] = motor6 & 0xff;
    can_tx_data[4] = motor7 >> 8;
    can_tx_data[5] = motor7 & 0xFF;
    can_tx_data[6] = motor8 >> 8;
    can_tx_data[7] = motor8 & 0xFF;

    HAL_CAN_AddTxMessage(&hcan2, &can_tx_message, can_tx_data, &send_mail_box);
}
/* USER CODE END 1 */
