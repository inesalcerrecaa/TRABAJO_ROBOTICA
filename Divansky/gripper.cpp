// gripper.cpp
#include "gripper.h" // INCLUYE DEFINICIONES PROPIAS
#include "config.h"  // NECESARIO PARA PIN_PWM_SERVO

static uint8_t  moving        = 0; // FLAG: 1 SI EL SERVO GIRA
static uint32_t move_start_ms = 0; // MARCA TIEMPO DE ARRANQUE

static void set_servo_pwm(uint16_t pulse_us) // AJUSTA ANCHO DE PULSO EN µS
{
    OCR5A = (uint16_t)(pulse_us * 2UL); // 0.5µS/TICK → MULTIPLICA POR 2
}

void Gripper_Init(void) // CONFIGURA ESTADO INICIAL
{
    TCCR5A = _BV(COM5A1) | _BV(WGM51);              // NON-INVERTING, FAST PWM
    TCCR5B = _BV(WGM53)  | _BV(WGM52) | _BV(CS51); // FAST PWM, PRESCALER /8
    ICR5   = PWM_SERVO_TOP;                          // TOPE 39999 → 50HZ
    pinMode(PIN_PWM_SERVO, OUTPUT);                  // PIN 46 COMO SALIDA
    set_servo_pwm(SERVO_STOP);                       // ARRANCA PARADO
    moving = 0;                                      // RESETEA FLAG
}

void Gripper_MoveNext(void) // ACTIVA EL GIRO DEL BRAZO
{
    if (moving) return;           // IGNORA SI YA SE MUEVE
    set_servo_pwm(SERVO_RUN);     // ENVÍA PULSO DE MARCHA
    moving        = 1;            // MARCA INICIO DE ACCIÓN
    move_start_ms = millis();     // GUARDA MOMENTO DEL CLICK
}

void Gripper_Update(void) // GESTIONA PARADA AUTOMÁTICA
{
    if (!moving) return;                                        // NADA QUE HACER

    uint32_t elapsed = millis() - move_start_ms;               // MS DESDE ARRANQUE

    if (elapsed >= GIRO_120_MS)                                // TIEMPO GIRO CUMPLIDO
    {
        set_servo_pwm(SERVO_STOP);                             // PARA EL SERVO
        moving = 0;                                            // LIBERA PARA NUEVO CICLO
    }
    else if (elapsed >= SAFETY_STOP_MS)                        // TIMEOUT SEGURIDAD
    {
        set_servo_pwm(SERVO_STOP);                             // PARADA FORZADA
        moving = 0;                                            // LIBERA ESTADO
    }
}