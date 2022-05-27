#include "headfile.h"
#include "Binarization.h"
#include "camera.h"
#include "Encoder.h"
#include "Motor.h"
#include "Servo.h"
#include "Variable.h"
#include "SJUOpenSource.h"
#include "LQOpenSource.h"

void ServoInit(void) {
    gtm_pwm_init(ATOM0_CH1_P33_9, 100, 0);//∂Êª˙PWM

    pwm_duty(ATOM0_CH1_P33_9, Servo_Center_Mid);
}

void ServoCtrl(short ServoDuty) {
    if (ServoDuty > Servo_Left_Max)
        ServoDuty = Servo_Left_Max;
    else if (ServoDuty < Servo_Right_Min)
        ServoDuty = Servo_Right_Min;
    pwm_duty(ATOM0_CH1_P33_9, ServoDuty);

}

void TestServo(void) {
    gpio_init(P22_0, GPI, 0, PULLUP);
    gpio_init(P22_1, GPI, 0, PULLUP);
    gpio_init(P22_2, GPI, 0, PULLUP);

    unsigned char a = 1, b = 1, c = 1;

    short int duty = Servo_Center_Mid;

    gtm_pwm_init(ATOM0_CH1_P33_9, 100, 1500);
    pwm_duty(ATOM0_CH1_P33_9, Servo_Center_Mid);

    while (TRUE) {
        a = gpio_get(P22_0);
        b = gpio_get(P22_1);
        c = gpio_get(P22_2);
        if (a == 0) {
            if (duty > 10)  //∑¿÷πduty≥¨
            {
                duty -= 10;
            }
        }
        if (b == 0) {
            duty = Servo_Center_Mid;
        }
        if (c == 0) {
            duty += 10;
        }

        pwm_duty(ATOM0_CH1_P33_9, duty);

        lcd_showint16(0, 0, duty);

        systick_delay_ms(STM0, 100);
    }
}
