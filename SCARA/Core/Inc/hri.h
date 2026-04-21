/*
 * hri.h
 *
 *  Created on: 21 abr 2026
 *      Author: Tester
 */

#ifndef INC_HRI_H_
#define INC_HRI_H_

#include "main.h"

//funciones de pantalla
void LCD_Init(void);
void Display_LCD_Escribir(uint8_t fila, uint8_t col, char *texto);

//funciones de botones y sensores
int Leer_Botones_Accion(void);
int Leer_Boton_Reset(void);
int Leer_Sensor_Hall(void);

#endif /* INC_HRI_H_ */
