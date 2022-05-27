#include "headfile.h"
#include "Binarization.h"
#include "camera.h"
#include "Encoder.h"
#include "Motor.h"
#include "Servo.h"
#include "Variable.h"
#include "SJUOpenSource.h"
#include "LQOpenSource.h"

double MAX_SPEED_A = 3000;
double MAX_SPEED_B = 3000;

void MotorInit(void) {
    gtm_pwm_init(MOTOR1_PWM, MOTOR_FREQUENCY, 0);       //ʹ��P02_5�������PWM���Ƶ��1  PWMƵ��10kHZ
    gtm_pwm_init(MOTOR1_DIR, MOTOR_FREQUENCY, 0);
    gtm_pwm_init(MOTOR2_PWM, MOTOR_FREQUENCY, 0);         //ʹ��P02_6�������PWM���Ƶ��2  PWMƵ��10kHZ
    gtm_pwm_init(MOTOR2_DIR, MOTOR_FREQUENCY, 0);
}

unsigned short A = 0;
unsigned short B = 0;

/*
 * ���pwm����
 */
void Left_rear_pwm_set(double Left_rearvalue) {
    if (Left_rearvalue > 0) {
        if (Left_rearvalue > MAX_SPEED_A)
            Left_rearvalue = MAX_SPEED_A;
        pwm_duty(MOTOR1_PWM, Left_rearvalue);   //������Motor_PWM 0-100����ռ�ձ�0-100
        pwm_duty(MOTOR1_DIR, 0);   //������Motor_PWM 0-100����ռ�ձ�0-100
    } else if (Left_rearvalue < 0) {
        if (Left_rearvalue < -MAX_SPEED_A)
            Left_rearvalue = -MAX_SPEED_A;
        pwm_duty(MOTOR1_PWM, 0);   //������Motor_PWM 0-100����ռ�ձ�0-100
        pwm_duty(MOTOR1_DIR, -Left_rearvalue);   //������Motor_PWM 0-100����ռ�ձ�0-100
    }
    A = Left_rearvalue;
}

/*
 * �Һ�pwm����
 */
void Right_rear_pwm_set(double Right_rearvalue) {
    if (Right_rearvalue > 0) {
        if (Right_rearvalue > MAX_SPEED_B)
            Right_rearvalue = MAX_SPEED_B;
        pwm_duty(MOTOR2_PWM, Right_rearvalue);   //������Motor_PWM 0-100����ռ�ձ�0-100
        pwm_duty(MOTOR2_DIR, 0);   //������Motor_PWM 0-100����ռ�ձ�0-100
    } else if (Right_rearvalue < 0) {
        if (Right_rearvalue < -MAX_SPEED_B)
            Right_rearvalue = -MAX_SPEED_B;
        pwm_duty(MOTOR2_PWM, 0);   //������Motor_PWM 0-100����ռ�ձ�0-100
        pwm_duty(MOTOR2_DIR, -Right_rearvalue);   //������Motor_PWM 0-100����ռ�ձ�0-100
    }
    B = Right_rearvalue;

}

/*
 * �Һ����ӿ�
 */
void Right_Motor_Interface(unsigned char direction, float Motor_Speed) {
    gpio_set(P21_4, direction);
    pwm_duty(MOTOR1_PWM, Motor_Speed * 100);   //�ҵ����Motor_PWM 0-100����ռ�ձ�0-100
}

/*
 * ������ӿ�
 */
void Left_Motor_Interface(unsigned char direction, float Motor_Speed) {
    gpio_set(P21_2, direction);
    pwm_duty(MOTOR2_PWM, Motor_Speed * 100);   //������Motor_PWM 0-100����ռ�ձ�0-100
}

void TestMotor(void) {
    gpio_init(P22_0, GPI, 0, PULLUP);
    gpio_init(P22_1, GPI, 0, PULLUP);
    gpio_init(P22_2, GPI, 0, PULLUP);

    unsigned long duty = 3000;

    unsigned char a = 1, b = 1, c = 1;

    gtm_pwm_init(MOTOR1_PWM, MOTOR_FREQUENCY, MOTOR_Mid);
    gtm_pwm_init(MOTOR1_DIR, MOTOR_FREQUENCY, 0);
    gtm_pwm_init(MOTOR2_PWM, MOTOR_FREQUENCY, MOTOR_Mid);
    gtm_pwm_init(MOTOR2_DIR, MOTOR_FREQUENCY, 0);

    while (TRUE) {
        a = gpio_get(P22_0);
        b = gpio_get(P22_1);
        c = gpio_get(P22_2);

        if (a == 0)      //����KEY0����ռ�ձȼ�С
        {
            duty += 500;
        }
        if (b == 0)      //����KEY2����ռ�ձȼӴ�
        {
            duty -= 500;
        }
        if (c == 0)      //����KEY1����ռ�ձ���ֵ
        {
            duty = 3000;
        }

        pwm_duty(MOTOR1_PWM, duty);
        pwm_duty(MOTOR2_PWM, duty);

        lcd_showint16(2, 2, duty);
        systick_delay_ms(STM0, 200);
    }
}
