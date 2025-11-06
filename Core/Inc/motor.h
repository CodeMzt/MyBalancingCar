#ifndef __MOTOR_H__
#define __MOTOR_H__

/**
*	PWM分频后的 ARR+1
*	计划实现的最大转速 PWM_REALMIN<=PWM_MIN<=PWM_MAX<=PWM_REALMAX
*/
#define PWM_REALMAX 1000
#define PWM_REALMIN -1000
#define PWM_MAX 1000
#define PWM_MIN -1000

void Load(int motor1,int motor2);
void Motor_Init(void);

#endif
