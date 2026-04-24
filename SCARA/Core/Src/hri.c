/* hri.c — Lectura de botones */
#include "hri.h"        // CABECERA DE INTERFAZ
#include "gripper.h"    // MÓDULO DEL ACTUADOR
#include "kinematics.h" // NECESARIO PARA LAS FUNCIONES DE DIBUJAR

/* BTN_COLOR → PC2 — GPIO_Input (polling con anti-rebote simple) */
#define DEBOUNCE_MS  50 // RETARDO ANTI-REBOTES

static uint32_t last_press_ms = 0;           // MARCA DE TIEMPO ANTERIOR
static uint8_t  last_state    = GPIO_PIN_SET; // ESTADO PREVIO PULL-UP

//ESTADOS PREVIOS DE LOS 4 BOTONES
static uint8_t last_state_color   = GPIO_PIN_SET;   // PC2 (Pull-up)
static uint8_t last_state_linea   = GPIO_PIN_RESET; // PC3
static uint8_t last_state_circulo = GPIO_PIN_RESET; // PC4
static uint8_t last_state_reset   = GPIO_PIN_RESET; // PC5

void HRI_Update(void) // REFRESCAR ENTRADAS
{
	//MOVER GRIPPER (FUNCIONA YA)
    uint8_t current_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2); // LEE PIN PC2

    /* Flanco de bajada + anti-rebote */
    if ((current_state == GPIO_PIN_RESET) && // DETECTA PULSACIÓN
        (last_state    == GPIO_PIN_SET))
    {
        last_press_ms = HAL_GetTick(); // RESETEA TEMPORIZADOR
        Gripper_MoveNext();            // MUEVE EL GRIPPER
    }

    last_state = current_state; // ACTUALIZA MEMORIA ESTADO

    //BOTON LINEA
    uint8_t current_linea = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3); // LEE PIN PC3

        /* Flanco de bajada + anti-rebote */
        if ((current_linea == GPIO_PIN_SET) && // DETECTA PULSACIÓN
            (last_state_linea == GPIO_PIN_RESET))
        {
            last_press_ms = HAL_GetTick(); // RESETEA TEMPORIZADOR
            Dibujar_Linea_Aleatoria();            // MUEVE EL GRIPPER
        }

        last_state_linea = current_linea; // ACTUALIZA MEMORIA ESTADO

    // BOTON CIRCULO
        uint8_t current_circulo = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4); // LEE PIN PC4

            /* Flanco de bajada + anti-rebote */
            if ((current_circulo == GPIO_PIN_SET) && // DETECTA PULSACIÓN
                (last_state_circulo == GPIO_PIN_RESET))
            {
                last_press_ms = HAL_GetTick(); // RESETEA TEMPORIZADOR
                Dibujar_Circulo_Aleatorio();            // MUEVE EL GRIPPER
            }

            last_state_circulo = current_circulo; // ACTUALIZA MEMORIA ESTADO

    //BOTON RESET
            uint8_t current_reset = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5); // LEE PIN PC5

                /* Flanco de bajada + anti-rebote */
                if ((current_reset == GPIO_PIN_SET) && // DETECTA PULSACIÓN
                    (last_state_reset == GPIO_PIN_RESET))
                {
                    last_press_ms = HAL_GetTick(); // RESETEA TEMPORIZADOR
                    Gripper_Init();            // MUEVE EL GRIPPER
                }

                last_state_reset = current_reset; // ACTUALIZA MEMORIA ESTADO
}
void HRI_Init(void) {
	// Inicializa variables leyendo el estado real al arrancar
	    last_state_color   = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2);
	    last_state_linea   = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);
	    last_state_circulo = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4);
	    last_state_reset   = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5);
}
