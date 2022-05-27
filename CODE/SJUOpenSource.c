#include "headfile.h"
#include "Binarization.h"
#include "camera.h"
#include "Encoder.h"
#include "Motor.h"
#include "Servo.h"
#include "Variable.h"
#include "SJUOpenSource.h"
#include "LQOpenSource.h"
// (原点)
/* ------------------------------------------------------------------  60
 * |*                             |                                  |
 * |                              |                                  |
 * |                              |                                  |
 * |                              |                                  |
 * |                              |                                  |
 * |                              |                                  |
 * |                              |                                  |
 * |                              |                                  |
 * |-----------------------------------------------------------------|
 * 80
 *
 */

/*---------------------------------------------------------------
 【函    数】BT_Binary_lifting_line
 【功    能】寻找除了元素之外的所有普通边线
 【参    数】无
 【返 回 值】无
 【基本思路】 从下往上扫，连中间往两边扫，上一行的中线值记录下来，作为这一行往两边扫的起始值
                       过远的线视为无效线，不处理（在圆环地方强反光可能会有一些效果）
 ----------------------------------------------------------------*/
/*************基本边线参数*******************/
int old_line = LCD_W / 2;   //上一次扫到中线
int fiv_width[LCD_W];  //赛道宽度


unsigned char leftfindflag[LCD_H];  //左边界标志位


unsigned char rightfindflag[LCD_H];      //右边界标志位


int scaned_rows;
int lost_rows = 0;

int leftline_breakpoint;
int rightline_breakpoint;

int break_rows;

int Lost_centerline = 0;                      //中线丢失的行坐标(扫线到赛道外)
int LostNum_leftline = 0, LostNum_rightline = 0; //记录左右边界丢线数

void BT_Binary_lifting_line(unsigned char Image_Use[LCD_H][LCD_W]) {

    int BORDER_BIAS = 3;
    // 清除掉上一帧提取的边线数组数据
    for (int i = 0; i < LCD_H; i++) {
        leftline[i] = 0;
        rightline[i] = 0;
        centerline[i] = 0;
    }
    int row, cloum;   //行,列
    //开始扫线(从下往上,从中间往两边),为了扫线的严谨性,我们做BORDER_BIAS的误差处理，即扫线范围会小于图像大小
    for (row = 0; (row - BORDER_BIAS) < LCD_H; row++) //图像的原点在左下角
    {
        //左边扫线
        for (cloum = old_line; (cloum - BORDER_BIAS) > 0; cloum--) {
            if (Image_Use[row][cloum] == IMG_BLACK &&
                Image_Use[row][cloum - BORDER_BIAS] == IMG_BLACK)  //判断左边界点（BORDER_BIAS防偶然因素）
            {
                leftline[row] = cloum;    //记录左边界点
                break;
            }
        }
        //右边扫线
        for (cloum = old_line; (cloum + BORDER_BIAS) < LCD_W; cloum++) {
            if (Image_Use[row][cloum] == IMG_BLACK &&
                Image_Use[row][cloum + BORDER_BIAS] == IMG_BLACK)  //判断右边界点（BORDER_BIAS防偶然因素）
            {
                rightline[row] = cloum;   //记录右边界点
                break;
            }
        }
        //数据处理
        centerline[row] = (leftline[row] + rightline[row]) / 2;   //记录中线点
        if (leftline[row] == 0)     //左边界丢线
            LostNum_leftline++;  //左丢线数+1
        if (rightline[row] == 0)    //右边界丢线
            LostNum_rightline++; //左丢线数+1

        //防止扫线到赛道外
        if (Image_Use[row][centerline[row]] == IMG_BLACK &&
            Image_Use[row + BORDER_BIAS][centerline[row]] == IMG_BLACK)    //row行的中线是黑，扫到了赛道外
        {
            Lost_centerline = row;    //记录中线点丢失的行坐标
            if (row > 20)              //对前20行不做处理
                break;              //若已经在20行后发现扫描到了赛道外,直接break跳出该图的扫线处理
        }

        /*
         ** 参考的代码中都是有专门的数组单独记录每一行是否有丢线情况 这里先不做这样的记录
         ** 默认丢线情况下leftline[]==0
         ** 不等于0的情况都视为没有丢线
         ** */
        old_line = centerline[row];      //以上一次的中线值为下一次扫线的中间点
    }
}

