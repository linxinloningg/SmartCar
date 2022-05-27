#include "headfile.h"
#include "Binarization.h"
#include "camera.h"
#include "Encoder.h"
#include "Motor.h"
#include "Servo.h"
#include "Variable.h"
#include "SJUOpenSource.h"
#include "LQOpenSource.h"

#include <stdlib.h>

/*
 *  @brief  大津法二值化0.8ms程序（实际测试4ms在TC264中）
 *  @date:   2018-10
 *  @since      v1.2
 *  *image ：图像地址
 *  width:  图像宽
 *  height：图像高
 *  @author     Z小旋
 */
void otsuThreshold(unsigned char *image, unsigned short width, unsigned short height) {
#define GrayScale 256
    int pixelCount[GrayScale] = {0};//每个灰度值所占像素个数
    float pixelPro[GrayScale] = {0};//每个灰度值所占总像素比例
    int i, j;
    int Sumpix = width * height;   //总像素点
    unsigned char threshold = 0;
    unsigned char *data = image;  //指向像素数据的指针

    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            pixelCount[(int) data[i * width + j]]++;  //将像素值作为计数数组的下标
            //   pixelCount[(int)image[i][j]]++;    若不用指针用这个
        }
    }
    float u = 0;
    for (i = 0; i < GrayScale; i++) {
        pixelPro[i] = (float) pixelCount[i] / Sumpix;   //计算每个像素在整幅图像中的比例
        u += i * pixelPro[i];  //总平均灰度
    }

    float maxVariance = 0.0;  //最大类间方差
    float w0 = 0, avgValue = 0;  //w0 前景比例 ，avgValue 前景平均灰度
    for (int i = 0; i < 256; i++)     //每一次循环都是一次完整类间方差计算 (两个for叠加为1个)
    {
        w0 += pixelPro[i];  //假设当前灰度i为阈值, 0~i 灰度像素所占整幅图像的比例即前景比例
        avgValue += i * pixelPro[i];

        float variance = pow((avgValue / w0 - u), 2) * w0 / (1 - w0);    //类间方差
        if (variance > maxVariance) {
            maxVariance = variance;
            threshold = (unsigned char) i;
        }
    }

    image_threshold = threshold;
}

void lq_otsuThreshold(unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W]) {
    unsigned char i, j;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelshortegralBack = 0;
    unsigned long Pixelshortegral = 0;
    signed long PixelshortegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    unsigned char MinValue, MaxValue;
    unsigned char Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j < MT9V03X_H; j++) {
        for (i = 0; i < MT9V03X_W; i++) {
            HistoGram[mt9v03x_image[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++);        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--); //获取最大灰度的值

    if (MaxValue == MinValue)
        image_threshold = MaxValue;         // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        image_threshold = MinValue;        // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j];        //  像素总数

    Pixelshortegral = 0;
    for (j = MinValue; j <= MaxValue; j++) {
        Pixelshortegral += HistoGram[j] * j;        //灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++) {
        PixelBack = PixelBack + HistoGram[j];     //前景像素点数
        PixelFore = Amount - PixelBack;           //背景像素点数
        OmegaBack = (float) PixelBack / Amount;   //前景像素百分比
        OmegaFore = (float) PixelFore / Amount;   //背景像素百分比
        PixelshortegralBack += HistoGram[j] * j;  //前景灰度值
        PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //背景灰度值
        MicroBack = (float) PixelshortegralBack / PixelBack;   //前景灰度百分比
        MicroFore = (float) PixelshortegralFore / PixelFore;   //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);   //计算类间方差
        if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    image_threshold = Threshold;                        //返回最佳阈值;
}

void averageThreshold(unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W]) {

    unsigned short i = 0, j = 0;
    unsigned long tv = 0;
    //累加
    for (i = 0; i < MT9V03X_H; i++) {
        for (j = 0; j < MT9V03X_W; j++) {
            tv += mt9v03x_image[i][j];   //累加
        }
    }
    image_threshold = (unsigned short) (tv / MT9V03X_H / MT9V03X_W);   //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100
    image_threshold = image_threshold + 20;      //此处阈值设置，根据环境的光线来设定
}

void lq_sobel(unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W], unsigned char Image_Use[MT9V03X_H][MT9V03X_W],
              unsigned char Threshold) {
    /** 卷积核大小 */
    short KERNEL_SIZE = 3;
    short xStart = KERNEL_SIZE / 2;
    short xEnd = MT9V03X_W - KERNEL_SIZE / 2;
    short yStart = KERNEL_SIZE / 2;
    short yEnd = MT9V03X_H - KERNEL_SIZE / 2;
    short i, j, k;
    short temp[4];
    for (i = yStart; i < yEnd; i++) {
        for (j = xStart; j < xEnd; j++) {
            /* 计算不同方向梯度幅值  */
            temp[0] = -(short) mt9v03x_image[i - 1][j - 1] + (short) mt9v03x_image[i - 1][j + 1]     //{{-1, 0, 1},
                      - (short) mt9v03x_image[i][j - 1] + (short) mt9v03x_image[i][j + 1]        // {-1, 0, 1},
                      - (short) mt9v03x_image[i + 1][j - 1] + (short) mt9v03x_image[i + 1][j + 1];    // {-1, 0, 1}};

            temp[1] = -(short) mt9v03x_image[i - 1][j - 1] + (short) mt9v03x_image[i + 1][j - 1]     //{{-1, -1, -1},
                      - (short) mt9v03x_image[i - 1][j] + (short) mt9v03x_image[i + 1][j]       // { 0,  0,  0},
                      - (short) mt9v03x_image[i - 1][j + 1] + (short) mt9v03x_image[i + 1][j + 1];    // { 1,  1,  1}};

            temp[2] = -(short) mt9v03x_image[i - 1][j] + (short) mt9v03x_image[i][j - 1]       //  0, -1, -1
                      - (short) mt9v03x_image[i][j + 1] + (short) mt9v03x_image[i + 1][j]       //  1,  0, -1
                      - (short) mt9v03x_image[i - 1][j + 1] + (short) mt9v03x_image[i + 1][j - 1];    //  1,  1,  0

            temp[3] = -(short) mt9v03x_image[i - 1][j] + (short) mt9v03x_image[i][j + 1]       // -1, -1,  0
                      - (short) mt9v03x_image[i][j - 1] + (short) mt9v03x_image[i + 1][j]       // -1,  0,  1
                      - (short) mt9v03x_image[i - 1][j - 1] + (short) mt9v03x_image[i + 1][j + 1];    //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* 找出梯度幅值最大值  */
            for (k = 1; k < 4; k++) {
                if (temp[0] < temp[k]) {
                    temp[0] = temp[k];
                }
            }

            if (temp[0] > Threshold) {
                Image_Use[i][j] = IMG_WHITE;
            } else {
                Image_Use[i][j] = IMG_BLACK;
            }
        }
    }
}

