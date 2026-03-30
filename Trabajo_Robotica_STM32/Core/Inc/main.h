/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HALL_SENSOR_Pin GPIO_PIN_1
#define HALL_SENSOR_GPIO_Port GPIOC
#define HALL_SENSOR_EXTI_IRQn EXTI1_IRQn
#define BTN_C1_Pin GPIO_PIN_2
#define BTN_C1_GPIO_Port GPIOC
#define BTN_C1_EXTI_IRQn EXTI2_IRQn
#define BTN_C2_Pin GPIO_PIN_3
#define BTN_C2_GPIO_Port GPIOC
#define BTN_C2_EXTI_IRQn EXTI3_IRQn
#define BTN_C3_Pin GPIO_PIN_4
#define BTN_C3_GPIO_Port GPIOC
#define BTN_C3_EXTI_IRQn EXTI4_IRQn
#define BTN_START_Pin GPIO_PIN_5
#define BTN_START_GPIO_Port GPIOC
#define BTN_START_EXTI_IRQn EXTI9_5_IRQn
#define DIR_J1_IN1_Pin GPIO_PIN_7
#define DIR_J1_IN1_GPIO_Port GPIOE
#define DIR_J1_IN2_Pin GPIO_PIN_8
#define DIR_J1_IN2_GPIO_Port GPIOE
#define DIR_J2_IN1_Pin GPIO_PIN_10
#define DIR_J2_IN1_GPIO_Port GPIOE
#define DIR_J2_IN2_Pin GPIO_PIN_12
#define DIR_J2_IN2_GPIO_Port GPIOE
#define DIR_Z_IN1_Pin GPIO_PIN_14
#define DIR_Z_IN1_GPIO_Port GPIOE
#define DIR_Z_IN2_Pin GPIO_PIN_15
#define DIR_Z_IN2_GPIO_Port GPIOE
#define LIMIT_SW_Z_Pin GPIO_PIN_8
#define LIMIT_SW_Z_GPIO_Port GPIOD
#define LIMIT_SW_Z_EXTI_IRQn EXTI9_5_IRQn
#define LIMIT_SW_J2_Pin GPIO_PIN_9
#define LIMIT_SW_J2_GPIO_Port GPIOD
#define LIMIT_SW_J2_EXTI_IRQn EXTI9_5_IRQn
#define LIMIT_SW_J1_Pin GPIO_PIN_10
#define LIMIT_SW_J1_GPIO_Port GPIOD
#define LIMIT_SW_J1_EXTI_IRQn EXTI15_10_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
