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
//将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


//工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
//工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
//然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
//一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

//对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用enableInterrupts();来开启中断嵌套
//简单点说实际上进入中断后TC系列的硬件自动调用了disableInterrupts();来拒绝响应任何的中断，因此需要我们自己手动调用enableInterrupts();来开启中断的响应。

/*
 * 1：龙邱
 * 2：博特
 * 3：上交大
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

    mt9v03x_finish_flag = 0;//在图像使用完毕后  务必清除标志位，否则不会开始采集下一幅图像
    //注意：一定要在图像使用完毕后在清除此标志位

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

    mt9v03x_finish_flag = 0;//在图像使用完毕后  务必清除标志位，否则不会开始采集下一幅图像

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

    mt9v03x_finish_flag = 0;//在图像使用完毕后  务必清除标志位，否则不会开始采集下一幅图像



}
*/

int core0_main(void) {
    get_clk();//获取时钟频率  务必保留

    //用户在此处调用各种初始化函数等
    //**************************传感器模块初始化**************************

    lcd_init();     //初始化TFT屏幕
    //*****************************************************************
    mt9v03x_init();//初始化摄像头
    //*****************************************************************
    gpio_init(P20_8, GPO, 0, PUSHPULL);//设置P20_8为输出 默认输出低电平  PUSHPULL：推挽输出
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

    //等待所有核心初始化完毕
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();

    while (TRUE) {
        //用户在此处编写任务代码
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

                mt9v03x_finish_flag = 0;//在图像使用完毕后  务必清除标志位，否则不会开始采集下一幅图像
            }

            gpio_toggle(P20_8);//翻转IO

        }

    }
}

#pragma section all restore


