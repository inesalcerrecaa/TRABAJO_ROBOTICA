#include "hri.h" // CABECERA PROPIA
#include "gripper.h" // CONTROL MOTOR
#include "kinematics.h" // TRAYECTORIAS ROBOT
#include <Wire.h> // COMUNICACION I2C
#include <string.h> // MANEJO TEXTO

#define DEBOUNCE_MS 50 // FILTRO RUIDO
#define LCD_ADDR 0x27 // DIRECCION PANTALLA

static int color_idx = 0; // INDICE CICLO COLOR
const char* nombres_colores[] = {"ROJO ", "AZUL ", "VERDE"}; // LISTA COLORES

static uint8_t last_state_color   = HIGH; // ESTADO BOTON 1
static uint8_t last_state_linea   = HIGH; // ESTADO BOTON 2
static uint8_t last_state_circulo = HIGH; // ESTADO BOTON 3
static uint8_t last_state_reset   = HIGH; // ESTADO BOTON 4

static uint32_t last_press_ms = 0; // MARCA TIEMPO CLICK

void HRI_Init(void) {
    // CONFIGURACIÓN DE PINES
    pinMode(PIN_BTN_COLOR,   INPUT_PULLUP); // ENTRADA PULLUP
    pinMode(PIN_BTN_LINEA,   INPUT_PULLUP); // ENTRADA PULLUP
    pinMode(PIN_BTN_CIRCULO, INPUT_PULLUP); // ENTRADA PULLUP
    pinMode(PIN_BTN_RESET,   INPUT_PULLUP); // ENTRADA PULLUP

    // INICIALIZACIÓN DEL HARDWARE (EL ORDEN IMPORTA, REINA)
    delay(100);    // 2. ESPERA ESTABILIZACIÓN 
    LCD_Init();    // 3. SECUENCIA DE ARRANQUE LCD
}

void HRI_Update(void) {
    uint32_t now = millis(); // TOMA TIEMPO ACTUAL

    uint8_t cur_color = digitalRead(PIN_BTN_COLOR); // LEE BOTON COLOR
    if ((cur_color == LOW) && (last_state_color == HIGH) && (now - last_press_ms >= DEBOUNCE_MS)) {
        last_press_ms = now; // GUARDA MOMENTO
        Display_LCD_Escribir(1, 7, (char*)nombres_colores[color_idx]); // ESCRIBE COLOR ACTUAL
        color_idx = (color_idx + 1) % 3; // CAMBIA AL SIGUIENTE
        Gripper_MoveNext(); // GIRA REVOLVER
    }
    last_state_color = cur_color; // ACTUALIZA ESTADO

    uint8_t cur_linea = digitalRead(PIN_BTN_LINEA); // LEE BOTON LINEA
    if ((cur_linea == LOW) && (last_state_linea == HIGH) && (now - last_press_ms >= DEBOUNCE_MS)) {
        last_press_ms = now; // GUARDA MOMENTO
        Display_LCD_Escribir(1, 0, (char*)"DIBUJANDO LINEA "); // ACTUALIZA LCD
        Dibujar_Linea_Aleatoria(); // LANZA MOVIMIENTO
    }
    last_state_linea = cur_linea; // ACTUALIZA ESTADO

    uint8_t cur_circulo = digitalRead(PIN_BTN_CIRCULO); // LEE BOTON CIRCULO
    if ((cur_circulo == LOW) && (last_state_circulo == HIGH) && (now - last_press_ms >= DEBOUNCE_MS)) {
        last_press_ms = now; // GUARDA MOMENTO
        Display_LCD_Escribir(1, 0, (char*)"DIBUJANDO CIRCUL"); // ACTUALIZA LCD
        Dibujar_Circulo_Aleatorio(); // LANZA MOVIMIENTO
    }
    last_state_circulo = cur_circulo; // ACTUALIZA ESTADO

    uint8_t cur_reset = digitalRead(PIN_BTN_RESET); // LEE BOTON RESET
    if ((cur_reset == LOW) && (last_state_reset == HIGH) && (now - last_press_ms >= DEBOUNCE_MS)) {
        last_press_ms = now; // GUARDA MOMENTO
        Display_LCD_Escribir(1, 0, (char*)"MOVIENDO REPOSO "); // ACTUALIZA LCD
        Gripper_Init(); // PARA TODO
    }
    last_state_reset = cur_reset; // ACTUALIZA ESTADO
}

// DRIVER LCD (ESTO ES LO QUE TE FALTABA)
void LCD_Send_Cmd(char cmd) {
    char cmd_u = (cmd & 0xF0); // NIBBLE SUPERIOR
    char cmd_l = ((cmd << 4) & 0xF0); // NIBBLE INFERIOR
    uint8_t cmd_t[4] = {(uint8_t)(cmd_u|0x0C), (uint8_t)(cmd_u|0x08), (uint8_t)(cmd_l|0x0C), (uint8_t)(cmd_l|0x08)}; // COMANDOS I2C
    Wire.beginTransmission(LCD_ADDR); // INICIA TRAMA
    Wire.write(cmd_t, 4); // ENVIA BYTES
    Wire.endTransmission(); // FINALIZA TRAMA
}

void LCD_Send_Data(char data) {
    char data_u = (data & 0xF0); // NIBBLE SUPERIOR
    char data_l = ((data << 4) & 0xF0); // NIBBLE INFERIOR
    uint8_t data_t[4] = {(uint8_t)(data_u|0x0D), (uint8_t)(data_u|0x09), (uint8_t)(data_l|0x0D), (uint8_t)(data_l|0x09)}; // DATOS I2C
    Wire.beginTransmission(LCD_ADDR); // INICIA TRAMA
    Wire.write(data_t, 4); // ENVIA BYTES
    Wire.endTransmission(); // FINALIZA TRAMA
}

void LCD_Init(void) {
    delay(50); // ESPERA ARRANQUE
    LCD_Send_Cmd(0x30); delay(5); // WAKE UP 1
    LCD_Send_Cmd(0x30); delay(1); // WAKE UP 2
    LCD_Send_Cmd(0x30); delay(10); // WAKE UP 3
    LCD_Send_Cmd(0x20); delay(10); // MODO 4 BITS
    LCD_Send_Cmd(0x28); // 2 LINEAS 5X8
    LCD_Send_Cmd(0x08); // DISPLAY OFF
    LCD_Send_Cmd(0x01); delay(2); // LIMPIA PANTALLA
    LCD_Send_Cmd(0x06); // ENTRY MODE
    LCD_Send_Cmd(0x0C); // DISPLAY ON
}

void Display_LCD_Escribir(uint8_t fila, uint8_t col, char *texto) {
    uint8_t pos = (fila == 0) ? (0x80 + col) : (0xC0 + col); // CALCULA POSICION
    LCD_Send_Cmd(pos); // MUEVE CURSOR
    while (*texto) LCD_Send_Data(*texto++); // ESCRIBE CADENA
}