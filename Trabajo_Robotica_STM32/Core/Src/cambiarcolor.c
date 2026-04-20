/*
 * cambiarcolor.c
 *
 *  Created on: Apr 20, 2026
 *      Author: Gabriela
 */

#include "cambiarcolor.h" // INCLUYE CABECERA PERSONALIZADA

// Variables privadas para que no se pinchen desde fuera
static TIM_HandleTypeDef *gripper_htim; // PUNTERO AL TIMER DEL SERVO
static volatile uint8_t buscando = 0;   // ESTADO DE BÚSQUEDA ACTIVA

/**
 * @brief Inicializa el timer del servo
 */
void GRIPPER_Init(TIM_HandleTypeDef *htim) { // CONFIGURA EL HARDWARE INICIAL
    gripper_htim = htim; // ASIGNA EL TIMER AL MÓDULO
    HAL_TIM_PWM_Start(gripper_htim, TIM_CHANNEL_1); // ARRANCA SEÑAL PWM CANAL 1
    __HAL_TIM_SET_COMPARE(gripper_htim, TIM_CHANNEL_1, SERVO_STOP); // PARA EL SERVO AL INICIO
}

/**
 * @brief Lógica para buscar el imán con el sensor Hall
 */
void GRIPPER_SeleccionarColor(uint8_t color_destino) { // INICIA GIRO HASTA DETECTAR COLOR
    buscando = 1; // ACTIVA FLAG DE BÚSQUEDA

    // Arrancamos el motor
    __HAL_TIM_SET_COMPARE(gripper_htim, TIM_CHANNEL_1, SERVO_VEL); // ACTIVA GIRO DEL SERVO

    // Esperamos a que el Callback del Hall nos pare
    while(buscando) { // BLOQUEA HASTA HALLAR IMÁN
        // Aquí podrías añadir un delay o un contador de seguridad
    }
}

/**
 * @brief Esto lo llamarás desde el main.c cuando salte la interrupción
 */
void GRIPPER_HallCallback(uint16_t GPIO_Pin) { // GESTIONA INTERRUPCIÓN DEL SENSOR
    if (GPIO_Pin == HALL_SENSOR_Pin) { // FILTRA PIN DEL SENSOR HALL
        if (buscando) { // VERIFICA SI ESTÁBAMOS BUSCANDO
            __HAL_TIM_SET_COMPARE(gripper_htim, TIM_CHANNEL_1, SERVO_STOP); // DETIENE EL MOTOR
            buscando = 0; // FINALIZA PROCESO DE BÚSQUEDA
        }
    }
}
