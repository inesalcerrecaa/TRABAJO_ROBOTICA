/* hri.c — Lectura de botones */
#include "hri.h"        // CABECERA DE INTERFAZ
#include "gripper.h"    // MÓDULO DEL ACTUADOR
#include <stdio.h>  //para printf
#include <string.h>

/* BTN_COLOR → PC2 — GPIO_Input (polling con anti-rebote simple) */
#define DEBOUNCE_MS  50 // RETARDO ANTI-REBOTES


extern UART_HandleTypeDef huart2; //declaro variable externa para huart2
static uint32_t last_press_ms = 0;           // MARCA DE TIEMPO ANTERIOR
static uint8_t  last_state    = GPIO_PIN_SET; // ESTADO PREVIO PULL-UP


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

void HRI_Update(void) // REFRESCAR ENTRADAS
{
    uint8_t current_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2); // LEE PIN PC2

    /* Flanco de bajada + anti-rebote */
    if ((current_state == GPIO_PIN_RESET) && // DETECTA PULSACIÓN
        (last_state    == GPIO_PIN_SET))
    {
        last_press_ms = HAL_GetTick(); // RESETEA TEMPORIZADOR
        Gripper_MoveNext();            // MUEVE EL GRIPPER
    }

    last_state = current_state; // ACTUALIZA MEMORIA ESTADO
}
void HRI_Init(void) {
    // Puedes dejarla vacía por ahora o usarla para
    // inicializar variables de estado de los botones.
}
