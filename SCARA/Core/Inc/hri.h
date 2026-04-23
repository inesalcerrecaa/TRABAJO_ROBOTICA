/* hri.h */
#ifndef HRI_H // EVITA DOBLE INCLUSIÓN
#define HRI_H // DEFINE IDENTIFICADOR

#pragma once
#include "stm32f4xx_hal.h" // LIBRERÍA BASE HAL

void HRI_Update(void); // PROTOTIPO REFRESCO HRI
void HRI_Init(void);

//prototipo funcion imprimir por pantalla
void Interfaz_enviar(float realJ1, float objetivoJ1, float voltajeJ1,
    float realJ2, float objetivoJ2, float voltajeJ2,
    float realZ, float objetivoZ, float voltajeZ);

#endif /* HRI_H */ // FIN DE GUARDAS
