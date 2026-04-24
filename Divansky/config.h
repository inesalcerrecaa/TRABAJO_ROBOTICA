#pragma once
#include <Arduino.h>

// PWM MOTORES
extern const uint8_t PIN_PWM_BR1;    // PIN PWM MOTOR 1
extern const uint8_t PIN_PWM_BR2;    // PIN PWM MOTOR 2
extern const uint8_t PIN_PWM_BA;     // PIN PWM MOTOR Z
extern const uint8_t PIN_PWM_SERVO;  // PIN PWM SERVO GRIPPER

// DIRECCION MOTORES
extern const uint8_t PIN_BR1_IN1, PIN_BR1_IN2; // PINES GIRO MOTOR 1
extern const uint8_t PIN_BR2_IN1, PIN_BR2_IN2; // PINES GIRO MOTOR 2
extern const uint8_t PIN_BA_IN1,  PIN_BA_IN2;  // PINES GIRO MOTOR Z

// ENCODERS
extern const uint8_t PIN_ENC_BR1_A, PIN_ENC_BR1_B; // FEEDBACK MOTOR 1
extern const uint8_t PIN_ENC_BR2_A, PIN_ENC_BR2_B; // FEEDBACK MOTOR 2
extern const uint8_t PIN_ENC_BA_A,  PIN_ENC_BA_B;  // FEEDBACK MOTOR Z

// BOTONES
extern const uint8_t PIN_BTN_COLOR;   // PIN BOTON COLOR
extern const uint8_t PIN_BTN_LINEA;   // PIN BOTON LINEA
extern const uint8_t PIN_BTN_CIRCULO; // PIN BOTON CIRCULO
extern const uint8_t PIN_BTN_RESET;   // PIN BOTON RESET

// PARAMETROS
extern const uint32_t SERIAL_BAUD;    // BAUDIOS SERIAL
extern const uint16_t PWM_MOTOR_TOP;  // RESOLUCION PWM MOTORES
extern const uint16_t PWM_SERVO_TOP;  // RESOLUCION PWM SERVO

// ─── I2C PANTALLA (Pines fijos en Mega) ───────────────────
// SDA → PIN 20  // DATOS DE LA PANTALLA
// SCL → PIN 21  // RELOJ DE LA PANTALLA