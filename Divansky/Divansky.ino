#include <Wire.h>    // LIBRERIA BUS I2C
#include "config.h"  // DEFINICION PINES
#include "hri.h"     // FUNCIONES INTERFAZ
#include "gripper.h" // FUNCIONES MOTOR

void setup() {
    Serial.begin(115200); // INICIA CONSOLA DEBUG
    Wire.begin();         // INICIA COMUNICACION I2C
    
    Gripper_Init();       // CONFIGURA TIMER MOTOR
    HRI_Init();           // CONFIGURA BOTONES Y LCD
}

void loop() {
    HRI_Update();         // REVISA ESTADO BOTONES
    Gripper_Update();     // REVISA TIEMPO MOTOR
}