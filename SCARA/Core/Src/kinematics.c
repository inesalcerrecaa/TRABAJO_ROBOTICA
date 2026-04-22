/*
 * kinematics.c -> cinematica inversa del robot SCARA 4-Dof
 *
 *  Created on: Mar 30, 2026
 *      Author: maria
 */

/* USER CODE BEGIN Includes */
#include "kinematics.h"
#include <math.h>      /* sqrtf, atan2f, acosf — usa versiones float */
/* USER CODE END Includes */

/* USER CODE BEGIN Private Variables */
/* Variables internas del brazo efectivo (se calculan con IK_Update_Effective_Arm) */
static float s_L_eff = L2 + L_xy;  /* Valor inicial con q4=0 */
static float s_alpha  = 0.0f;
/* USER CODE END Private Variables */

/* IK_Update_Effective_Arm
 * Llama SOLO cuando cambia q4 (cambio de color en reposo). */
void IK_Actualizar_Brazo_Efectivo(float q4_rad){
	/* USER CODE BEGIN IK_Actualizar_Brazo_Efectivo */
	/* Primero calculamos la longitud del brazo efectivo (L2+ rotulador como un sólido rígido)*/
	s_L_eff = sqrtf((L2*L2)+(L_xy*L_xy)+(2.0f * L2*L_xy*cosf(q4_rad)));
	/* Ahora calculamos el ángulo de desviación geométrica alpha */
	s_alpha = atan2f (L_xy *sinf(q4_rad), L2+L_xy*cosf(q4_rad));

	/* USER CODE END IK_Actualizar_Brazo_Efectivo */
}

/*IK_Resolver_Movimiento
 * LLama en cada frame del bucle de trayectorias.
 * x_mm, y_mm = coordenadas en el PLANO XY local (base del robot).
 * z_mm = coordenada Z del punto objetivo local.
 */

IK_Result_t IK_Resolver_Movimiento (float x_mm, float y_mm, float z_mm){
	/* USER CODE BEGIN IK_Resolver_Movimiento */
	 IK_Result_t resultado={0.0f,0.0f,0.0f,0.0f}; /* valid=0 por defecto*/
	 /* Segundo paso: el eje Z (tronco que va desacoplado del plano XY) */
	 resultado.q2_mm=z_mm+L_z;

	 /* Ahora, plano xy - comprobación de rangi antes de acos */
	 float r_sq=(x_mm*x_mm)+(y_mm*y_mm);

	 /*comprobamos que el arqumento del arcos este entre -1 y 1 */
	 float cos_q3_prima= (r_sq-(L1*L1)-(s_L_eff*s_L_eff))/(2.0f*L1*s_L_eff);

	 /*protección de singularidad: que el punto esté fuera del espacio de trabajo */
	 if (cos_q3_prima>1.0f) {cos_q3_prima=1.0f;}
	 if (cos_q3_prima<-1.0f) {cos_q3_prima=-1.0f;}
	 /*calculamos el ángulo virtual del codo (q3_prima) y real (q3) */
	 float q3_prima = acosf(cos_q3_prima); /*está entre 0 y pi radianes*/
	 resultado.q3_rad=q3_prima - s_alpha; /*ESTE ES EL ÁNGULO REAL DEL MOTOR*/
	 /*calculamos el ángulo del hombro */
	 resultado.q1_rad=atan2f(y_mm,x_mm)- atan2f(s_L_eff*sinf(q3_prima),L1+s_L_eff*cosf(q3_prima));
	 /* q4 para perpendicularidad: lienzo a la derecha (+X = 0 rad)
	    Dirección absoluta del rotu = q1+q3+q4 → para apuntar a +X: total = 0
	    por tanto q4_perp = 0 - (q1+q3)                                       */
	 #define IK_PI  3.14159265f

	 resultado.q4_perp_rad = 0.0f - (resultado.q1_rad + resultado.q3_rad);
	/*si pasa todo esto significa que el punto será válido por lo que: */
	 resultado.valid=1;

	 return resultado;

	 /* USER CODE END IK_Resolver_Movimiento */
}
