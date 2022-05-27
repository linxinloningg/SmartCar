#ifndef CODE_BINARIZATION_H_
#define CODE_BINARIZATION_H_

#include "headfile.h"

//*********************************
#define IMG_WHITE 255
#define IMG_BLACK 0
//************全局变量**************

//**********************************


void otsuThreshold(unsigned char *image, unsigned short width, unsigned short height);     //计算二值化阈值: 大津法

void averageThreshold(unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W]);

void lq_otsuThreshold(unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W]);

void lq_sobel (unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W], unsigned char Image_Use[MT9V03X_H][MT9V03X_W], unsigned char Threshold);

void lq_sobelAutoThreshold (unsigned char mt9v03x_image[MT9V03X_H][MT9V03X_W], unsigned char Image_Use[MT9V03X_H][MT9V03X_W]);

#endif /* CODE_BINARIZATION_H_ */
