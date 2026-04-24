/* hri.c — Lectura de botones */
#include"main.h"  //necesario para la stm32
#include "hri.h"        // CABECERA DE INTERFAZ
#include "gripper.h"    // MÓDULO DEL ACTUADOR
#include "kinematics.h"  //NECESARIO PARA LAS FUNCIONES DE DIBUJAR
#include <stdio.h>  //para printf
#include <string.h>

/* BTN_COLOR → PC2 — GPIO_Input (polling con anti-rebote simple) */
#define DEBOUNCE_MS  50 // RETARDO ANTI-REBOTES
#define LCD_ADDR 0x4E  //pantalla

extern I2C_HandleTypeDef hi2c1;  //declarar variable externa para lcd
extern UART_HandleTypeDef huart2; //declaro variable externa para huart2
static uint32_t last_press_ms = 0;           // MARCA DE TIEMPO ANTERIOR


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
        (last_state_color    == GPIO_PIN_SET))
    {
        last_press_ms = HAL_GetTick(); // RESETEA TEMPORIZADOR
        Gripper_MoveNext();            // MUEVE EL GRIPPER
    }

    last_state_color = current_state; // ACTUALIZA MEMORIA ESTADO

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

//FUNCION que devuelve 1 si esta pulsado el boton de reset
int Leer_Boton_Reset(void) {
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_SET) {
        return 1;
    }
    return 0;
}

//FUNCION SALIDA POR PANTALLA
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

//FUNCIONES DE PANTALLA
void LCD_Send_Cmd(char cmd) {
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (cmd & 0xf0);
  data_l = ((cmd << 4) & 0xf0);
  data_t[0] = data_u | 0x0C;
  data_t[1] = data_u | 0x08;
  data_t[2] = data_l | 0x0C;
  data_t[3] = data_l | 0x08;
  HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void LCD_Send_Data(char data) {
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (data & 0xf0);
  data_l = ((data << 4) & 0xf0);
  data_t[0] = data_u | 0x0D;
  data_t[1] = data_u | 0x09;
  data_t[2] = data_l | 0x0D;
  data_t[3] = data_l | 0x09;
  HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void LCD_Init(void) {
    HAL_Delay(50);
    LCD_Send_Cmd(0x30);
    HAL_Delay(5);
    LCD_Send_Cmd(0x30);
    HAL_Delay(1);
    LCD_Send_Cmd(0x30);
    HAL_Delay(10);
    LCD_Send_Cmd(0x20);
    HAL_Delay(10);
    LCD_Send_Cmd(0x28);
    HAL_Delay(1);
    LCD_Send_Cmd(0x08);
    HAL_Delay(1);
    LCD_Send_Cmd(0x01);
    HAL_Delay(2);
    LCD_Send_Cmd(0x06);
    HAL_Delay(1);
    LCD_Send_Cmd(0x0C);
}

void Display_LCD_Escribir(uint8_t fila, uint8_t col, char *texto) {
    uint8_t pos = (fila == 0) ? 0x80 : 0xC0;
    LCD_Send_Cmd(pos | col);
    while (*texto) LCD_Send_Data(*texto++);
}



