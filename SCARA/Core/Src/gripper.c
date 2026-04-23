/* gripper.c */
#include "gripper.h" // INCLUYE DEFINICIONES PROPIAS

static uint8_t  moving        = 0; // FLAG: 1 SI EL SERVO GIRA
static uint32_t move_start_ms = 0; // MARCA TIEMPO DE ARRANQUE

static void set_servo_pwm(uint32_t pulse) // AJUSTA EL ANCHO DE PULSO
{
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, pulse); // ESCRIBE EN REGISTRO CCR1
}

void Gripper_Init(void) // CONFIGURA ESTADO INICIAL
{
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1); // ENCIENDE PERIFÉRICO PWM
    set_servo_pwm(SERVO_STOP); // ASEGURA QUE EMPIECE PARADO
    moving = 0; // RESETEA ESTADO MOVIMIENTO
}

void Gripper_MoveNext(void) // ACTIVA EL GIRO DEL BRAZO
{
    if (moving) return; // IGNORA SI YA SE ESTÁ MOVIENDO

    set_servo_pwm(SERVO_RUN); // ENVÍA COMANDO DE MARCHA
    moving = 1; // MARCA INICIO DE ACCIÓN
    move_start_ms = HAL_GetTick(); // GUARDA MOMENTO EXACTO DEL CLICK
}

void Gripper_Update(void) // GESTIONA LA PARADA AUTOMÁTICA
{
    // COMPRUEBA SI TOCA PARAR SEGÚN EL TIEMPO DEFINIDO
    if (moving && (HAL_GetTick() - move_start_ms >= GIRO_120_MS))
    {
        set_servo_pwm(SERVO_STOP); // MANDA PULSO DE FRENADO
        moving = 0; // LIBERA PARA NUEVA PULSACIÓN
    }
}
