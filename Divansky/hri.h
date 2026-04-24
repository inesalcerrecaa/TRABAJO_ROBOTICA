// hri.h
#pragma once
#include <Arduino.h>
#include "config.h"

void HRI_Init(void);
void HRI_Update(void);
int  Leer_Boton_Reset(void);

void Interfaz_enviar(float realJ1, float objetivoJ1, float voltajeJ1,  // TELEMETRÍA J1
                     float realJ2, float objetivoJ2, float voltajeJ2,  // TELEMETRÍA J2
                     float realZ,  float objetivoZ,  float voltajeZ);  // TELEMETRÍA Z

void LCD_Init(void);                                          // ARRANQUE LCD
void LCD_Send_Cmd(char cmd);                                  // ENVÍA COMANDO LCD
void LCD_Send_Data(char data);                                // ENVÍA DATO LCD
void Display_LCD_Escribir(uint8_t fila, uint8_t col, char *texto); // ESCRIBE EN POSICIÓN