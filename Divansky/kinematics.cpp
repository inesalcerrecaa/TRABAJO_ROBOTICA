#include "kinematics.h" // INCLUYE CABECERA
#include <math.h> // MATES AVANZADAS

const float L1 = 200.0; // LARGO ESLABON 1
const float L2 = 200.0; // LARGO ESLABON 2
const float L_Z = 30.0; // OFFSET EJE Z
const float L_XY = 163.0; // OFFSET PLANO XY

IK_Result calcularMovimientoRobot(float X_objetivo, float Y_objetivo, float Z_objetivo, float angulo_servo_q4_grados) { // CALCULA CINEMATICA
  IK_Result resultado; // VARIABLE RETORNO
  
  float q4_rad = angulo_servo_q4_grados * (PI / 180.0); // CONVIERTE A RAD

  float L_efectivo = sqrt((L2 * L2) + (L_XY * L_XY) + (2.0 * L2 * L_XY * cos(q4_rad))); // BRAZO EQUIVALENTE
  float alpha_rad = atan2(L_XY * sin(q4_rad), L2 + L_XY * cos(q4_rad)); // ANGULO CORRECCION

  resultado.J3_mm = Z_objetivo + L_Z; // POSICION EJE Z

  float distancia_sq = (X_objetivo * X_objetivo) + (Y_objetivo * Y_objetivo); // DISTANCIA XY CUADRADO
  float cos_q3_prima = (distancia_sq - (L1 * L1) - (L_efectivo * L_efectivo)) / (2.0 * L1 * L_efectivo); // COSENO LEY COSENOS
  
  if (cos_q3_prima > 1.0 || cos_q3_prima < -1.0) { // FILTRO ALCANCE
      resultado.punto_valido = false; // FUERA DE RANGO
      if (cos_q3_prima > 1.0) cos_q3_prima = 1.0; // SATURA MAX
      if (cos_q3_prima < -1.0) cos_q3_prima = -1.0; // SATURA MIN
  } else {
      resultado.punto_valido = true; // PUNTO ALCANZABLE
  }

  float q3_prima_rad = acos(cos_q3_prima); // ANGULO CODO RAD
  float J2_rad = q3_prima_rad - alpha_rad; // CORRIGE ANGULO CODO

  float J1_rad = atan2(Y_objetivo, X_objetivo) - atan2(L_efectivo * sin(q3_prima_rad), L1 + L_efectivo * cos(q3_prima_rad)); // ANGULO HOMBRO RAD

  resultado.J1_grados = J1_rad * (180.0 / PI); // PASA J1 A GRADOS
  resultado.J2_grados = J2_rad * (180.0 / PI); // PASA J2 A GRADOS

  return resultado; // DEVUELVE CALCULOS
}

void Dibujar_Linea_Aleatoria(void) { // TRAYECTORIA RECTA
    Serial.println("DIBUJANDO LINEA... "); // DEBUG SERIE
}

void Dibujar_Circulo_Aleatorio(void) { // TRAYECTORIA CIRCULAR
    Serial.println("DIBUJANDO CIRCULO... "); // DEBUG SERIE
}