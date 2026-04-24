// motion.cpp
#include "motion.h"
#include <math.h>

volatile int32_t encoder_BR1 = 0; // PASOS MOTOR 1
volatile int32_t encoder_BR2 = 0; // PASOS MOTOR 2
volatile int32_t encoder_BA  = 0; // PASOS MOTOR Z

static volatile uint8_t prev_BA_A  = 0; // PARA EJE Z

// ─── ISR EJE Z (Pines A8, A9) ──────────────────────────────────────────
// ESTE SE QUEDA PORQUE attachInterrupt NO LLEGA A ESTOS PINES EN MEGA
ISR(PCINT2_vect) { 
    uint8_t a = digitalRead(PIN_ENC_BA_A);
    uint8_t b = digitalRead(PIN_ENC_BA_B);
    if (a != prev_BA_A) {
        encoder_BA += (a == b) ? -1 : 1; // ACTUALIZA PASOS Z
        prev_BA_A = a;
    }
}

// ─── INIT ENCODERS ────────────────────────────────────────────────────
void Encoders_Init(void) {
    // MOTOR 1 (BR1)
    pinMode(PIN_ENC_BR1_A, INPUT_PULLUP);
    pinMode(PIN_ENC_BR1_B, INPUT_PULLUP);
    // USAMOS LAMBDAS PARA QUE EL CODIGO SEA TOP Y NO DE ERRORES
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_BR1_A), [](){
        encoder_BR1 += (digitalRead(PIN_ENC_BR1_A) == digitalRead(PIN_ENC_BR1_B)) ? -1 : 1;
    }, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_BR1_B), [](){
        encoder_BR1 += (digitalRead(PIN_ENC_BR1_A) != digitalRead(PIN_ENC_BR1_B)) ? -1 : 1;
    }, CHANGE);

    // MOTOR 2 (BR2)
    pinMode(PIN_ENC_BR2_A, INPUT_PULLUP);
    pinMode(PIN_ENC_BR2_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_BR2_A), [](){
        encoder_BR2 += (digitalRead(PIN_ENC_BR2_A) == digitalRead(PIN_ENC_BR2_B)) ? -1 : 1;
    }, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_BR2_B), [](){
        encoder_BR2 += (digitalRead(PIN_ENC_BR2_A) != digitalRead(PIN_ENC_BR2_B)) ? -1 : 1;
    }, CHANGE);

    // MOTOR Z (BA) - CONFIGURACION PCINT
    pinMode(PIN_ENC_BA_A, INPUT_PULLUP);
    pinMode(PIN_ENC_BA_B, INPUT_PULLUP);
    PCICR  |= _BV(PCIE2); // ACTIVA EL BANCO 2
    PCMSK2 |= _BV(PCINT16) | _BV(PCINT17); // PINES A8 Y A9
    prev_BA_A = digitalRead(PIN_ENC_BA_A);
    
    Serial.println("ENCODERS: ONLINE 💅"); // AVISO CONSOLA
}


// ─── LECTURA ENCODER ──────────────────────────────────────────────────────────
int32_t Leer_Pasos_Encoder(int numero_motor) {
    switch (numero_motor) {
        case 1: return encoder_BR1; // DEVUELVE PASOS MOTOR 1
        case 2: return encoder_BR2; // DEVUELVE PASOS MOTOR 2
        case 3: return encoder_BA;  // DEVUELVE PASOS MOTOR Z
        default: return 0;          // MOTOR INVÁLIDO
    }
}

// ─── PID ──────────────────────────────────────────────────────────────────────
void PID_Init(PID_Controller* pid, float p, float i, float d, float umbral) {
    pid->kp = p;              // GANANCIA PROPORCIONAL
    pid->ki = i;              // GANANCIA INTEGRAL
    pid->kd = d;              // GANANCIA DERIVATIVA
    pid->umbralFriccion = umbral; // MÍNIMO VOLTAJE ÚTIL
    pid->err = 0;             // RESETEA ERROR
    pid->err_ini = 0;         // RESETEA ERROR ANTERIOR
    pid->integral = 0;        // RESETEA ACUMULADOR
    pid->setpoint = 0;        // RESETEA OBJETIVO
    pid->actual = 0;          // RESETEA POSICIÓN
    pid->voltage = 0;         // RESETEA SALIDA
}

