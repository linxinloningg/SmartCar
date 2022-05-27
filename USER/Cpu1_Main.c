/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            main
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        ADS v1.2.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-3-23
 ********************************************************************************************************************/

/*
 * 按键{
 * 1: P14.2
 * 2: P14.3
 * 3: P14.4
 * 4: P14.5
 * 5: P14.6
 * 编码器{
 * 0: P20.3
 * 1: P10.3
 * 舵机: P33.9
 * 电机{
 * 1: P02.7
 * 2: P02.6
 * 3: P02.5
 * 4: P02.4
 * }
 * }
 * }
 */

#include "headfile.h"
#include "Binarization.h"
#include "camera.h"
#include "Encoder.h"
#include "Motor.h"
#include "Servo.h"
#include "Variable.h"
#include "SJUOpenSource.h"
#include "LQOpenSource.h"

//#include "debug.h"

#pragma section all "cpu1_dsram"
//将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中

uint8 lcdenable_flag = 1;//isp屏幕的使能，1为开启，0 为关闭

void display_LQ(void) {
    draw_line(&img_line);

    lcd_displayimage032(line_data[0], LCD_W, LCD_H);
}

void display_SJU(void) {
    lcd_displayimage032_line(Filter_Image[0], LCD_W, LCD_H, leftline, centerline, rightline);
}

/*
void display_TEST(void){
    lcd_displayimage032_line(Filter_Image[0], MT9V03X_W, MT9V03X_H, leftline, centerline, rightline);
}
*/

void core1_main(void) {
    disableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    //用户在此处调用各种初始化函数等


    //等待所有核心初始化完毕
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();
    while (TRUE) {
        //用户在此处编写任务代码
        if (mt9v03x_finish_flag) {

            if (lcdenable_flag) {

                if (Scheme_selection == 1) {
                    display_LQ();
                } else if (Scheme_selection == 2) {
                    display_SJU();
                } else {
                    lcd_displayimage032(Filter_Image[0], LCD_W, LCD_H);
                }

                gpio_toggle(P20_9);

            }
        }

    }

}

#pragma section all restore
