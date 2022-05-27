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
 *  @brief  ��򷨶�ֵ��0.8ms����ʵ�ʲ���4ms��TC264�У�
 *  @date:   2018-10
 *  @since      v1.2
 *  *image ��ͼ���ַ
 *  width:  ͼ���
 *  height��ͼ���
 *  @author     ZС��
 */
void otsuThreshold(unsigned char *image, unsigned short width, unsigned short height) {
#define GrayScale 256
    int pixelCount[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ���ظ���
    float pixelPro[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ�����ر���
    int i, j;
    int Sumpix = width * height;   //�����ص�
    unsigned char threshold = 0;
    unsigned char *data = image;  //ָ���������ݵ�ָ��

    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            pixelCount[(int) data[i * width + j]]++;  //������ֵ��Ϊ����������±�
            //   pixelCount[(int)image[i][j]]++;    ������ָ�������
        }
    }
    float u = 0;
    for (i = 0; i < GrayScale; i++) {
        pixelPro[i] = (float) pixelCount[i] / Sumpix;   //����ÿ������������ͼ���еı���
        u += i * pixelPro[i];  //��ƽ���Ҷ�
    }

    float maxVariance = 0.0;  //�����䷽��
    float w0 = 0, avgValue = 0;  //w0 ǰ������ ��avgValue ǰ��ƽ���Ҷ�
    for (int i = 0; i < 256; i++)     //ÿһ��ѭ������һ��������䷽����� (����for����Ϊ1��)
    {
        w0 += pixelPro[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷ�������ռ����ͼ��ı�����ǰ������
        avgValue += i * pixelPro[i];

        float variance = pow((avgValue / w0 - u), 2) * w0 / (1 - w0);    //��䷽��
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
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��;
    unsigned char MinValue, MaxValue;
    unsigned char Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ

    for (j = 0; j < MT9V03X_H; j++) {
        for (i = 0; i < MT9V03X_W; i++) {
            HistoGram[mt9v03x_image[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++);        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--); //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)
        image_threshold = MaxValue;         // ͼ����ֻ��һ����ɫ
    if (MinValue + 1 == MaxValue)
        image_threshold = MinValue;        // ͼ����ֻ�ж�����ɫ

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j];        //  ��������

    Pixelshortegral = 0;
    for (j = MinValue; j <= MaxValue; j++) {
        Pixelshortegral += HistoGram[j] * j;        //�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++) {
        PixelBack = PixelBack + HistoGram[j];     //ǰ�����ص���
        PixelFore = Amount - PixelBack;           //�������ص���
        OmegaBack = (float) PixelBack / Amount;   //ǰ�����ذٷֱ�
        OmegaFore = (float) PixelFore / Amount;   //�������ذٷֱ�
        PixelshortegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
        PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //�����Ҷ�ֵ
        MicroBack = (float) PixelshortegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
        MicroFore = (float) PixelshortegralFore / PixelFore;   //�����ҶȰٷֱ�
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);   //������䷽��
        if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    image_threshold = Threshold;                        //���������ֵ;
}

void averageThreshold(unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W]) {

    unsigned short i = 0, j = 0;
    unsigned long tv = 0;
    //�ۼ�
    for (i = 0; i < MT9V03X_H; i++) {
        for (j = 0; j < MT9V03X_W; j++) {
            tv += mt9v03x_image[i][j];   //�ۼ�
        }
    }
    image_threshold = (unsigned short) (tv / MT9V03X_H / MT9V03X_W);   //��ƽ��ֵ,����Խ��ԽС��ȫ��Լ35��������ĻԼ160��һ������´�Լ100
    image_threshold = image_threshold + 20;      //�˴���ֵ���ã����ݻ����Ĺ������趨
}

void lq_sobel(unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W], unsigned char Image_Use[MT9V03X_H][MT9V03X_W],
              unsigned char Threshold) {
    /** ����˴�С */
    short KERNEL_SIZE = 3;
    short xStart = KERNEL_SIZE / 2;
    short xEnd = MT9V03X_W - KERNEL_SIZE / 2;
    short yStart = KERNEL_SIZE / 2;
    short yEnd = MT9V03X_H - KERNEL_SIZE / 2;
    short i, j, k;
    short temp[4];
    for (i = yStart; i < yEnd; i++) {
        for (j = xStart; j < xEnd; j++) {
            /* ���㲻ͬ�����ݶȷ�ֵ  */
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

            /* �ҳ��ݶȷ�ֵ���ֵ  */
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
    /** ����˴�С */
    short KERNEL_SIZE = 3;
    short xStart = KERNEL_SIZE / 2;
    short xEnd = MT9V03X_W - KERNEL_SIZE / 2;
    short yStart = KERNEL_SIZE / 2;
    short yEnd = MT9V03X_H - KERNEL_SIZE / 2;
    short i, j, k;
    short temp[4];
    for (i = yStart; i < yEnd; i++) {
        for (j = xStart; j < xEnd; j++) {
            /* ���㲻ͬ�����ݶȷ�ֵ  */
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

            /* �ҳ��ݶȷ�ֵ���ֵ  */
            for (k = 1; k < 4; k++) {
                if (temp[0] < temp[k]) {
                    temp[0] = temp[k];
                }
            }

            /* ʹ�����ص����������ص�֮�͵�һ������    ��Ϊ��ֵ  */
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