void lq_sobelAutoThreshold(unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W],
                           unsigned char Image_Use[MT9V03X_H][MT9V03X_W]) {
    /** 卷积核大小 */
    short KERNEL_SIZE = 3;
    short xStart = KERNEL_SIZE / 2;
    short xEnd = MT9V03X_W - KERNEL_SIZE / 2;
    short yStart = KERNEL_SIZE / 2;
    short yEnd = MT9V03X_H - KERNEL_SIZE / 2;
    short i, j, k;
    short temp[4];
    for (i = yStart; i < yEnd; i++) {
        for (j = xStart; j < xEnd; j++) {
            /* 计算不同方向梯度幅值  */
            temp[0] = -(short) mt9v03x_image[i - 1][j - 1] + (short) mt9v03x_image[i - 1][j + 1]     //{{-1, 0, 1},
                      - (short) mt9v03x_image[i][j - 1] + (short) mt9v03x_image[i][j + 1]       // {-1, 0, 1},
                      - (short) mt9v03x_image[i + 1][j - 1] + (short) mt9v03x_image[i + 1][j + 1];    // {-1, 0, 1}};

            temp[1] = -(short) mt9v03x_image[i - 1][j - 1] + (short) mt9v03x_image[i + 1][j - 1]     //{{-1, -1, -1},
                      - (short) mt9v03x_image[i - 1][j] + (short) mt9v03x_image[i + 1][j]       // { 0,  0,  0},
                      - (short) mt9v03x_image[i - 1][j + 1] + (short) mt9v03x_image[i + 1][j + 1];    // { 1,  1,  1}};

            temp[2] = -(short) mt9v03x_image[i - 1][j] + (short) mt9v03x_image[i][j - 1]       //  0, -1, -1
                      - (short) mt9v03x_image[i][j + 1] + (short) mt9v03x_image[i + 1][j]       //  1,  0, -1
                      - (short) mt9v03x_image[i - 1][j + 1] + (short) mt9v03x_image[i + 1][j - 1];    //  1,  1,  0

            temp[3] = -(short) mt9v03x_image[i - 1][j] + (short) mt9v03x_image[i][j + 1]       // -1, -1,  0
                      - (short) mt9v03x_image[i][j - 1] + (short) mt9v03x_image[i + 1][j]       // -1,  0,  1
                      - (short) mt9v03x_image[i - 1][j - 1] + (short) mt9v03x_image[i + 1][j + 1];    //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* 找出梯度幅值最大值  */
            for (k = 1; k < 4; k++) {
                if (temp[0] < temp[k]) {
                    temp[0] = temp[k];
                }
            }

            /* 使用像素点邻域内像素点之和的一定比例    作为阈值  */
            temp[3] = (short) mt9v03x_image[i - 1][j - 1] + (short) mt9v03x_image[i - 1][j] +
                      (short) mt9v03x_image[i - 1][j + 1]
                      + (short) mt9v03x_image[i][j - 1] + (short) mt9v03x_image[i][j] + (short) mt9v03x_image[i][j + 1]
                      + (short) mt9v03x_image[i + 1][j - 1] + (short) mt9v03x_image[i + 1][j] +
                      (short) mt9v03x_image[i + 1][j + 1];

            if (temp[0] > temp[3] / 12.0f) {
                Image_Use[i][j] = IMG_BLACK;
            } else {
                Image_Use[i][j] = IMG_WHITE;
            }

        }
    }
}