void SJU_Binary_lifting_line(unsigned char Image_Use[LCD_H][LCD_W]) {


    // 清除掉上一帧提取的边线数组数据
    for (int i = 0; i < LCD_H; i++) {
        leftline[i] = 0;
        rightline[i] = 0;
        centerline[i] = 0;

        leftfindflag[i] = 0;
        rightfindflag[i] = 0;

    }

    for (int j = LCD_H - 1; j > 0; j--) {

        //左边界 连续找到两个黑边，退出循环 ，视为边界点
        for (int i = old_line; i >= 1 && i <= LCD_W - 1; i--) {

            fiv_width[j]++;

            if (Image_Use[j][i - 1] == 0 && Image_Use[j][i] == 0) {
                leftfindflag[j] = 1;
                leftline[j] = (i - 1);                    //左线数组
                break;
            } else leftfindflag[j] = 0;

        }


        //右边界 连续找到两个黑边，退出循环 ，视为边界点
        for (int i = old_line; i >= 0 && i < LCD_W - 2; i++) {

            fiv_width[j]++;

            if (Image_Use[j][i + 1] == 0 && Image_Use[j][i] == 0) {
                rightfindflag[j] = 1;
                rightline[j] = (i + 1);                      //右线数组
                break;
            } else rightfindflag[j] = 0;
        }

        //边线判断(丢线扫线次数判断)

        if (j >= 10 && leftfindflag[j] == 1 && rightfindflag[j] == 1)
            //50行内取到的赛道边界计数
            scaned_rows++;

        if (j >= 50 && leftfindflag[j] == 0 && rightfindflag[j] == 0) {
            //10行以内两边全丢次数
            lost_rows++;
        }

        //20行内断线次数
        if (j <= 20 && (leftline[j] - leftline[j - 1]) >= 20) leftline_breakpoint = j;

        if (j <= 20 && (rightline[j] - rightline[j - 1]) <= -20) rightline_breakpoint = j;

        //取中线
        centerline[j] = (leftline[j] + rightline[j]) / 2;



        //中线处理
        if ((Image_Use[j][centerline[j]]) == 0 && (Image_Use[j - 1][centerline[j]]) == 0) {

            break_rows = j;

            //防止一开始就break;
            if (break_rows <= 40) {
                break;
            }

        }
        old_line = centerline[j];                        //扫线完毕给old_line重新赋值

    }
    old_line = centerline[LCD_H/2];

}

