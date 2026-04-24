#include "config.h"

// PWM MOTORES (TIMER 1 Y 5)
const uint8_t PIN_PWM_BR1    = 11;    // PWM MOTOR 1
const uint8_t PIN_PWM_BR2    = 12;    // PWM MOTOR 2
const uint8_t PIN_PWM_BA     = 13;    // PWM MOTOR Z

const uint8_t PIN_PWM_SERVO  = 46;    // PWM SERVO GRIPPER
extern const uint16_t PWM_MOTOR_TOP  = 799;   // TOPE PARA 20KHZ
extern const uint16_t PWM_SERVO_TOP  = 39999; // TOPE PARA 50HZ

// DIRECCION MOTORES
const uint8_t PIN_BR1_IN1    = 22;    // GIRO MOTOR 1A
const uint8_t PIN_BR1_IN2    = 23;    // GIRO MOTOR 1B
const uint8_t PIN_BR2_IN1    = 24;    // GIRO MOTOR 2A
const uint8_t PIN_BR2_IN2    = 25;    // GIRO MOTOR 2B
const uint8_t PIN_BA_IN1     = 26;    // GIRO MOTOR ZA
const uint8_t PIN_BA_IN2     = 27;    // GIRO MOTOR ZB

// ENCODERS (A8/A9 TIENEN PCINT PARA EL EJE Z)
const uint8_t PIN_ENC_BR1_A  = 3;     // INT4 HARDWARE
const uint8_t PIN_ENC_BR1_B  = 2;     // INT5 HARDWARE
const uint8_t PIN_ENC_BR2_A  = 19;    // INT2 HARDWARE
const uint8_t PIN_ENC_BR2_B  = 18;    // INT3 HARDWARE
const uint8_t PIN_ENC_BA_A   = A8;    // PCINT16 PARA EJE Z
const uint8_t PIN_ENC_BA_B   = A9;    // PCINT17 PARA EJE Z

// BOTONES
const uint8_t PIN_BTN_COLOR   = 30;   // BOTON COLOR
const uint8_t PIN_BTN_LINEA   = 31;   // BOTON LINEA
const uint8_t PIN_BTN_CIRCULO = 32;   // BOTON CIRCULO
const uint8_t PIN_BTN_RESET   = 33;   // BOTON RESET

// PANTALLA I2C
// SDA -> 20
// SCL -> 21