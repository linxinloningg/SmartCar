#ifndef CODE_SERVO_H_
#define CODE_SERVO_H_

#include "headfile.h"

#define Servo_Delta 280           //舵机左右转动的差值，与舵机型号，拉杆和舵机臂长有关
#define Servo_Center_Mid 1370      //舵机直行中值，
#define Servo_Left_Max   (Servo_Center_Mid+Servo_Delta)      //舵机左转极限值
#define Servo_Right_Min  (Servo_Center_Mid-Servo_Delta)      //舵机右转极限值

void ServoInit(void);

void ServoCtrl(short ServoDuty);

void TestServo(void);

#endif /* CODE_SERVO_H_ */


