
/* USER CODE BEGIN Includes */
#include "main.h"              // ¡Obligatorio para la STM32!
#include "kinmatics.h" // El trabajo de María
#include"motion.h"
#include"hri.h"  //para Leer_boton_start



/* USER CODE END Includes */

/* USER CODE BEGIN PV */

//VARIABLES GLOBALES
PID pidJ1(2.0f, 0.1f, 0.1f,200.0f);  //estos valores (kp,ki,kd,umbral) habrá que determinarlos
PID pidZ(4.0f, 0.5f, 0.2f,150.0f);
PID pidJ2(1.2f, 0.1f, 0.04f,120.0f);

//variables de tiempo y de sensores
uint32_t tiempoAnterior = 0;
float posicionRealZ = 0.0;
float posicionRealBase = 0.0;

/* USER CODE END PV */
int main(void)
{
    /* USER CODE BEGIN 2 */
    // Arrancar los motores
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // Enciende la base J1
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // Enciende la traslacion del eje Z
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //Enciente el codo J2

    Homing();

    tiempoAnterior = HAL_GetTick(); // Empezamos a contar el tiempo

    

    /* USER CODE END 2 */

    /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        //calculo de dt, tiempo que ha pasado desde la ultima iteracion del bucle con respecto a la siguiente
        uint32_t ahora = HAL_GetTick();
        if ((ahora - tiempoAnterior) < 10) continue;  // espera sin bloquear
        float dt = (ahora - tiempoAnterior) / 1000.0f;
        tiempoAnterior = ahora;


        //COMPROBAR NOMBRE FUNCION SOFIA
        //si, con la funcion de sofia, se ha pulsado el boton de reset, HOMING
        if (Leer_Boton_Start() == 2) {
            Homing();
        }    int boton=Leer_Boton_Start


        // Coordenadas que nos da maria de la cinemática COMPROBAR FUNCION y que ella ha creado una restuctura llamada ResultadosCinematica con los resultados del J1, J2, y Z
         ResultadosCinematica objetivos = obtenerAngulos();

        //Lectura de los sensores de los motores para las positicones actuales
       

        //COMPROBAR NOMBRE FUNCION SOFIA
        float realJ1 = Leer_Posicion_Encoder(1);
        float realJ2 = Leer_Posicion_Encoder(2); 
        float realZ = Leer_Posicion_Encoder(3);
       
        
        //Actualizacion de los 3 motores
       
        actualizarEje(pidJ1, realJ1, objetivos.j1, dt, &htim1, TIM_CHANNEL_1, GPIOE, GPIO_PIN_7, GPIO_PIN_8);
        actualizarEje(pidJ2, realJ2, objetivos.j2, dt, &htim1, TIM_CHANNEL_2, GPIOE, GPIO_PIN_10, GPIO_PIN_12);
        actualizarEje(pidZ, realZ, objetivos.z, dt, &htim1, TIM_CHANNEL_3, GPIOE, GPIO_PIN_14, GPIO_PIN_15);
        
        HAL_Delay(10); // Divansky respira 10ms 

        static uint32_t tiempoDebug = 0;  //variable local que dura durante todas las iteraciones gracias a statil

        //bucle if para comprobar si han pasado 100ms desde el ultimo envio de datos, y si ya han pasado enviar datos a la funcion
        if ((ahora - tiempoDebug) >= 100) {
            tiempoDebug = ahora; //resetear cronometro para volver a contar hasta 100ms y enviar datos
            //mandarr datos a la funcion para imprimir
            Interfaz_enviar(
                realJ1, objetivos.j1, pidJ1.getVoltaje(),
                realJ2, objetivos.j2, pidJ2.getVoltaje(),
                realZ, objetivos.z, pidZ.getVoltaje()
            );
        }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}


