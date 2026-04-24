// gripper.h
#pragma once
#include <Arduino.h>     // REEMPLAZA stm32f4xx_hal.h

#define SERVO_STOP      1500  // PULSO PARADA (µS)
#define SERVO_RUN       1750  // PULSO MARCHA (µS)
#define GIRO_120_MS     1100  // DURACIÓN GIRO 120° EN MS
#define SAFETY_STOP_MS  2000  // PARADA FORZADA SI NO TERMINA EN 2S

void Gripper_Init(void);     // INICIALIZA PWM
void Gripper_MoveNext(void); // INICIA MOVIMIENTO
void Gripper_Update(void);   // PARADA AUTOMÁTICA POR TIEMPO