/* gripper.h */
#ifndef GRIPPER_H // EVITA INCLUSIÓN DUPLICADA
#define GRIPPER_H // DEFINE EL MÓDULO

#include "stm32f4xx_hal.h" // LIBRERÍAS BASE HAL

/* gripper.h */
#define SERVO_STOP      1500   // VALOR PWM PARADA
#define SERVO_RUN       1750   // VALOR PWM MARCHA
#define GIRO_120_MS     1100   // TIEMPO GIRO 120 GRADOS
#define HALL_TIMEOUT_MS 2000   // TIEMPO LÍMITE SEGURIDAD

extern TIM_HandleTypeDef htim5; // INSTANCIA TIMER EXTERNO

void Gripper_Init(void);        // INICIALIZA PWM
void Gripper_MoveNext(void);    // INICIA MOVIMIENTO
void Gripper_HallDetected(void); // DETECCIÓN POR IMÁN
void Gripper_Update(void);      // CONTROL POR TIEMPO

#endif // FIN DE CABECERA
