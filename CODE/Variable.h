#ifndef CODE_VARIABLE_H_
#define CODE_VARIABLE_H_

#include "headfile.h"

extern int Scheme_selection;

extern unsigned char image_threshold;  //ͼ����ֵ

// ѹ���߿�
#define LCD_H 120
#define LCD_W 160

extern unsigned char Small_Image[LCD_H][LCD_W];
extern unsigned char Extend_Image[MT9V03X_H][MT9V03X_W];
extern unsigned char Binary_Image[LCD_H][LCD_W];
extern unsigned char Filter_Image[LCD_H][LCD_W];

extern unsigned char ImageSide[LCD_H][2];
extern unsigned char UpdowmSide[LCD_W][2];
extern unsigned char line_data[LCD_H][LCD_W];

/***************************************************/
extern int leftline[LCD_H];

extern int rightline[LCD_H];

extern int centerline[LCD_H];

extern int topline[LCD_W];
/***************************************************/

/**  @brief    ת�����  */
extern short g_sSteeringError;
/**  @brief    ���߱�־λ  */
extern unsigned char g_ucIsNoSide;

/**  @brief    ������־λ  */
extern unsigned char g_ucFlagRoundabout;

/**  @brief    ʮ�ֱ�־λ  */
extern unsigned char g_ucFlagCross;

/**  @brief    �����߱�־λ  */
extern unsigned char g_ucFlagZebra;

/**  @brief    Y�Ͳ�ڱ�־λ  */
extern unsigned char g_ucFlagFork;
extern unsigned char g_ucForkNum;

/**  @brief    T�Ͳ�ڱ�־λ  */
extern unsigned char g_ucFlagT;

extern int ServoDuty;
extern unsigned char Servo_P;

//���ӵĶ���

extern int16 Left_rear_speed;/*��ǰ���ٶ�*/
extern int16 Right_rear_speed;/*�Һ����ٶ�*/

#endif /* CODE_VARIABLE_H_ */
