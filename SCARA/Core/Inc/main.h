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
#define BTN_COLOR_Pin GPIO_PIN_2
#define BTN_COLOR_GPIO_Port GPIOC
#define BTN_LINEA_Pin GPIO_PIN_3
#define BTN_LINEA_GPIO_Port GPIOC
#define BTN_LINEA_EXTI_IRQn EXTI3_IRQn
#define TIM_ENC_BR1_2_Pin GPIO_PIN_1
#define TIM_ENC_BR1_2_GPIO_Port GPIOA
#define TIM_ENC_BR1_1_Pin GPIO_PIN_5
#define TIM_ENC_BR1_1_GPIO_Port GPIOA
#define TIM_ENC_BR2_1_Pin GPIO_PIN_6
#define TIM_ENC_BR2_1_GPIO_Port GPIOA
#define TIM_ENC_BR2_2_Pin GPIO_PIN_7
#define TIM_ENC_BR2_2_GPIO_Port GPIOA
#define BTN_CIRCULO_Pin GPIO_PIN_4
#define BTN_CIRCULO_GPIO_Port GPIOC
#define BTN_CIRCULO_EXTI_IRQn EXTI4_IRQn
#define BTN_RESET_Pin GPIO_PIN_5
#define BTN_RESET_GPIO_Port GPIOC
#define GIRO_BR1_In1_Pin GPIO_PIN_7
#define GIRO_BR1_In1_GPIO_Port GPIOE
#define GIRO_BR1_In2_Pin GPIO_PIN_8
#define GIRO_BR1_In2_GPIO_Port GPIOE
#define TIM_BR1_Pin GPIO_PIN_9
#define TIM_BR1_GPIO_Port GPIOE
#define GIRO_BR2_In1_Pin GPIO_PIN_10
#define GIRO_BR2_In1_GPIO_Port GPIOE
#define TIM_BR2_Pin GPIO_PIN_11
#define TIM_BR2_GPIO_Port GPIOE
#define GIRO_BR2_In2_Pin GPIO_PIN_12
#define GIRO_BR2_In2_GPIO_Port GPIOE
#define TIM_BA_Pin GPIO_PIN_13
#define TIM_BA_GPIO_Port GPIOE
#define GIRO_BA_In1_Pin GPIO_PIN_14
#define GIRO_BA_In1_GPIO_Port GPIOE
#define GIRO_BA_In2_Pin GPIO_PIN_15
#define GIRO_BA_In2_GPIO_Port GPIOE
#define TIM_ENC_BA1_Pin GPIO_PIN_12
#define TIM_ENC_BA1_GPIO_Port GPIOD
#define TIM_ENC_BA2_Pin GPIO_PIN_13
#define TIM_ENC_BA2_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
