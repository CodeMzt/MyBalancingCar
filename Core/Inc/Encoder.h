#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "tim.h"
#include "stm32f1xx_hal.h"

int ReadSpeed(TIM_HandleTypeDef* htimx);
void Encoder_Init(void);

#endif
