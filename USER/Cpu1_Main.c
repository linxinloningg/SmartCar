/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            main
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ3184284598)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        ADS v1.2.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-3-23
 ********************************************************************************************************************/

/*
 * ����{
 * 1: P14.2
 * 2: P14.3
 * 3: P14.4
 * 4: P14.5
 * 5: P14.6
 * ������{
 * 0: P20.3
 * 1: P10.3
 * ���: P33.9
 * ���{
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
//���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��

uint8 lcdenable_flag = 1;//isp��Ļ��ʹ�ܣ�1Ϊ������0 Ϊ�ر�

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
    //�û��ڴ˴����ø��ֳ�ʼ��������


    //�ȴ����к��ĳ�ʼ�����
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();
    while (TRUE) {
        //�û��ڴ˴���д�������
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
