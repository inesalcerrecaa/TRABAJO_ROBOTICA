#ifndef KINEMATICS_H // EVITA DOBLE INCLUSION
#define KINEMATICS_H // DEFINE CABECERA

#include <Arduino.h> // LIBRERIA BASE

struct IK_Result { // ESTRUCTURA RESULTADOS
    float J1_grados; // ANGULO HOMBRO
    float J2_grados; // ANGULO CODO
    float J3_mm; // ALTURA Z
    bool  punto_valido; // FLAG ALCANCE
};

IK_Result calcularMovimientoRobot(float X_objetivo, float Y_objetivo, float Z_objetivo, float angulo_servo_q4_grados); // PROTOTIPO IK

void Dibujar_Linea_Aleatoria(); // PROTOTIPO LINEA
void Dibujar_Circulo_Aleatorio(); // PROTOTIPO CIRCULO

#endif // FIN CABECERA