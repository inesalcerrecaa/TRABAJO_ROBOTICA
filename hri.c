#include "interfaz.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

void Interfaz_enviar(float realJ1, float objetivoJ1, float voltajeJ1,
    float realJ2, float objetivoJ2, float voltajeJ2,
    float realZ, float objetivoZ, float voltajeZ) {

    char buffer[200];

    snprintf(buffer, sizeof(buffer),
        "J1: pos=%.1f obj=%.1f pid=%.1f | J2: pos=%.1f obj=%.1f pid=%.1f | Z: pos=%.1f obj=%.1f pid=%.1f\r\n",
        realJ1, objetivoJ1, voltajeJ1,
        realJ2, objetivoJ2, voltajeJ2,
        realZ, objetivoZ, voltajeZ);

    HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buffer, strlen(buffer));
}

//CORREGIDO