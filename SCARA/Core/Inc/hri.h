/* hri.h */
#ifndef INC_HRI_H // EVITA DOBLE INCLUSIÓN
#define INC_HRI_H // DEFINE IDENTIFICADOR

#pragma once
#include "stm32f4xx_hal.h" // LIBRERÍA BASE HAL
#include"main.h"


//funciones de reseteo
void HRI_Update(void);
void HRI_Init(void);


//funciones de pantalla
void LCD_Init(void);
void Display_LCD_Escribir(uint8_t fila, uint8_t col, char *texto);

//funciones de ESTADO DE BOTONES
int Leer_Botones_Accion(void);
int Leer_Boton_Reset(void);

//prototipo funcion imprimir por pantalla
void Interfaz_enviar(float realJ1, float objetivoJ1, float voltajeJ1,
    float realJ2, float objetivoJ2, float voltajeJ2,
    float realZ, float objetivoZ, float voltajeZ);


//funciones de botones
void HRI_Update(void);
void HRI_Init(void);


#endif

