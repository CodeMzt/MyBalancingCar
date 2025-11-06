/********************************************************************************

* @File motor.c

* @Author: Ma Ziteng

* @Version: 1.0

* @Date: 2025-10

* @Description: 电机控制模块

********************************************************************************/

#include "motor.h"
#include "tim.h"
#include "stm32f1xx_hal.h"



#define ABS(x) (x>=0?x:-x)



/**
* @brief 电机转速限幅
* @param motor1 			1号电机目标速度
*		@range -1000~+1000
* @param motor1 			2号电机目标速度
*		@range -1000~+1000
* @return  
*/
void Motor_Limit(int* motor1,int* motor2){
	(*motor1) = (*motor1)>PWM_MAX?PWM_MAX:((*motor1)<PWM_MIN?PWM_MIN:(*motor1));
	(*motor2) = (*motor2)>PWM_MAX?PWM_MAX:((*motor2)<PWM_MIN?PWM_MIN:(*motor2));
}



/**
* @brief 电机加载速度
* @param motor1 			1号电机速度
*		@range -1000~+1000
* @param motor1 			2号电机速度
*		@range -1000~+1000
* @return  
*/
void Load(int motor1,int motor2){
	Motor_Limit(&motor1,&motor2);
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,(motor1>0?GPIO_PIN_SET:GPIO_PIN_RESET));
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,(motor1<0?GPIO_PIN_SET:GPIO_PIN_RESET));
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,ABS(motor1));
	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,(motor2>0?GPIO_PIN_SET:GPIO_PIN_RESET));
	HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,(motor2<0?GPIO_PIN_SET:GPIO_PIN_RESET));
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,ABS(motor2));
}
/**
* @brief 初始化
* @param  
* @return  
*/
void Motor_Init(void){
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
}
