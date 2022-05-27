#include "headfile.h"
#include "Binarization.h"
#include "camera.h"
#include "Encoder.h"
#include "Motor.h"
#include "Servo.h"
#include "Variable.h"
#include "SJUOpenSource.h"
#include "LQOpenSource.h"
// (ԭ��)
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
 ����    ����BT_Binary_lifting_line
 ����    �ܡ�Ѱ�ҳ���Ԫ��֮���������ͨ����
 ����    ������
 ���� �� ֵ����
 ������˼·�� ��������ɨ�����м�������ɨ����һ�е�����ֵ��¼��������Ϊ��һ��������ɨ����ʼֵ
                       ��Զ������Ϊ��Ч�ߣ���������Բ���ط�ǿ������ܻ���һЩЧ����
 ----------------------------------------------------------------*/
/*************�������߲���*******************/
int old_line = LCD_W / 2;   //��һ��ɨ������
int fiv_width[LCD_W];  //�������


unsigned char leftfindflag[LCD_H];  //��߽��־λ


unsigned char rightfindflag[LCD_H];      //�ұ߽��־λ


int scaned_rows;
int lost_rows = 0;

int leftline_breakpoint;
int rightline_breakpoint;

int break_rows;

int Lost_centerline = 0;                      //���߶�ʧ��������(ɨ�ߵ�������)
int LostNum_leftline = 0, LostNum_rightline = 0; //��¼���ұ߽綪����

void BT_Binary_lifting_line(unsigned char Image_Use[LCD_H][LCD_W]) {

    int BORDER_BIAS = 3;
    // �������һ֡��ȡ�ı�����������
    for (int i = 0; i < LCD_H; i++) {
        leftline[i] = 0;
        rightline[i] = 0;
        centerline[i] = 0;
    }
    int row, cloum;   //��,��
    //��ʼɨ��(��������,���м�������),Ϊ��ɨ�ߵ��Ͻ���,������BORDER_BIAS��������ɨ�߷�Χ��С��ͼ���С
    for (row = 0; (row - BORDER_BIAS) < LCD_H; row++) //ͼ���ԭ�������½�
    {
        //���ɨ��
        for (cloum = old_line; (cloum - BORDER_BIAS) > 0; cloum--) {
            if (Image_Use[row][cloum] == IMG_BLACK &&
                Image_Use[row][cloum - BORDER_BIAS] == IMG_BLACK)  //�ж���߽�㣨BORDER_BIAS��żȻ���أ�
            {
                leftline[row] = cloum;    //��¼��߽��
                break;
            }
        }
        //�ұ�ɨ��
        for (cloum = old_line; (cloum + BORDER_BIAS) < LCD_W; cloum++) {
            if (Image_Use[row][cloum] == IMG_BLACK &&
                Image_Use[row][cloum + BORDER_BIAS] == IMG_BLACK)  //�ж��ұ߽�㣨BORDER_BIAS��żȻ���أ�
            {
                rightline[row] = cloum;   //��¼�ұ߽��
                break;
            }
        }
        //���ݴ���
        centerline[row] = (leftline[row] + rightline[row]) / 2;   //��¼���ߵ�
        if (leftline[row] == 0)     //��߽綪��
            LostNum_leftline++;  //������+1
        if (rightline[row] == 0)    //�ұ߽綪��
            LostNum_rightline++; //������+1

        //��ֹɨ�ߵ�������
        if (Image_Use[row][centerline[row]] == IMG_BLACK &&
            Image_Use[row + BORDER_BIAS][centerline[row]] == IMG_BLACK)    //row�е������Ǻڣ�ɨ����������
        {
            Lost_centerline = row;    //��¼���ߵ㶪ʧ��������
            if (row > 20)              //��ǰ20�в�������
                break;              //���Ѿ���20�к���ɨ�赽��������,ֱ��break������ͼ��ɨ�ߴ���
        }

        /*
         ** �ο��Ĵ����ж�����ר�ŵ����鵥����¼ÿһ���Ƿ��ж������ �����Ȳ��������ļ�¼
         ** Ĭ�϶��������leftline[]==0
         ** ������0���������Ϊû�ж���
         ** */
        old_line = centerline[row];      //����һ�ε�����ֵΪ��һ��ɨ�ߵ��м��
    }
}

