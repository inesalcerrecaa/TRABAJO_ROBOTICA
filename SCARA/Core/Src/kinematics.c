/*
 * kinematics.c -> cinematica inversa del robot SCARA 4-Dof
 *
 *  Created on: Mar 30, 2026
 *      Author: maria
 */

/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <math.h>      /* sqrtf, atan2f, acosf — usa versiones float */
#include "kinematics.h"
#include "motion.h"
#include "gripper.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private Variables */
/* Variables internas del brazo efectivo (se calculan con IK_Update_Effective_Arm) */
static float s_L_eff = L2 + L_xy;
/* Valor inicial con q4=0 */
static float s_alpha  = 0.0f;
/* Variables globales que Alba puede leer para su PID:
   volatile porque se escriben desde trayectorias y se leen desde control */
volatile float objetivo_q1 = 0.0f;
volatile float objetivo_q3 = 0.0f;
volatile float objetivo_z  = 0.0f;
/* USER CODE END Private Variables */
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LIENZO_X_MAX 300
#define LIENZO_Y_MAX 400
#define PI 3.14159265f
/* USER CODE END PD */

/*USER CODE BEGIN 0*/
void Mover_Eje_Z (int altura_mm){
	(void)altura_mm;
}

//Funciones fantasma, cuando Maria y Alba termineis, conectareis vuestro codigo aqui dentro
void Mover_Robot_A_Coordenada (int x, int y){
	 //BLOQUE DE MARÍA: Cinemática Inversa

	    // El lienzo está a 45°: Sofía ya programó que Z = Y en la compensación.
	    // La Z que necesita la IK se obtiene directamente de la coordenada Y del lienzo.
	    float z_mm = (float)y;

	    IK_Result_t ik = IK_Resolver_Movimiento((float)x, (float)y, z_mm);

	    if (!ik.valid) {
	        // Punto fuera de rango: no mover, esperar siguiente frame
	        return;
	    }

	    // --- BLOQUE DE ALBA: PID y motores ---
	    // Alba rellena estas funciones en pid_core.c y pwm_motors.c
	   // PID_SetTarget_Hombro(ik.q1_rad);   // → TIM1_CH1 (PE9) + DIR_J1 (PE7/PE8)
	   // PID_SetTarget_Codo(ik.q3_rad);     // → TIM1_CH2 (PE11) + DIR_J2 (PE10/PE12)
	   // Mover_Eje_Z(ik.q2_mm);            // → TIM1_CH3 (PE13) + DIR_Z (PE14/PE15)
	//AVISO A ALBA: al trazar las lineas, los motores X, Y, Z tienen que moverse a la vez
}

//Funcion para el eje Z (la torre principal)
/* void Mover_Eje_Z (int altura_mm){
*	//aqui (segun gemini) alba pone el codigo del motor de la torre z
*}
*/

void Bajar_Rotulador(int y_actual){
	//como esta a 45º, la altura del papel es proporcional a y
	int z_papel = y_actual;

	Mover_Eje_Z(z_papel);
	//Esperamos medio segundo: asi dejamos que el motor termine de llegar y las vibraciones se calmen
	//antes de empezar a dibujar
	HAL_Delay(500);
}

void Subir_Rotulador(int y_actual){
	//Subimos la estructura 20mm (por ejemplo), para asegurarnos de que el rotu no roza el lienzo
	//al moverse
	int z_papel = y_actual;

	Mover_Eje_Z(z_papel + 20);
	//Esperamos a que termine de subir
	HAL_Delay(500);
}

//Funciones mias (Sofia) para que dibuje circulos y lineas con posiciones aleatorias en el lienzo
void Dibujar_Linea_Aleatoria(void){
	//genero dos puntos aleatorios dentro de los limites
	int inicio_x = rand() % LIENZO_X_MAX;
	int inicio_y = rand() % LIENZO_Y_MAX;

	int fin_x = rand() % LIENZO_X_MAX;
	int fin_y = rand() % LIENZO_Y_MAX;

	//Ejecuto la secuencia de dibujo
	Subir_Rotulador(inicio_y); //nos aseguramos de no manchar mientras vamos al siguiente punto
	Mover_Robot_A_Coordenada(inicio_x, inicio_y); //vamos al punto A
	Bajar_Rotulador(inicio_y); //apoyamos el rotu en el lienzo
	Mover_Robot_A_Coordenada(fin_x, fin_y); //trazamos la recta hasta el punto B
	Subir_Rotulador(fin_y); //terminamos y levantamos usando la altura del final
}

