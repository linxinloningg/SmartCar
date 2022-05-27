#include "headfile.h"
#include "Variable.h"


int Scheme_selection = 1;

unsigned char image_threshold;  //图像阈值

unsigned char Small_Image[LCD_H][LCD_W];
unsigned char Extend_Image[MT9V03X_H][MT9V03X_W];
unsigned char Binary_Image[LCD_H][LCD_W];
unsigned char Filter_Image[LCD_H][LCD_W];


/*!
* @brief    边线数组
*
* @note
*/
unsigned char ImageSide[LCD_H][2];

unsigned char UpdowmSide[LCD_W][2];

/******************************************************/
int centerline[LCD_H];
int leftline[LCD_H];
int rightline[LCD_H];

int topline[LCD_W];
/******************************************************/

unsigned char line_data[LCD_H][LCD_W];

/**  @brief    转向误差  */
short g_sSteeringError = 0;

/**  @brief    丢线标志位  */
unsigned char g_ucIsNoSide = 0;

/**  @brief    环岛标志位  */
unsigned char g_ucFlagRoundabout  = 0;

/**  @brief    十字标志位  */
unsigned char g_ucFlagCross  = 0;

/**  @brief    斑马线标志位  */
unsigned char g_ucFlagZebra  = 0;

/**  @brief    Y型岔口标志位  */
unsigned char g_ucFlagFork  = 0;
unsigned char g_ucForkNum  = 0;

/**  @brief    T型岔口标志位  */
unsigned char g_ucFlagT = 0;

int ServoDuty = 0;
unsigned char Servo_P = 12;

//轮子的定义

short int Right_rear_speed;
short int Left_rear_speed;
