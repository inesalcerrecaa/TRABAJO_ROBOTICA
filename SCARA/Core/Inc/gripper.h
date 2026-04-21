#ifndef GRIPPER_H
#define GRIPPER_H

#include <stdint.h>

void  Gripper_Init(void);
char* Gripper_GetColorActual(void);
char* Gripper_SiguienteColor(void);
void Gripper_Reset(void);

#endif // GRIPPER_H
