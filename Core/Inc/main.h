/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define huart_Referee huart3
#define huart_RC huart2
#define huart_Navigation huart4
#define huart_Vision huart1
#define LED_0_Pin GPIO_PIN_2
#define LED_0_GPIO_Port GPIOE
#define LED_1_Pin GPIO_PIN_3
#define LED_1_GPIO_Port GPIOE
#define LED_2_Pin GPIO_PIN_4
#define LED_2_GPIO_Port GPIOE
#define LED_3_Pin GPIO_PIN_5
#define LED_3_GPIO_Port GPIOE
#define Gate_3_Pin GPIO_PIN_6
#define Gate_3_GPIO_Port GPIOA
#define Gate_0_Pin GPIO_PIN_7
#define Gate_0_GPIO_Port GPIOA
#define Gate_1_Pin GPIO_PIN_5
#define Gate_1_GPIO_Port GPIOC
#define Gate_2_Pin GPIO_PIN_1
#define Gate_2_GPIO_Port GPIOB
#define RC_SBUS_Pin GPIO_PIN_6
#define RC_SBUS_GPIO_Port GPIOD
#define beep_Pin GPIO_PIN_8
#define beep_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
