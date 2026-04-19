/*
 * kinematics.h
 *
 *  Created on: Mar 30, 2026
 *      Author: maria
 */

#ifndef INC_KINEMATICS_H_
#define INC_KINEMATICS_H_
/* USER CODE BEGIN Includes */
#include <stdint.h>
/* USER CODE BEGIN Includes */

/* LOS PARAMETROS FÍSICOS DEL ROBOT (en mm) */
#define L1 200.0f /* Longitud del antebrazo*/
#define L2 200.0f /*Longitud del brazo*/
#define L_z 30.0f /* Caida vertical del rotulador*/
#define L_xy 163.0f /*Extensión horizontal del rotu*/

/* RESULTADO DE LA CINEMÁTICA INVERSA */
typedef struct {
    float q1_rad;    /* Ángulo del hombro (rad)                */
    float q2_mm;     /* Traslación del tronco (mm)             */
    float q3_rad;    /* Ángulo del codo (rad)                  */
    uint8_t valid;   /* 1 = punto alcanzable, 0 = fuera rango  */
} IK_Result_t;

/* PROTOTIPO DE FUNCIONES PÚBLICAS */

/**
 * @brief  Llamar UNA SOLA VEZ al cambiar de color (q4 fijo).
 *         Precalcula L_eff y alpha que se reutilizan en cada frame.
 * @param  q4_rad  Ángulo actual del servo del gripper (rad)
 */
void IK_Update_Effective_Arm(float q4_rad);

/**
 * @brief  Cinemática Inversa. Llama en cada frame del bucle de
 *         trayectorias con las coordenadas locales del lienzo.
 * @param  x_mm, y_mm, z_mm  Coordenadas objetivo en mm
 * @return IK_Result_t  Ángulos articulares + flag de validez
 */
IK_Result_t Mover_Robot_A_Coordenada(float x_mm, float y_mm, float z_mm);
#endif /* INC_KINEMATICS_H_ */
