#include "motion.h"
#include <math.h>   // Para usar fabsf() para valor abs


//Declaramos variables externas
extern TIM_HandleTypeDef htim1;
//extern GPIO_TypeDef* GPIOE;

//Declaramos variables internas volatiles, las flags
volatile uint8_t homingJ1_done = 0;
volatile uint8_t homingJ2_done = 0;
volatile uint8_t homingZ_done = 0;


//funcion con todos los datos PERSONALIZADOS para cada PID, inicializamos el resto de variables a cero
void PID_Init(PID_Controller* pid, float p, float i, float d, float umbral) {
    pid->kp = p;
    pid->ki = i;
    pid->kd = d;
    pid->umbralFriccion = umbral;

    pid->err = 0;
    pid->err_ini = 0;
    pid->integral = 0;
    pid->setpoint = 0;
    pid->actual = 0;
    pid->voltage = 0;
}



//funcion para calcular el PID
float PID_Calcular(PID_Controller* pid, float valorActual, float objetivo, float dt) {
    pid->actual = valorActual;
    pid->setpoint = objetivo;

    pid->err = pid->setpoint - pid->actual;


    // Suma de errores para el t�rmino integral
    pid->integral += pid->err * dt;

    // L�mites integraci�n (Anti-windup)
    if (pid->integral > 100) pid->integral = 100;
    else if (pid->integral < -100) pid->integral = -100;

    // T�rmino derivativo
    float derivative = (pid->err - pid->err_ini) / dt;

    // F�rmula PID
    pid->voltage = (pid->kp * pid->err) + (pid->ki * pid->integral) + (pid->kd * derivative);

    // L�mites voltaje para el PWM
    if (pid->voltage > 999) pid->voltage = 999;
    else if (pid->voltage < -999) pid->voltage = -999;

    pid->err_ini = pid->err;

    // Compensaci�n de fricci�n
    if (pid->voltage > 0 && pid->voltage < pid->umbralFriccion) {
        pid->voltage = pid->umbralFriccion;
    }
    else if (pid->voltage < 0 && pid->voltage > -pid->umbralFriccion) {
        pid->voltage = -pid->umbralFriccion;
    }

    return pid->voltage;
}

float PID_GetVoltaje(PID_Controller* pid) {
    return pid->voltage;
}


//funcion para actualizar los ejes de los motores conociendo los objetivos que nos da maria

void Update_Motor_Axis(PID_Controller* pid, float posicionActual, float objetivoMaria, float dt,
    TIM_HandleTypeDef* htim, uint32_t canal,
    GPIO_TypeDef* port, uint16_t pin1, uint16_t pin2)
{
    float voltaje = PID_Calcular(pid, posicionActual, objetivoMaria, dt);
    uint32_t pwm = (uint32_t)fabsf(voltaje); // Valor absoluto del voltaje para el timer, indica la velocidad

    // Puente H: Direcci�n + Velocidad
    if (voltaje > 0) {
        HAL_GPIO_WritePin(port, pin1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(port, pin2, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(htim, canal, pwm); //velocidad siempre constante en ambos casos, lo que cambia es la direccion con el orden de los pines de set y reset
    }
    else {
        HAL_GPIO_WritePin(port, pin1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port, pin2, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(htim, canal, pwm);
    }
}


//homing

void Homing(void) {
    homingJ1_done = 0;
    homingJ2_done = 0;
    homingZ_done = 0;

    // Arrancamos PWM
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, HOMING_PWM_J1);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, HOMING_PWM_J2);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, HOMING_PWM_Z);

    // Configuramos direcci�n de giro hacia los sensores (Pines del puerto E)
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);

    uint32_t tiempoInicio = HAL_GetTick();

    // Esperamos a que las interrupciones cambien los flags a 1
    while (!homingJ1_done || !homingJ2_done || !homingZ_done) {
        // Timeout de seguridad de 10 segundos
        if ((HAL_GetTick() - tiempoInicio) > HOMING_TIMEOUT_MS) {
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
            break;
        }
        HAL_Delay(10);
    }
}
