#pragma once
#ifndef MOTION_H
#define MOTION_H

#include "main.h" // Obligatorio para los tipos de datos de STM32

//parámetros para el homing
#define HOMING_PWM_J1      180
#define HOMING_PWM_J2      150
#define HOMING_PWM_Z       160
#define HOMING_TIMEOUT_MS  10000


//declarar variables externas para que motion.c pueda usarlas
extern volatile uint8_t homingJ1_done;
extern volatile uint8_t homingJ2_done;
extern volatile uint8_t homingZ_done;

//declaro estructura del PID
typedef struct {
    float kp, ki, kd;
    float setpoint;
    float actual;
    float err, err_ini, integral;
    float voltage;
    float umbralFriccion;
} PID_Controller;


//declcaración de funciones
void PID_Init(PID_Controller* pid, float p, float i, float d, float umbral); //funcion para iniciar el PID
float PID_Calcular(PID_Controller* pid, float valorActual, float objetivo, float dt);  //funcion para llegar hasta el objetivo del pid
float PID_GetVoltaje(PID_Controller* pid); //funcion que devuelve el voltaje mandado

//funcion que atucaliza el eje del motor con el objetivo deseado
void Update_Motor_Axis(PID_Controller* pid, float posicionActual, float objetivoMaria, float dt,
    TIM_HandleTypeDef* htim, uint32_t canal,
    GPIO_TypeDef* port, uint16_t pin1, uint16_t pin2);

void Homing(void);

#endif // MOTION_H
