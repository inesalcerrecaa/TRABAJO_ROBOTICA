#ifndef INC_HRI_H_
#define INC_HRI_H_

#include "main.h"

void LCD_Init(void);
void Display_LCD_Escribir(uint8_t fila, uint8_t col, char *texto);
int  Leer_Botones_Accion(void);
int  Leer_Boton_Reset(void);

#endif
