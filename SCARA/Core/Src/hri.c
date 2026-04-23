/* hri.c — Lectura de botones */
#include "hri.h"        // CABECERA DE INTERFAZ
#include "gripper.h"    // MÓDULO DEL ACTUADOR

/* BTN_COLOR → PC2 — GPIO_Input (polling con anti-rebote simple) */
#define DEBOUNCE_MS  50 // RETARDO ANTI-REBOTES

static uint32_t last_press_ms = 0;           // MARCA DE TIEMPO ANTERIOR
static uint8_t  last_state    = GPIO_PIN_SET; // ESTADO PREVIO PULL-UP

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