void Dibujar_Circulo_Aleatorio(void){
	//Genero un centro aleatorio, dejando un margen de 10mm
	int centro_x = 10 + (rand() % (LIENZO_X_MAX - 20));
	int centro_y = 10 + (rand() % (LIENZO_Y_MAX - 20));

	//Calculamos la distancia a los 4 bordes del lienzo
	int dist_izq = centro_x;
	int dist_der = LIENZO_X_MAX - centro_x;
	int dist_abajo = centro_y;
	int dist_arriba = LIENZO_Y_MAX - centro_y;

	//Buscamos cual es la distancia mas pequeña (ese sera nuestro limite)
	int radio_maximo = dist_izq;
	if (dist_der < radio_maximo) radio_maximo = dist_der;
	if (dist_abajo < radio_maximo) radio_maximo = dist_abajo;
	if (dist_arriba < radio_maximo) radio_maximo = dist_arriba;

	//Elegimos un radio aleatorio seguro (restando 5mm extra de margen)
	int radio_dibujo = 5 + (rand() % (radio_maximo - 5));

	//Ejecuto la secuencia de dibujo
	//Subimos usando la altura del centro para ir hasta allí sin rozar
	Subir_Rotulador(centro_y);

	//Variables para mi interpolacion
	int puntos_resolucion = 36; // 36 pasos, rectas de 10 grados
	float angulo_paso = (2*PI) / puntos_resolucion; //convierto los pasos a radianes

	//Calculo el punto 0 para empezar a trazar
	int primer_x = centro_x + (int) (radio_dibujo * cos(0.0f));
	int primer_y = centro_y + (int) (radio_dibujo * sin(0.0f));

	//Vamos al borde derecho del círculo para empezar a dibujar
	//Mover_Robot_A_Coordenada(centro_x + radio_dibujo, centro_y);
	Mover_Robot_A_Coordenada(primer_x, primer_y);
	//Bajar_Rotulador(centro_y);
	Bajar_Rotulador(primer_y);

	//Bucle de interpolación
	for (int i = 1; i <= puntos_resolucion; i++){
		float angulo_actual = i * angulo_paso;

		//fórmulas trigonometricas
		int punto_x = centro_x + (int) (radio_dibujo * cos(angulo_actual));
		int punto_y = centro_y + (int) (radio_dibujo * sin(angulo_actual));

		//le mandamos la orden al cerebro del robot
		Mover_Robot_A_Coordenada(punto_x, punto_y);

		//pequeño paron para que la placa tenga tiempo de mover los motores
		HAL_Delay(20);
	}

	//Terminamos y levantamos
	Subir_Rotulador(centro_y);

}
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

	 resultado.q4_perp_rad = 0.0f - (resultado.q1_rad + resultado.q3_rad);
	/*si pasa todo esto significa que el punto será válido por lo que: */
	 resultado.valid=1;

	 objetivo_q1=resultado.q1_rad;
	 objetivo_q3=resultado.q3_rad;
	 objetivo_z=resultado.q2_mm;
	 return resultado;

	 /* USER CODE END IK_Resolver_Movimiento */
}
float IK_Calcular_q4_Perpendicular(float x_centro_mm, float y_centro_mm)
{
    /* USER CODE BEGIN IK_Calcular_q4_Perpendicular */

    /* 1. Resolvemos la IK en el centro con q4=0 para obtener q1 y q3 reales */
    IK_Actualizar_Brazo_Efectivo(0.0f);
    IK_Result_t ref = IK_Resolver_Movimiento(x_centro_mm, y_centro_mm, 0.0f);

    /* 2. q4 necesario para que el rotulador apunte a +X (lienzo a la derecha)
          q1 + q3 + q4 = 0  →  q4 = -(q1+q3)                               */
    float q4_optimo = 0.0f - (ref.q1_rad + ref.q3_rad);

    /* 3. Normalizar a [-π, π] por si la suma se sale del rango */
    #define IK_PI 3.14159265f
    while (q4_optimo >  PI) q4_optimo -= 2.0f * PI;
    while (q4_optimo < -PI) q4_optimo += 2.0f * PI;

    return q4_optimo;

    /* USER CODE END IK_Calcular_q4_Perpendicular */
}
/*USER CODE END 0*/
