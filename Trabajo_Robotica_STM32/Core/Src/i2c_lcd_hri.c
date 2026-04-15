/*
 * i2c_lcd_hri.c
 *
 *  Created on: Mar 30, 2026
 *      Author: Sofia
 */

#include "main.h"
#include "i2c_lcd_hri.h"

/* USER CODE BEGIN 0 */
//referencia al puerto I2C configurado en el main.c
extern I2C_HandleTypeDef hi2c1;
//direccion I2C de la pantalla (suele ser 0x4E, si no enciende probamos con 0x27<<1)
#define LCD_ADDR 0x4E

//FUNCIONES RECICLADAS

void LCD_Send_Cmd(char cmd) {
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (cmd & 0xf0);
  data_l = ((cmd << 4) & 0xf0);
  data_t[0] = data_u | 0x0C;  // en=1, rs=0
  data_t[1] = data_u | 0x08;  // en=0, rs=0
  data_t[2] = data_l | 0x0C;  // en=1, rs=0
  data_t[3] = data_l | 0x08;  // en=0, rs=0
  HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void LCD_Send_Data(char data) {
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (data & 0xf0);
  data_l = ((data << 4) & 0xf0);
  data_t[0] = data_u | 0x0D;  // en=1, rs=1
  data_t[1] = data_u | 0x09;  // en=0, rs=1
  data_t[2] = data_l | 0x0D;  // en=1, rs=1
  data_t[3] = data_l | 0x09;  // en=0, rs=1
  HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void Display_LCD_Escribir(uint8_t fila, uint8_t col, char *texto) {
    // Fila 0 = 0x80, Fila 1 = 0xC0
    uint8_t pos = (fila == 0) ? 0x80 : 0xC0;
    LCD_Send_Cmd(pos | col);
    while (*texto) LCD_Send_Data(*texto++);
}

//LECTURA DE BOTONES TÁCTILES (ANTIRREBOTE)

static uint32_t btn1_start_time = 0;
static uint8_t  btn1_is_pressed = 0;

static uint32_t btn2_start_time = 0;
static uint8_t  btn2_is_pressed = 0;

static uint32_t btn3_start_time = 0;
static uint8_t  btn3_is_pressed = 0;

static uint32_t reset_start_time = 0;
static uint8_t  reset_is_pressed = 0;

#define DEBOUNCE_MS     50
#define TIEMPO_RESET_MS 2000 //dos segundos de pulsacion larga para el reset

//funcion para leer que accion se ha pedido
//Devuelve: 1 (Girar Gripper), 2 (Círculo), 3 (Línea), 0 (Nada)
int Leer_Botones_Accion(void)
{
	uint32_t ahora = HAL_GetTick();
	//BOTÓN 1: Girar Gripper (PC2)
	GPIO_PinState estado_btn1 = HAL_GPIO_ReadPin(GPIOC, BTN_C1_Pin);
	if (estado_btn1 == GPIO_PIN_SET) //asumimos que los tactiles dan 1 al tocar
	{
		if (!btn1_is_pressed)
		{
			btn1_is_pressed = 1;
			btn1_start_time = ahora;
		}
	} else {
		if (btn1_is_pressed){
			uint32_t duracion = ahora - btn1_start_time;
			btn1_is_pressed = 0;
			if (duracion >= DEBOUNCE_MS) return 1; //Accion 1
		}
	}

	//BOTÓN 2: Dibujar Círculo (PC3)
		GPIO_PinState estado_btn2 = HAL_GPIO_ReadPin(GPIOC, BTN_C2_Pin);
		if (estado_btn2 == GPIO_PIN_SET) //asumimos que los tactiles dan 1 al tocar
		{
			if (!btn2_is_pressed)
			{
				btn2_is_pressed = 1;
				btn2_start_time = ahora;
			}
		} else {
			if (btn2_is_pressed){
				uint32_t duracion = ahora - btn2_start_time;
				btn2_is_pressed = 0;
				if (duracion >= DEBOUNCE_MS) return 2; //Accion 2
			}
		}

	//BOTÓN 3: Dibujar Línena (PC4)
		GPIO_PinState estado_btn3 = HAL_GPIO_ReadPin(GPIOC, BTN_C3_Pin);
		if (estado_btn3 == GPIO_PIN_SET) //asumimos que los tactiles dan 1 al tocar
			{
				if (!btn3_is_pressed)
				{
					btn3_is_pressed = 1;
					btn3_start_time = ahora;
				}
			} else {
				if (btn3_is_pressed){
					uint32_t duracion = ahora - btn3_start_time;
					btn3_is_pressed = 0;
					if (duracion >= DEBOUNCE_MS) return 3; //Accion 3
				}
			}

	return 0; //si no hay clics validos devuelve 0
}

//Funcion para el boton de RESET (PC5) - Requiere pulsacion larga
//Devuelve  si se ha mantenido pulsado 2 segundos, 0 si no.
int Leer_Boton_Start (void)
{
	uint32_t ahora = HAL_GetTick();
	GPIO_PinState estado_reset = HAL_GPIO_ReadPin(GPIOC, BTN_START_Pin);

	if (estado_reset == GPIO_PIN_SET){
		if (!reset_is_pressed){
			reset_is_pressed = 1;
			reset_start_time = ahora;
		}
	}else {
		if (reset_is_pressed){
			uint32_t duracion = ahora - reset_start_time;
			reset_is_pressed = 0;
			if (duracion >= TIEMPO_RESET_MS) return 1; //indica un clic valido de reset
		}
	}

	return 0;
}

//LECTURA DEL SENSOR HALL (Tambor Revolver)
//devuelve 1 si detecta el iman, 0 si no lo detecta
int Leer_Sensor_Hall (void){
	//leemos el pin PC1 usando el nombre (Label), configurado por Gabri
	if (HAL_GPIO_ReadPin(GPIOC, HALL_SENSOR_Pin) == GPIO_PIN_SET){
		return 1;
	}

	return 0;
}
/* USER CODE END 0 */
