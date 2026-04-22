/* gripper.c */
#include "gripper.h"

static uint8_t  moving        = 0;
static uint32_t move_start_ms = 0;

static void set_servo_pwm(uint32_t pulse)
{
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, pulse);
}

void Gripper_Init(void)
{
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
    set_servo_pwm(SERVO_STOP);
    moving = 0;
}

void Gripper_MoveNext(void)
{
    if (moving) return;

    set_servo_pwm(SERVO_RUN);
    moving        = 1;
    move_start_ms = HAL_GetTick();
}

void Gripper_Update(void)
{
    if (moving && (HAL_GetTick() - move_start_ms >= GIRO_120_MS))
    {
        set_servo_pwm(SERVO_STOP);
        moving = 0;
    }
}
