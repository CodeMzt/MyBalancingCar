/********************************************************************************

* @File pid.c

* @Author: Ma Ziteng

* @Version: 1.0

* @Date: 2025-10

* @Description: PID控制模块

********************************************************************************/




#include "pid.h"
#include "Encoder.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050.h"
#include "motor.h"
#include "oled.h"
#include "usart.h"



/**
* @variety 标志
*	orientation	方向
*		orientation[0]:前进标志
*		orientation[1]:后退标志
*		orientation[2]:左转标志
*		orientation[3]:右转标志
*/
short orientation[4];



/**
* @variety 传感器参数
*	pitch roll yaw				角度值
*	gyrox	gyroy	gyroz			角速度值
*	accx accy accz				加速度值
*	Encoder1 Encoder2			编码器速度值
*/
float pitch,roll,yaw;
short gyrox,gyroy,gyroz;
short accx,accyz,accz;
int Encoder1,Encoder2;



uint8_t rx_buff[2];



/**
* @variety 			闭环控制中间量
*	Vertical_Out
*	Velocity_Out
* Turn_Out
*	Target_Speed
*	Target_Turn
* OffsetAngle		平衡时的偏移量，因为小车中心不在正中央;也可以抵消恒定误差
*/
float Vertical_Out,Velocity_Out,Turn_Out,Target_Speed,Target_Turn;
float OffsetAngle = -4.6;



/**
* @variety 比例系数
*	KP	比例值，用于乘以差值			现在	
*	KI	积分值，用于累积差值 			过去
*	KD	微分值，用于乘以变化率		将来	
*/

float Vertical_KP=28,Vertical_KD=0.095;
float Velocity_KP=-0.69,Velocity_KI=0;
float Turn_KP=20,Turn_KD=-0.000007;





/**
* @brief 	直立环PD控制器
* @param 	Med 			目标角度值
* @param 	Angle		当前角度值
* @param 	gyro_X		当前角度速度值(X分量)
*
* @return 返回控制值
*	@note 	不用PID，否则参数过多
*/
int Vertical(float Med,float Angle,float gyro_X){
	return Vertical_KP * (Angle - Med) + Vertical_KD * gyro_X;
}



/**
* @brief 	速度环PI控制器
* @param 	Target 			期望速度(实际为2倍)
* @param 	Encoder_1		当前1号电机速度 满速约120
* @param 	Encoder_2		当前2号电机速度	满速约120
*	@note 	无法直接测得积分值，故不用传这个参数，我们自己算
*
* @return 返回控制值
*	@note 	不用PID，否则参数过多
*/
int Velocity(int Target,int Encoder_1,int Encoder_2){
	Velocity_KI=Velocity_KP/200.0;
	//上一次的值;积分值
	static int Encoder = 0,Encoder_S;
	//滤波系数
	static float k = 0.8;
	//低通滤波:MPU6050数据抖动很厉害，让数据不会前后差很多
	Encoder = k * (Encoder_1 + Encoder_2 - Target) + (1 - k)* Encoder;
	//累积
	Encoder_S += Encoder;
	/**
	积分限幅 -7000->7000
	*/
	Encoder_S = Encoder_S>7000?7000:(Encoder_S<(-7000)?(-7000):Encoder_S);
	
	return Velocity_KP * Encoder + Velocity_KI * Encoder_S;
}



/**
* @brief 	转向环PD控制器
* @param 	Angle 		目标角度值
* @param 	gyro_Z		当前角度速度值(Z分量)
*
* @return 返回控制值
*	@note 	当前角度值为0
*/
int Turn(float Angle,float gyro_Z){
	return Turn_KP * Angle  + Turn_KD * gyro_Z;
}



/**
* @brief 	总控制
* @param 	无
*
* @return 无
*	@note 	
*/
void Control(void){
	int PWM_out1,PWM_out2;
	
	//获取传感器数据
	Encoder1  = ReadSpeed(&htim2);
	Encoder2  = -ReadSpeed(&htim4);
	mpu_dmp_get_data(&pitch,&roll,&yaw);

	MPU_Get_Accelerometer(&accx,&accyz,&accz);
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	
	//遥控
	if(orientation[0])
		Target_Speed=(Target_Speed==35)?35:Target_Speed+0.1;
	else if(orientation[1])
		Target_Speed=(Target_Speed==-35)?-35:Target_Speed-0.1;
	else Target_Speed=0;
	if(orientation[2])
		Target_Turn=5;
	else if(orientation[3])
		Target_Turn=-5;
	else Target_Turn=0;
	
	//传入PID控制器
	Velocity_Out = Velocity(Target_Speed,Encoder1,Encoder2);					//外环
	Vertical_Out = Vertical(Velocity_Out + OffsetAngle,roll,gyrox);		//内环
	Turn_Out = Turn(Target_Turn,gyroz);
	
	//差速转向模型
	PWM_out1 = Vertical_Out - Turn_Out;
	PWM_out2 = Vertical_Out + Turn_Out;
	
	//Load(100,100);
	
	Load(PWM_out1,PWM_out2);
}
