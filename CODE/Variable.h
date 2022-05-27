#ifndef CODE_VARIABLE_H_
#define CODE_VARIABLE_H_

#include "headfile.h"

extern int Scheme_selection;

extern unsigned char image_threshold;  //图像阈值

// 压缩高宽
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

/**  @brief    转向误差  */
extern short g_sSteeringError;
/**  @brief    丢线标志位  */
extern unsigned char g_ucIsNoSide;

/**  @brief    环岛标志位  */
extern unsigned char g_ucFlagRoundabout;

/**  @brief    十字标志位  */
extern unsigned char g_ucFlagCross;

/**  @brief    斑马线标志位  */
extern unsigned char g_ucFlagZebra;

/**  @brief    Y型岔口标志位  */
extern unsigned char g_ucFlagFork;
extern unsigned char g_ucForkNum;

/**  @brief    T型岔口标志位  */
extern unsigned char g_ucFlagT;

extern int ServoDuty;
extern unsigned char Servo_P;

//轮子的定义

extern int16 Left_rear_speed;/*右前轮速度*/
extern int16 Right_rear_speed;/*右后轮速度*/

#endif /* CODE_VARIABLE_H_ */
