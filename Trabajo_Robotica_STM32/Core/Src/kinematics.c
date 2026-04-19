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
static float s_L_eff = L2 + L_XY;  /* Valor inicial con q4=0 */
static float s_alpha  = 0.0f;
/* USER CODE END Private Variables */


