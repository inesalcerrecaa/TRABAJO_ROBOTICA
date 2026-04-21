/*
 * trajectories.c
 *
 *  Created on: Mar 30, 2026
 *      Author: Sofia
 */

#include "main.h"
/*USER CODE BEGIN Includes*/
#include <stdlib.h> //para la funcion rand
#include <math.h> //para calcular la interpolacion circular
#include "kinematics.h"
#include "motion.h"
#include "gripper.h"
/*USER CODE END Includes*/

/*USER CODE BEGIN PD*/
#define LIENZO_X_MAX 300
#define LIENZO_Y_MAX 400
#define PI 3.14159265
/*USER CODE END PD*/

/*USER CODE BEGIN 0*/
void Mover_Eje_Z (int altura_mm);

//Funciones fantasma, cuando Maria y Alba termineis, conectareis vuestro codigo aqui dentro
void Mover_Robot_A_Coordenada (int x, int y){
	 //BLOQUE DE MARÍA: Cinemática Inversa

	    // El lienzo está a 45°: Sofía ya programó que Z = Y en la compensación.
	    // La Z que necesita la IK se obtiene directamente de la coordenada Y del lienzo.
	    float z_mm = (float)y;

	    //IK_Result_t ik = IK_Resolver_Movimiento((float)x, (float)y, z_mm);

	    //if (!ik.valid) {
	        // Punto fuera de rango: no mover, esperar siguiente frame
	        return;
	   // }

	    // --- BLOQUE DE ALBA: PID y motores ---
	    // Alba rellena estas funciones en pid_core.c y pwm_motors.c
	   // PID_SetTarget_Hombro(ik.q1_rad);   // → TIM1_CH1 (PE9) + DIR_J1 (PE7/PE8)
	   // PID_SetTarget_Codo(ik.q3_rad);     // → TIM1_CH2 (PE11) + DIR_J2 (PE10/PE12)
	   // Mover_Eje_Z(ik.q2_mm);            // → TIM1_CH3 (PE13) + DIR_Z (PE14/PE15)
	//AVISO A ALBA: al trazar las lineas, los motores X, Y, Z tienen que moverse a la vez
}

//Funcion para el eje Z (la torre principal)
void Mover_Eje_Z (int altura_mm){
	//aqui (segun gemini) alba pone el codigo del motor de la torre z
}

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
	int primer_x = centro_x + (radio_dibujo * cos(0));
	int primer_y = centro_y + (radio_dibujo * sin(0));

	//Vamos al borde derecho del círculo para empezar a dibujar
	Mover_Robot_A_Coordenada(centro_x + radio_dibujo, centro_y);
	Bajar_Rotulador(centro_y);

	//Bucle de interpolación
	for (int i = 1; i <= puntos_resolucion; i++){
		float angulo_actual = i * angulo_paso;

		//fórmulas trigonometricas
		int punto_x = centro_x + (radio_dibujo * cos(angulo_actual));
		int punto_y = centro_y + (radio_dibujo * sin(angulo_actual));

		//le mandamos la orden al cerebro del robot
		Mover_Robot_A_Coordenada(punto_x, punto_y);

		//pequeño paron para que la placa tenga tiempo de mover los motores
		HAL_Delay(20);
	}

	//Terminamos y levantamos
	Subir_Rotulador(centro_y);

}
/*USER CODE END 0*/