void SJU_Binary_lifting_line(unsigned char Image_Use[LCD_H][LCD_W]) {


    // �������һ֡��ȡ�ı�����������
    for (int i = 0; i < LCD_H; i++) {
        leftline[i] = 0;
        rightline[i] = 0;
        centerline[i] = 0;

        leftfindflag[i] = 0;
        rightfindflag[i] = 0;

    }

    for (int j = LCD_H - 1; j > 0; j--) {

        //��߽� �����ҵ������ڱߣ��˳�ѭ�� ����Ϊ�߽��
        for (int i = old_line; i >= 1 && i <= LCD_W - 1; i--) {

            fiv_width[j]++;

            if (Image_Use[j][i - 1] == 0 && Image_Use[j][i] == 0) {
                leftfindflag[j] = 1;
                leftline[j] = (i - 1);                    //��������
                break;
            } else leftfindflag[j] = 0;

        }


        //�ұ߽� �����ҵ������ڱߣ��˳�ѭ�� ����Ϊ�߽��
        for (int i = old_line; i >= 0 && i < LCD_W - 2; i++) {

            fiv_width[j]++;

            if (Image_Use[j][i + 1] == 0 && Image_Use[j][i] == 0) {
                rightfindflag[j] = 1;
                rightline[j] = (i + 1);                      //��������
                break;
            } else rightfindflag[j] = 0;
        }

        //�����ж�(����ɨ�ߴ����ж�)

        if (j >= 10 && leftfindflag[j] == 1 && rightfindflag[j] == 1)
            //50����ȡ���������߽����
            scaned_rows++;

        if (j >= 50 && leftfindflag[j] == 0 && rightfindflag[j] == 0) {
            //10����������ȫ������
            lost_rows++;
        }

        //20���ڶ��ߴ���
        if (j <= 20 && (leftline[j] - leftline[j - 1]) >= 20) leftline_breakpoint = j;

        if (j <= 20 && (rightline[j] - rightline[j - 1]) <= -20) rightline_breakpoint = j;

        //ȡ����
        centerline[j] = (leftline[j] + rightline[j]) / 2;



        //���ߴ���
        if ((Image_Use[j][centerline[j]]) == 0 && (Image_Use[j - 1][centerline[j]]) == 0) {

            break_rows = j;

            //��ֹһ��ʼ��break;
            if (break_rows <= 40) {
                break;
            }

        }
        old_line = centerline[j];                        //ɨ����ϸ�old_line���¸�ֵ

    }
    old_line = centerline[LCD_H/2];

}

//
///*
//---------------------------------------------------------------
// ����    ����cal_losttimes
// ����    �ܡ���һ����ȡԪ������
// ����    ������
// ���� �� ֵ����
// ������˼·�� ����ǰn�������߸���ʧ������Ŀ
// ----------------------------------------------------------------
//*/
//int lostleft_times; //ǰN������δɨ���Ĵ���
//int lostright_times; //ǰN������δɨ���Ĵ���
//
//unsigned char l_start;  //��ʼ�հ���
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
//        //���߲���
//        if (leftfindflag[LCD_H - 1 - i] == 0) {
//            lostleft_times++;
//            if (flag_of_leftbreak == 0)     //�������һ��֮ǰû���������ߣ������
//            {
//                l_start--;
//            }
//        } else   //ɨ����
//        {
//            //lostleft_times����������
//            flag_of_leftbreak = 1;  //break��־����
//        }
//        //���߲���
//        if (rightfindflag[LCD_H - 1 - i] == 0)       //δɨ����
//        {
//            lostright_times++;
//            if (flag_of_rightbreak == 0)     //�������һ��֮ǰû���������ߣ������
//            {
//                r_start--;
//            }
//        } else    //ɨ����
//        {
//            //lostright_times����������
//            flag_of_rightbreak = 1;  //break��־����
//        }
//
//    }
//
//}
//
////=======================�������ʵĺ����Ͳ���======================
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
////ѡ������������ʣ�����Խ���ٶ�Խ����
//float process_curvity(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char x3,
//                      unsigned char y3) {
//    float K;
//    int S_of_ABC = ((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2;
//    //����ķ��ű�ʾ����
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
//unsigned char overflow(int x)      //��ֹ��ϵ����ݳ���unsigned char��Χ���³����޷�
//{
//    unsigned char result = 0;
//    if (x >= 185) x = 185;
//    else if (x <= 0) x = 0;
//    else result = (unsigned char) x;
//    return result;
//}
//
///************��С���˷��������*************************/
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
//    if (type == 0)      //�������
//    {
//        for (i = startline; i < endline; i++) {
//            sumX += i;
//            sumY += centerline[i];
//        }
//        averageX = sumX / sumlines;     //x��ƽ��ֵ
//        averageY = sumY / sumlines;     //y��ƽ��ֵ
//        for (i = startline; i < endline; i++) {
//            sumUp += (centerline[i] - averageY) * (i - averageX);
//            sumDown += (i - averageX) * (i - averageX);
//        }
//        if (sumDown == 0) parameterB = 0;
//        else parameterB = sumUp / sumDown;
//        parameterA = averageY - parameterB * averageX;
//    } else if (type == 1)//�������
//    {
//        for (i = startline; i < endline; i++) {
//            sumX += i;
//            sumY += leftline[i];
//        }
//        averageX = sumX / sumlines;     //x��ƽ��ֵ
//        averageY = sumY / sumlines;     //y��ƽ��ֵ
//        for (i = startline; i < endline; i++) {
//            sumUp += (leftline[i] - averageY) * (i - averageX);
//            sumDown += (i - averageX) * (i - averageX);
//        }
//        if (sumDown == 0) parameterB = 0;
//        else parameterB = sumUp / sumDown;
//        parameterA = averageY - parameterB * averageX;
//    } else if (type == 2)//�������
//    {
//        for (i = startline; i < endline; i++) {
//            sumX += i;
//            sumY += rightline[i];
//        }
//        averageX = sumX / sumlines;     //x��ƽ��ֵ
//        averageY = sumY / sumlines;     //y��ƽ��ֵ
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
