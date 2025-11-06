/********************************************************************************

* @File sr04.c

* @Author: Ma Ziteng

* @Version: 1.0

* @Date: 2025-10

* @Description: 超声波测距

********************************************************************************/
#include "sr04.h"
#include "gpio.h"
#include "tim.h"
#include "delay.h"

uint16_t count;
double distance;

/**
  * @brief		发送测速信号
  * @param		无
  * @param		无
  * @return		无
  */
void sr04_Send(void){
	HAL_GPIO_WritePin(echo_GPIO_Port,echo_Pin,GPIO_PIN_SET);
	Delay_us(12);
	HAL_GPIO_WritePin(echo_GPIO_Port,echo_Pin,GPIO_PIN_RESET);
	Delay_us(60);
}
/**
  * @brief		接受信号中断函数
  * @param	GPIO_Pin	接受信号引脚
  * @return		无
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin != trig_Pin)
		return;
	if(HAL_GPIO_ReadPin(trig_GPIO_Port,trig_Pin)==SET){
		__HAL_TIM_SetCounter(&htim3,0);
		HAL_TIM_Base_Start(&htim3);
	}
	else{
		HAL_TIM_Base_Stop(&htim3);
		count = __HAL_TIM_GetCounter(&htim3);
		distance = count*0.000001*17000;
	}
}
