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

#pragma section all "cpu0_dsram"
//���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��


//���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
//����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
//Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
//һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

//����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ��enableInterrupts();�������ж�Ƕ��
//�򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ�������disableInterrupts();���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ�����enableInterrupts();�������жϵ���Ӧ��

/*
 * 1������
 * 2������
 * 3���Ͻ���
 */

void selection_LQ(void) {
    otsuThreshold(mt9v03x_image, MT9V03X_H, MT9V03X_W);

    // image_threshold = 80;
    // lq_sobel(mt9v03x_image, Image_Use, image_threshold);

    // lq_sobelAutoThreshold(mt9v03x_image, Image_Use);

    Image_Compress();

    Image_Binary(image_threshold);

    Bin_Image_Filter();

    ImageGetSide(Filter_Image, ImageSide);

    UpdownSideGet(Filter_Image, UpdowmSide);

    mt9v03x_finish_flag = 0;//��ͼ��ʹ����Ϻ�  ��������־λ�����򲻻Ὺʼ�ɼ���һ��ͼ��
    //ע�⣺һ��Ҫ��ͼ��ʹ����Ϻ�������˱�־λ

    CameraCar();
}

void selection_SJU(void) {
    otsuThreshold(mt9v03x_image, MT9V03X_H, MT9V03X_W);

    // image_threshold = 80;
    // lq_sobel(mt9v03x_image, Image_Use, image_threshold);

    // lq_sobelAutoThreshold(mt9v03x_image, Image_Use);

    Image_Compress();

    Image_Binary(image_threshold);

    Bin_Image_Filter();

    BT_Binary_lifting_line(Filter_Image);

    mt9v03x_finish_flag = 0;//��ͼ��ʹ����Ϻ�  ��������־λ�����򲻻Ὺʼ�ɼ���һ��ͼ��

}

/*
void selection_TEST(void) {
    otsuThreshold(mt9v03x_image, MT9V03X_H, MT9V03X_W);

    // image_threshold = 80;
    // lq_sobel(mt9v03x_image, Image_Use, image_threshold);

    // lq_sobelAutoThreshold(mt9v03x_image, Image_Use);

    Image_Compress();

    Image_Binary(image_threshold);

    Bin_Image_Filter();

    edge_detec(Filter_Image);

    mt9v03x_finish_flag = 0;//��ͼ��ʹ����Ϻ�  ��������־λ�����򲻻Ὺʼ�ɼ���һ��ͼ��



}
*/

int core0_main(void) {
    get_clk();//��ȡʱ��Ƶ��  ��ر���

    //�û��ڴ˴����ø��ֳ�ʼ��������
    //**************************������ģ���ʼ��**************************

    lcd_init();     //��ʼ��TFT��Ļ
    //*****************************************************************
    mt9v03x_init();//��ʼ������ͷ
    //*****************************************************************
    gpio_init(P20_8, GPO, 0, PUSHPULL);//����P20_8Ϊ��� Ĭ������͵�ƽ  PUSHPULL���������
    gpio_init(P20_9, GPO, 0, PUSHPULL);
    //*****************************************************************
    MotorInit();
    //*****************************************************************
    ServoInit();
    //*****************************************************************
    Left_rear_pwm_set(50);
    Right_rear_pwm_set(50);
    //*****************************************************************
    EncoderInit();

    //�ȴ����к��ĳ�ʼ�����
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();

    while (TRUE) {
        //�û��ڴ˴���д�������
        if (mt9v03x_finish_flag) {

            if (Scheme_selection == 1) {
                selection_LQ();
            } else if (Scheme_selection == 2) {
                selection_SJU();
            } else {
                otsuThreshold(mt9v03x_image, MT9V03X_H, MT9V03X_W);

                // image_threshold = 80;
                // lq_sobel(mt9v03x_image, Image_Use, image_threshold);

                // lq_sobelAutoThreshold(mt9v03x_image, Image_Use);

                Image_Compress();

                Image_Binary(image_threshold);

                Bin_Image_Filter();

                mt9v03x_finish_flag = 0;//��ͼ��ʹ����Ϻ�  ��������־λ�����򲻻Ὺʼ�ɼ���һ��ͼ��
            }

            gpio_toggle(P20_8);//��תIO

        }

    }
}

#pragma section all restore


