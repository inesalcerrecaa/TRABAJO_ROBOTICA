//PARTE ESCRITA POR SOFÍA
//El encoder es el sentido del tacto o la propiocepcion del robot

#ifndef INC_MOTION_H_
#define INC_MOTION_H_

#include "main.h"

//funciones de los encoders
void Encoders_Init(void);
int32_t Leer_Pasos_Encoder(int numero_motor);

#endif /*INC_MOTION_H_*/
