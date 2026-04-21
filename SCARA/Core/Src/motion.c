/*
 * encoder_hw.c
 *
 *  Created on: Mar 30, 2026
 *      Author: Sofia
 */

#include "main.h"
#include "motion.h"

/* USER CODE BEGIN 0 */
//declaramos los handlers de los Timers que configuro Gabri en el main.c en modo encoder
extern TIM_HandleTypeDef htim2; //motor 1
extern TIM_HandleTypeDef htim3; //motor 2
extern TIM_HandleTypeDef htim4; //motor 3
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */
//inicialización de los encoders
//esta función se llama una sola vez en el main.c para encender los contadores
void Encoders_Init (void) {
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

//lectura de los encoders
//deuelve la cantidad de pasos (posición) del motor que le pidas (1, 2 o 3)
int32_t Leer_Pasos_Encoder (int numero_motor) {
	int32_t pasos = 0;

	switch (numero_motor) {
	case 1:
		//leemos el registro del timer 2
		pasos = (int32_t)__HAL_TIM_GET_COUNTER(&htim2);
		break;
	case 2:
		//leemos el registro del timer 3
		pasos = (int32_t)__HAL_TIM_GET_COUNTER(&htim3);
		break;
	case 3:
		//leemos el registro del timer 4
		pasos = (int32_t)__HAL_TIM_GET_COUNTER(&htim4);
		break;
	default:
		pasos = 0;
		break;
	}

	return pasos;
}
/* USER CODE END 1 */