float PID_Calcular(PID_Controller* pid, float valorActual, float objetivo, float dt) {
    pid->actual   = valorActual;              // GUARDA POSICIÓN REAL
    pid->setpoint = objetivo;                 // GUARDA OBJETIVO

    pid->err = pid->setpoint - pid->actual;   // CALCULA ERROR

    pid->integral += pid->err * dt;           // ACUMULA INTEGRAL
    if (pid->integral >  100) pid->integral =  100; // ANTI-WINDUP POSITIVO
    if (pid->integral < -100) pid->integral = -100; // ANTI-WINDUP NEGATIVO

    float derivative = (pid->err - pid->err_ini) / dt; // TÉRMINO DERIVATIVO

    pid->voltage = (pid->kp * pid->err)       // TÉRMINO P
                 + (pid->ki * pid->integral)   // TÉRMINO I
                 + (pid->kd * derivative);     // TÉRMINO D

    if (pid->voltage >  PWM_MOTOR_TOP) pid->voltage =  PWM_MOTOR_TOP; // SATURA POSITIVO
    if (pid->voltage < -PWM_MOTOR_TOP) pid->voltage = -PWM_MOTOR_TOP; // SATURA NEGATIVO

    pid->err_ini = pid->err; // GUARDA ERROR PARA PRÓXIMA ITERACIÓN

    // COMPENSACIÓN FRICCIÓN ESTÁTICA
    if (pid->voltage > 0 && pid->voltage <  pid->umbralFriccion) pid->voltage =  pid->umbralFriccion; // EMPUJA POSITIVO
    if (pid->voltage < 0 && pid->voltage > -pid->umbralFriccion) pid->voltage = -pid->umbralFriccion; // EMPUJA NEGATIVO

    return pid->voltage; // DEVUELVE SALIDA
}

float PID_GetVoltaje(PID_Controller* pid) {
    return pid->voltage; // DEVUELVE ÚLTIMO VOLTAJE CALCULADO
}

// ─── ACTUALIZAR EJE MOTOR ─────────────────────────────────────────────────────
void Update_Motor_Axis(PID_Controller* pid, float posicionActual, float objetivo, float dt,
                       uint8_t pin_pwm, uint8_t pin_in1, uint8_t pin_in2, uint16_t pwm_top) {

    float voltaje = PID_Calcular(pid, posicionActual, objetivo, dt); // CALCULA PID
    uint16_t pwm  = (uint16_t)fabsf(voltaje);                        // VALOR ABSOLUTO PARA DUTY

    if (voltaje > 0) {
        digitalWrite(pin_in1, HIGH); // DIRECCIÓN ADELANTE IN1
        digitalWrite(pin_in2, LOW);  // DIRECCIÓN ADELANTE IN2
    } else {
        digitalWrite(pin_in1, LOW);  // DIRECCIÓN ATRÁS IN1
        digitalWrite(pin_in2, HIGH); // DIRECCIÓN ATRÁS IN2
    }

    // ESCRIBE PWM SEGÚN CANAL — Timer1: OCR1A=pin11, OCR1B=pin12, OCR1C=pin13
    if      (pin_pwm == PIN_PWM_BR1) OCR1A = pwm; // CANAL A TIMER1
    else if (pin_pwm == PIN_PWM_BR2) OCR1B = pwm; // CANAL B TIMER1
    else if (pin_pwm == PIN_PWM_BA)  OCR1C = pwm; // CANAL C TIMER1
}

// ─── HOMING ───────────────────────────────────────────────────────────────────
void Homing(void) {
    // ARRANCA MOTORES HACIA POSICIÓN INICIAL
    OCR1A = HOMING_PWM_J1; // PWM MOTOR 1
    OCR1B = HOMING_PWM_J2; // PWM MOTOR 2
    OCR1C = HOMING_PWM_Z;  // PWM MOTOR Z

    // DIRECCIÓN HACIA ORIGEN
    digitalWrite(PIN_BR1_IN1, HIGH); digitalWrite(PIN_BR1_IN2, LOW); // DIR BR1
    digitalWrite(PIN_BR2_IN1, HIGH); digitalWrite(PIN_BR2_IN2, LOW); // DIR BR2
    digitalWrite(PIN_BA_IN1,  HIGH); digitalWrite(PIN_BA_IN2,  LOW); // DIR BA

    delay(HOMING_TIMEOUT_MS); // ESPERA TIEMPO FIJO (10S)

    // PARA MOTORES
    OCR1A = 0; // PARA MOTOR 1
    OCR1B = 0; // PARA MOTOR 2
    OCR1C = 0; // PARA MOTOR Z

    // DECLARA ESTE PUNTO COMO ORIGEN
    encoder_BR1 = 0; // CERO ENCODER 1
    encoder_BR2 = 0; // CERO ENCODER 2
    encoder_BA  = 0; // CERO ENCODER Z
}