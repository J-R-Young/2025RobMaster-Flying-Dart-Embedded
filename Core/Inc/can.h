/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "Motor.h"
/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;

extern CAN_HandleTypeDef hcan2;

/* USER CODE BEGIN Private defines */

//CAN1
#define		MAIN_PULL_MOTOR_CAN_ID			0x201					//������
#define		PUSH_L_MOTOR_CAN_ID				0x202					//ǰ������
#define		PUSH_R_MOTOR_CAN_ID				0x203					//ǰ���ҵ��

//CAN2
#define     Small_Friction_CAN_ID           0x201                   //СĦ���ֵ��
#define		FRIC_L_MOTOR_CAN_ID				0x202					//Ħ��������
#define		FRIC_R_MOTOR_CAN_ID				0x203					//Ħ�����ҵ��
#define		LIFT_L_MOTOR_CAN_ID				0x204					//̧������
#define		LOAD_MOTOR_CAN_ID				0x205					//װ����
#define		YAW_MOTOR_CAN_ID				0x20A					//YAW����

/* USER CODE END Private defines */

void MX_CAN1_Init(void);
void MX_CAN2_Init(void);

/* USER CODE BEGIN Prototypes */
void can_filter_init(void);


void CAN_TX_CAN1_0x200(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_TX_CAN1_0x1FF(int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8);

void CAN_TX_CAN2_0x200(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_TX_CAN2_0x1FF(int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8);

void CAN_TX_CAN2_0x2FF(int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8);


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

