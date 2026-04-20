/*
 * cambiarcolor.h
 *
 *  Created on: Apr 20, 2026
 *      Author: Gabriela
 */

#ifndef INC_CAMBIARCOLOR_H_
#define INC_CAMBIARCOLOR_H_

#include "main.h" // Necesario para que reconozca los tipos de la HAL

// Definimos los valores del PWM para el MG996R de 360 grados
#define SERVO_STOP  1500
#define SERVO_VEL   1550 // Ajusta este valor según la prisa que tengas

// Prototipos de las funciones
void GRIPPER_Init(TIM_HandleTypeDef *htim);
void GRIPPER_SeleccionarColor(uint8_t color_destino);
void GRIPPER_HallCallback(uint16_t GPIO_Pin);

#endif /* INC_CAMBIARCOLOR_H_ */
