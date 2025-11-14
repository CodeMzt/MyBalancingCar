/********************************************************************************

* @File Encoder.c

* @Author: Ma Ziteng

* @Version: 1.0

* @Date: 2025-10

* @Description: 编码器测速模块

********************************************************************************/
#include "Encoder.h"

/**
* @brief 编码器测速
* @param htimx 			目标定时器值
* @return temp 			读取的计数值
*/
int ReadSpeed(TIM_HandleTypeDef* htimx){
	int temp;
	temp = (short)__HAL_TIM_GetCounter(htimx); //将16位改成8位，使之有符号 int16_t(正数部分)->int8_t
	__HAL_TIM_SetCounter(htimx,0);
	return temp;
}
/**
* @brief 编码器测速初始化
* @param 
* @return 
*/
void Encoder_Init(void){
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
}