//
///*
//---------------------------------------------------------------
// 【函    数】cal_losttimes
// 【功    能】进一步提取元素特征
// 【参    数】无
// 【返 回 值】无
// 【基本思路】 计算前n行左右线各丢失的线数目
// ----------------------------------------------------------------
//*/
//int lostleft_times; //前N行左线未扫到的次数
//int lostright_times; //前N行右线未扫到的次数
//
//unsigned char l_start;  //起始空白行
//unsigned char r_start;
//
//float curvity_left;
//float curvity_right;
//
//
///**********************************************/
//void cal_losttimes(void) {
//
//    unsigned char flag_of_rightbreak = 0;
//    unsigned char flag_of_leftbreak = 0;
//
//    for (int i = 0; i < lost_rows; i++) {
//        //左线操作
//        if (leftfindflag[LCD_H - 1 - i] == 0) {
//            lostleft_times++;
//            if (flag_of_leftbreak == 0)     //如果在这一行之前没有遭遇断线，则计数
//            {
//                l_start--;
//            }
//        } else   //扫到线
//        {
//            //lostleft_times不继续增加
//            flag_of_leftbreak = 1;  //break标志成立
//        }
//        //右线操作
//        if (rightfindflag[LCD_H - 1 - i] == 0)       //未扫到线
//        {
//            lostright_times++;
//            if (flag_of_rightbreak == 0)     //如果在这一行之前没有遭遇断线，则计数
//            {
//                r_start--;
//            }
//        } else    //扫到线
//        {
//            //lostright_times不继续增加
//            flag_of_rightbreak = 1;  //break标志成立
//        }
//
//    }
//
//}
//
////=======================计算曲率的函数和参数======================
//unsigned char my_sqrt(unsigned char x) {
//    unsigned char ans = 0, p = 0x80;
//    while (p != 0) {
//        ans += p;
//        if (ans * ans > x) {
//            ans -= p;
//        }
//        p = (unsigned char) (p / 2);
//    }
//    return (ans);
//}
//
//float curvity = 0;
//
//
///*
// * curvity_right = process_curvity(rightline[0], 0, rightline[curvity_point1],
// * curvity_point1, rightline[curvity_point2], curvity_point2);
// *
// */
////选三个点计算曲率，曲率越大，速度越慢！
//float process_curvity(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char x3,
//                      unsigned char y3) {
//    float K;
//    int S_of_ABC = ((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2;
//    //面积的符号表示方向
//    unsigned char q1 = (unsigned char) ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
//    unsigned char AB = my_sqrt(q1);
//    q1 = (unsigned char) ((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));
//    unsigned char BC = my_sqrt(q1);
//    q1 = (unsigned char) ((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
//    unsigned char AC = my_sqrt(q1);
//    if (AB * BC * AC == 0) {
//        K = 0;
//    } else
//        K = (float) 4 * S_of_ABC / (AB * BC * AC);
//    return K;
//}
////======================================================================
//
////======================================================================
//int Taylor_log(int x) {
//    int result = 0;
//    if (x == 1) result = 0;
//    else result = (x - 1) - (x - 1) * (x - 1) / 2 + (x - 1) * (x - 1) * (x - 1) / 3;
//    return result;
//}
//
//double double_abs(double x) {
//    if (x < 0)
//        return (double) (-x);
//    else
//        return (double) x;
//}
//
//unsigned char My_Abs(int x) {
//    if (x < 0)
//        return (unsigned char) (-x);
//    else
//        return (unsigned char) x;
//}
//
//int My_Min(int x, int y) {
//    if (x > y)
//        return y;
//    else
//        return x;
//}
//
//unsigned char overflow(int x)      //防止拟合的数据超出unsigned char范围导致出错，限幅
//{
//    unsigned char result = 0;
//    if (x >= 185) x = 185;
//    else if (x <= 0) x = 0;
//    else result = (unsigned char) x;
//    return result;
//}
//
///************最小二乘法拟合中线*************************/
//
//float parameterA;
//float parameterB;
//
//void regression(int type, int startline, int endline) {
//    int i = 0;
//    int sumlines = endline - startline;
//    if (sumlines == 0) sumlines = 1;
//    float sumX = 0;
//    float sumY = 0;
//    float averageX = 0;
//    float averageY = 0;
//    float sumUp = 0;
//    float sumDown = 0;
//    if (type == 0)      //拟合中线
//    {
//        for (i = startline; i < endline; i++) {
//            sumX += i;
//            sumY += centerline[i];
//        }
//        averageX = sumX / sumlines;     //x的平均值
//        averageY = sumY / sumlines;     //y的平均值
//        for (i = startline; i < endline; i++) {
//            sumUp += (centerline[i] - averageY) * (i - averageX);
//            sumDown += (i - averageX) * (i - averageX);
//        }
//        if (sumDown == 0) parameterB = 0;
//        else parameterB = sumUp / sumDown;
//        parameterA = averageY - parameterB * averageX;
//    } else if (type == 1)//拟合左线
//    {
//        for (i = startline; i < endline; i++) {
//            sumX += i;
//            sumY += leftline[i];
//        }
//        averageX = sumX / sumlines;     //x的平均值
//        averageY = sumY / sumlines;     //y的平均值
//        for (i = startline; i < endline; i++) {
//            sumUp += (leftline[i] - averageY) * (i - averageX);
//            sumDown += (i - averageX) * (i - averageX);
//        }
//        if (sumDown == 0) parameterB = 0;
//        else parameterB = sumUp / sumDown;
//        parameterA = averageY - parameterB * averageX;
//    } else if (type == 2)//拟合右线
//    {
//        for (i = startline; i < endline; i++) {
//            sumX += i;
//            sumY += rightline[i];
//        }
//        averageX = sumX / sumlines;     //x的平均值
//        averageY = sumY / sumlines;     //y的平均值
//        for (i = startline; i < endline; i++) {
//            sumUp += (rightline[i] - averageY) * (i - averageX);
//            sumDown += (i - averageX) * (i - averageX);
//        }
//        if (sumDown == 0) parameterB = 0;
//        else parameterB = sumUp / sumDown;
//        parameterA = averageY - parameterB * averageX;
//
//    }
//}
