#include "main.h"
#include "hri.h" // <-- OJO: pon aquí el nombre de tu archivo .h
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;
#define LCD_ADDR 0x4E

//funcion interfaz
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
// --- FUNCIONES DE PANTALLA ---
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

// --- FUNCIONES DE BOTONES Y SENSORES ---
static uint32_t btn1_start_time = 0;
static uint8_t  btn1_is_pressed = 0;

static uint32_t btn2_start_time = 0;
static uint8_t  btn2_is_pressed = 0;

static uint32_t btn3_start_time = 0;
static uint8_t  btn3_is_pressed = 0;

static uint32_t reset_start_time = 0;
static uint8_t  reset_is_pressed = 0;

#define DEBOUNCE_MS     50
#define TIEMPO_RESET_MS 2000

int Leer_Botones_Accion(void)
{
	uint32_t ahora = HAL_GetTick();

	// BOTÓN 1: Girar Gripper (PC2)
	GPIO_PinState estado_btn1 = HAL_GPIO_ReadPin(GPIOC, BTN_COLOR_Pin);
	if (estado_btn1 == GPIO_PIN_SET) {
		if (!btn1_is_pressed) {
			btn1_is_pressed = 1;
			btn1_start_time = ahora;
		}
	} else {
		if (btn1_is_pressed){
			uint32_t duracion = ahora - btn1_start_time;
			btn1_is_pressed = 0;
			if (duracion >= DEBOUNCE_MS) return 1;
		}
	}

	// BOTÓN 2: Dibujar Círculo (PC4)
    GPIO_PinState estado_btn2 = HAL_GPIO_ReadPin(GPIOC, BTN_CIRCULO_Pin);
    if (estado_btn2 == GPIO_PIN_SET) {
        if (!btn2_is_pressed) {
            btn2_is_pressed = 1;
            btn2_start_time = ahora;
        }
    } else {
        if (btn2_is_pressed){
            uint32_t duracion = ahora - btn2_start_time;
            btn2_is_pressed = 0;
            if (duracion >= DEBOUNCE_MS) return 2;
        }
    }

	// BOTÓN 3: Dibujar Línea (PC3)
    GPIO_PinState estado_btn3 = HAL_GPIO_ReadPin(GPIOC, BTN_LINEA_Pin);
    if (estado_btn3 == GPIO_PIN_SET) {
        if (!btn3_is_pressed) {
            btn3_is_pressed = 1;
            btn3_start_time = ahora;
        }
    } else {
        if (btn3_is_pressed){
            uint32_t duracion = ahora - btn3_start_time;
            btn3_is_pressed = 0;
            if (duracion >= DEBOUNCE_MS) return 3;
        }
    }

	return 0;
}

int Leer_Boton_Reset(void)
{
	uint32_t ahora = HAL_GetTick();
	GPIO_PinState estado_reset = HAL_GPIO_ReadPin(GPIOC, BTN_RESET_Pin);

	if (estado_reset == GPIO_PIN_SET){
		if (!reset_is_pressed){
			reset_is_pressed = 1;
			reset_start_time = ahora;
		}
	}else {
		if (reset_is_pressed){
			uint32_t duracion = ahora - reset_start_time;
			reset_is_pressed = 0;
			if (duracion >= TIEMPO_RESET_MS) return 1;
		}
	}
	return 0;
}


