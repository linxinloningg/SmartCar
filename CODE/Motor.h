#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

#include "headfile.h"

#define MOTOR_FREQUENCY 10000
#define MOTOR_Mid 1000

#define MOTOR1_DIR   ATOM0_CH0_P21_2              //定义1电机方向控制引脚
#define MOTOR1_PWM   ATOM0_CH1_P21_3    //定义1电机PWM引脚

#define MOTOR2_DIR   ATOM0_CH2_P21_4              //定义2电机方向控制引脚
#define MOTOR2_PWM   ATOM0_CH3_P21_5    //定义2电机PWM引脚

extern double MAX_SPEED_A;
extern double MAX_SPEED_B;

void MotorInit(void);

void Left_rear_pwm_set(double Left_rearvalue);

void Right_rear_pwm_set(double Right_rearvalue);

void Right_Motor_Interface(unsigned char direction, float Motor_Speed);

void Left_Motor_Interface(unsigned char direction, float Motor_Speed);

void TestMotor(void);

#endif /* CODE_MOTOR_H_ */
