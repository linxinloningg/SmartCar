#include "headfile.h"
#include "Binarization.h"
#include "camera.h"
#include "Encoder.h"
#include "Motor.h"
#include "Servo.h"
#include "Variable.h"
#include "SJUOpenSource.h"
#include "LQOpenSource.h"

/*---------------------------------------------------------------
 【函    数】Image_Compress
 【功    能】压缩图片
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void Image_Compress(void) {
    int i, j, row, line;
    const float div_h = MT9V03X_H / LCD_H;
    const float div_w = MT9V03X_W / LCD_W;
    for (i = 0; i < LCD_H; i++) {
        row = i * div_h + 0.5;
        for (j = 0; j < LCD_W; j++) {
            line = j * div_w + 0.5;
            Small_Image[i][j] = mt9v03x_image[row][line];
        }
    }

}

/*---------------------------------------------------------------
 【函    数】Image_Expand
 【功    能】图像扩大 188*120
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void Image_Expand (unsigned char Image_Use[LCD_H][LCD_W], unsigned char imageOut[MT9V03X_H][MT9V03X_W])
{
    uint8 i, j, iq, jq;
    for (i = 0; i < LCD_H; i++)
    {
        iq = i * 2;
        for (j = 0; j < LCD_W; j++)
        {
            if (Image_Use[i][j])
            {
                Image_Use[i][j] = 120;
            }
            jq = j * 2;
            imageOut[iq][jq] = imageOut[iq + 1][jq + 1] = imageOut[iq][jq + 1] = imageOut[iq + 1][jq] = Image_Use[i][j];
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
//特别注意:图像大于阈值即为白，小于阈值即为黑
//-------------------------------------------------------------------------------------------------------------------
void Image_Binary(unsigned char Threshold) {

    for (int i = 0; i < LCD_H; i++) {
        for (int j = 0; j < LCD_W; j++) {
            if (Small_Image[i][j] <= Threshold)//进行二值化之前只是得到阈值
            {
                Binary_Image[i][j] = IMG_BLACK;//0是黑色
            } else {
                Binary_Image[i][j] = IMG_WHITE;//1是白色
            }
        }
    }
}

/*---------------------------------------------------------------
 【函    数】Bin_Image_Filter
 【功    能】过滤噪点
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void Bin_Image_Filter(void) {
    short nr; //行
    short nc; //列

    for (int num = 0; num < LCD_H; num++) {
            memcpy(Filter_Image[num], Binary_Image[num], LCD_W);
        }

    for (nr = 1; nr < LCD_H - 1; nr++) {
        for (nc = 1; nc < LCD_W - 1; nc = nc + 1) {
            if ((Binary_Image[nr][nc] == IMG_BLACK)
                &&
                (Binary_Image[nr - 1][nc] + Binary_Image[nr + 1][nc] + Binary_Image[nr][nc + 1] + Binary_Image[nr][nc - 1] >
                 255 * 2)) {
                Filter_Image[nr][nc] = IMG_WHITE;
            } else if ((Binary_Image[nr][nc] == IMG_WHITE)
                       &&
                       (Binary_Image[nr - 1][nc] + Binary_Image[nr + 1][nc] + Binary_Image[nr][nc + 1] + Binary_Image[nr][nc - 1] <
                        255 * 2)) {
                Filter_Image[nr][nc] = IMG_BLACK;
            }
        }
    }

    // 除单一的噪点
    unsigned char temp = 0;

    for (int i = 1; i < LCD_H - 1; i++) {
        for (int j = 1; j < LCD_W - 1; j++) {
            temp = Binary_Image[i - 1][j - 1] + Binary_Image[i - 1][j] + Binary_Image[i - 1][j + 1] +
                   Binary_Image[i][j - 1] + Binary_Image[i][j] + Binary_Image[i][j + 1] +
                   Binary_Image[i + 1][j - 1] + Binary_Image[i + 1][j] + Binary_Image[i + 1][j + 1];

            /* 邻域内5个点是边沿 则保留该点 可以调节这里优化滤波效果 */
            if (temp > 4) {
                Filter_Image[i][j] = IMG_WHITE;
            } else {
                Filter_Image[i][j] = IMG_BLACK;
            }

        }
    }
}


