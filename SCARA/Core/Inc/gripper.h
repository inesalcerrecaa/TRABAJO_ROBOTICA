/* gripper.h */
#ifndef GRIPPER_H
#define GRIPPER_H

#include "stm32f4xx_hal.h"
/* gripper.h */
#define SERVO_STOP      1500
#define SERVO_RUN       1750
#define GIRO_120_MS      300   // ← cambia este valor
#define HALL_TIMEOUT_MS 2000   // lo dejamos por seguridad

extern TIM_HandleTypeDef htim5;

void Gripper_Init(void);
void Gripper_MoveNext(void);
void Gripper_HallDetected(void);
void Gripper_Update(void);

#endif
