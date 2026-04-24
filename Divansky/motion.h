// motion.h
#pragma once
#include <Arduino.h> // REEMPLAZA main.h + stm32f4xx_hal.h
#include "config.h"  // PINES MEGA

// PARÁMETROS HOMING
#define HOMING_PWM_J1      180    // DUTY CYCLE HOMING MOTOR 1
#define HOMING_PWM_J2      150    // DUTY CYCLE HOMING MOTOR 2
#define HOMING_PWM_Z       160    // DUTY CYCLE HOMING MOTOR Z
#define HOMING_TIMEOUT_MS  10000  // LÍMITE SEGURIDAD HOMING

// CONVERSIÓN ENCODER
#define MM_POR_VUELTA      8.0f    // CONFIRMAR CON GABRI
#define PASOS_POR_VUELTA   3960.0f // 11 PULSOS × 4 CUADRATURA × 90 REDUCTORA
#define PASOS_A_MM         (MM_POR_VUELTA / PASOS_POR_VUELTA) // FACTOR LINEAL
#define PASOS_A_RAD        (2.0f * 3.14159265f / PASOS_POR_VUELTA) // FACTOR ANGULAR

// CONTADORES ENCODER — INCREMENTADOS POR ISR
extern volatile int32_t encoder_BR1;
extern volatile int32_t encoder_BR2;
extern volatile int32_t encoder_BA;

// ESTRUCTURA PID
typedef struct {
    float kp, ki, kd;       // GANANCIAS
    float setpoint;          // OBJETIVO
    float actual;            // POSICIÓN REAL
    float err, err_ini;      // ERROR ACTUAL Y ANTERIOR
    float integral;          // ACUMULADOR INTEGRAL
    float voltage;           // SALIDA PID
    float umbralFriccion;    // COMPENSACIÓN ESTÁTICA
} PID_Controller;

// PROTOTIPOS
void Encoders_Init(void);
int32_t Leer_Pasos_Encoder(int numero_motor);

void PID_Init(PID_Controller* pid, float p, float i, float d, float umbral);
float PID_Calcular(PID_Controller* pid, float valorActual, float objetivo, float dt);
float PID_GetVoltaje(PID_Controller* pid);

void Update_Motor_Axis(PID_Controller* pid, float posicionActual, float objetivo, float dt,
                       uint8_t pin_pwm, uint8_t pin_in1, uint8_t pin_in2, uint16_t pwm_top);

void Homing(void);