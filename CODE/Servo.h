#ifndef CODE_SERVO_H_
#define CODE_SERVO_H_

#include "headfile.h"

#define Servo_Delta 280           //�������ת���Ĳ�ֵ�������ͺţ����˺Ͷ���۳��й�
#define Servo_Center_Mid 1370      //���ֱ����ֵ��
#define Servo_Left_Max   (Servo_Center_Mid+Servo_Delta)      //�����ת����ֵ
#define Servo_Right_Min  (Servo_Center_Mid-Servo_Delta)      //�����ת����ֵ

void ServoInit(void);

void ServoCtrl(short ServoDuty);

void TestServo(void);

#endif /* CODE_SERVO_H_ */


