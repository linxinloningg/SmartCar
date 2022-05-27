#ifndef CODE_LQOPENSOURCE_H_
#define CODE_LQOPENSOURCE_H_

#include "headfile.h"

/**  @brief    ������  */
#define ROAD_MAIN_ROW      40

/**  @brief    ʹ����ʼ��  */
#define ROAD_START_ROW     115

/**  @brief    ʹ�ý�����  */
#define ROAD_END_ROW       10

//*****************************************ͼ����*************************************************************************
unsigned char RoadIsNoSide(unsigned char imageInput[LCD_H][LCD_W], unsigned char imageOut[LCD_H][2],
                           unsigned char lineIndex);

void RoadNoSideProcess(unsigned char Image_Use[LCD_H][LCD_W], unsigned char imageOut[LCD_H][2],
                       unsigned char mode, unsigned char lineIndex);

unsigned char ImageGetSide(unsigned char Image_Use[LCD_H][LCD_W], unsigned char imageOut[LCD_H][2]);

unsigned char UpdownSideGet(unsigned char Image_Use[LCD_H][LCD_W], unsigned char imageOut[2][LCD_W]);
//*****************************************======*************************************************************************

//****************************************Ԫ��ʶ��***************************************************************************
// ��������
unsigned char
RoadIsRoundabout(unsigned char Upimage[2][LCD_W], unsigned char imageInput[LCD_H][LCD_W], unsigned char image[LCD_H][2],
                 unsigned char *flag);

void RoundaboutProcess(unsigned char imageInput[LCD_H][LCD_W], unsigned char imageSide[LCD_H][2],
                       unsigned char UpdowmSide[2][LCD_W], unsigned char *state);

// T�ֲ���
unsigned char RoadIsT(unsigned char imageUp[2][LCD_W], unsigned char imageSide[LCD_H][2], unsigned char *flag);

unsigned char
TProcess(unsigned char imageInput[LCD_H][LCD_W], unsigned char imageUp[2][LCD_W], unsigned char imageSide[LCD_H][2],
         unsigned char *flag);

//ʮ�ֲ���
unsigned char RoadIsCross(unsigned char imageSide[LCD_H][2], unsigned char *flag);

void CrossProcess(unsigned char  imageInput[LCD_H][LCD_W], unsigned char  imageSide[LCD_H][2], unsigned char * state);

// Y�Ͳ�·
unsigned char RoadIsFork(unsigned char imageInput[2][LCD_W], unsigned char imageSide[LCD_H][2], unsigned char *flag,
                         unsigned char *pY);

void ForkProcess(unsigned char UpSideInput[2][LCD_W], unsigned char imageSide[LCD_H][2], unsigned char *state);

//������
void ZebraProcess(unsigned char imageSide[LCD_H][2], unsigned char state, short int* speed);
//*****************************************======*************************************************************************

//*****************************************��������*************************************************************************

void CameraCar(void);
//*****************************************======*************************************************************************

//*****************************************������ʾ*************************************************************************
#define AT_IMAGE(img, x, y)          ((img)->data[(y)*(img)->step+(x)])
#define AT_IMAGE_CLIP(img, x, y)     AT_IMAGE(img, clip(x, 0, (img)->width-1), clip(y, 0, (img)->height-1))

#define DEF_IMAGE(ptr, w, h)         {.data=ptr, .width=w, .height=h, .step=w}
#define ROI_IMAGE(img, x1, y1, w, h) {.data=&AT_IMAGE(img, x1, y1), .width=w, .height=h, .step=img.width}

typedef struct image {
    unsigned char *data;
    unsigned long int width;
    unsigned long int height;
    unsigned long int step;
} image_t;

extern image_t img_line;

void draw_line(image_t *img);
//*****************************************======*************************************************************************

#endif /* CODE_LQOPENSOURCE_H_ */
