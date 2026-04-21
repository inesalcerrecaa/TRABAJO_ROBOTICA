#include "main.h" // INCLUYE DEFINICIONES DE HAL
#include "gripper.h" // INCLUYE PROTOTIPOS PROPIOS

extern TIM_HandleTypeDef htim5; // ACCEDE AL TIMER DEL SERVO DESDE EL MAIN

#define GRIPPER_STOP  1500 // VALOR DE PARADA (PUNTO MUERTO)
#define GRIPPER_SPEED 1650 // VELOCIDAD DE GIRO PARA BÚSQUEDA

static char* lista_colores[3] = {"ROJO   ", "AZUL   ", "VERDE  "}; // ETIQUETAS DE COLORES
static int color_actual = 0; // ÍNDICE DEL COLOR SELECCIONADO

// --- SENSOR HALL ---
static int Leer_Sensor_Hall(void)
{
    if (HAL_GPIO_ReadPin(GPIOC, SENSOR_Hall_Pin) == GPIO_PIN_SET) // LEE ESTADO FÍSICO DEL PIN
        return 1; // RETORNA 1 SI NO HAY IMÁN
    return 0; // RETORNA 0 SI DETECTA IMÁN
}

// --- SERVO ---
void Gripper_Init(void)
{
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1); // INICIA SEÑAL PWM
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, GRIPPER_STOP); // ASEGURA QUE ESTÉ PARADO
}

char* Gripper_GetColorActual(void)
{
    return lista_colores[color_actual]; // DEVUELVE EL TEXTO DEL COLOR
}

char* Gripper_SiguienteColor(void)
{
    color_actual++; // PASA AL SIGUIENTE ÍNDICE
    if (color_actual > 2) color_actual = 0; // REINICIA SI SUPERA EL LÍMITE

    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, GRIPPER_SPEED); // ACTIVA ROTACIÓN DEL SERVO

    HAL_Delay(500); // MARGEN PARA ABANDONAR EL IMÁN ACTUAL

    while (Leer_Sensor_Hall() == 1) // BUCLE MIENTRAS NO DETECTE EL SIGUIENTE IMÁN
    {
        // ESPERANDO...
    }

    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, GRIPPER_STOP); // DETIENE EL MOTOR AL LLEGAR
    return lista_colores[color_actual]; // RETORNA EL NUEVO COLOR
}

void Gripper_Reset(void)
{
    color_actual = 0; // VUELVE AL PRIMER COLOR
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, GRIPPER_STOP); // PARA EL SERVO
}
