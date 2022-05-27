#ifndef CODE_CAMERA_H_
#define CODE_CAMERA_H_

#include "headfile.h"
#include "Variable.h"

#define IMG_WHITE 255
#define IMG_BLACK 0

void Image_Binary(unsigned char Threshold);

void Bin_Image_Filter(void);

void Image_Compress(void);

void Image_Expand (unsigned char Image_Use[LCD_H][LCD_W], unsigned char imageOut[MT9V03X_H][MT9V03X_W]);

#endif /* CODE_CAMERA_H_ */
