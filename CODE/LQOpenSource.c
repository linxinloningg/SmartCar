#include "headfile.h"
#include "Binarization.h"
#include "camera.h"
#include "Encoder.h"
#include "Motor.h"
#include "Servo.h"
#include "Variable.h"
#include "SJUOpenSource.h"
#include "LQOpenSource.h"



//*****************************************ͼ����*************************************************************************
/*!
  * @brief    ��ȡ����
  *
  * @param    Image_Use �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  *
  * @return   �Ƿ���
  *
  * @note     ˼·���Ӿ��복ͷ�Ͻ����п�ʼ���м�����������
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */


/**  @brief    ʹ����ʼ��  */
#define ROAD_START_ROW     115
/**  @brief    ʹ�ý�����  */
#define ROAD_END_ROW       10

/*!
  * @brief    �ж��Ƿ���
  *
  * @param    Image_Use �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    lineIndex  �� ��
  *
  * @return   0��û�ж���   1:��߶���  2���ұ߶���  3�� ���Ҷ�����   4������
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
unsigned char RoadIsNoSide(unsigned char Image_Use[LCD_H][LCD_W], unsigned char imageOut[LCD_H][2],
                           unsigned char lineIndex) {
    unsigned char state = 0;
    unsigned char i = 0;
    static unsigned char last = 78;

    imageOut[lineIndex][0] = 0;
    imageOut[lineIndex][1] = 159;
    /* �þ���С���ȽϽ����� �ж��Ƿ��� */
    for (i = last; i > 1; i--) {
        if (Image_Use[lineIndex][i]) {
            imageOut[lineIndex][0] = i;
            break;
        }
    }

    if (i == 1) {
        /* ��߽綪�� */
        state = 1;
    }

    for (i = last; i < 159; i++) {
        if (Image_Use[lineIndex][i]) {
            imageOut[lineIndex][1] = i;
            break;
        }
    }

    if (i == 159) {
        /* ���ұ߽綪�� */
        if (state == 1) {
            state = 3;
        }

            /* �ұ߽綪�� */
        else {
            state = 2;
        }

    }
    if (imageOut[lineIndex][1] <= imageOut[lineIndex][0]) {
        state = 4;
    }
    return state;

}

/*!
  * @brief    ���ߴ���
  *
  * @param    Image_Use �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    mode       �� �Ǳ߶��ߣ�   1����߶���  2���ұ߶���
  * @param    lineIndex  �� ��������
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
void RoadNoSideProcess(unsigned char Image_Use[LCD_H][LCD_W], unsigned char imageOut[LCD_H][2],
                       unsigned char mode, unsigned char lineIndex) {
    unsigned char i = 0, j = 0, count = 0;

    switch (mode) {
        case 1:
            for (i = imageOut[lineIndex][1]; i > 1; i--) {
                count++;
                for (j = lineIndex; j > ROAD_END_ROW && lineIndex > count; j--) {
                    if (Image_Use[j][i]) {
                        imageOut[lineIndex - count][0] = 0;
                        imageOut[lineIndex - count][1] = i;
                        break;
                    }

                }
            }
            break;

        case 2:
            for (i = imageOut[lineIndex][0]; i < 159; i++) {
                count++;
                for (j = lineIndex; j > ROAD_END_ROW && lineIndex > count; j--) {
                    if (Image_Use[j][i]) {
                        imageOut[lineIndex - count][0] = i;
                        imageOut[lineIndex - count][1] = 159;
                        break;
                    }

                }
            }
            break;

    }

}

unsigned char ImageGetSide(unsigned char Image_Use[LCD_H][LCD_W], unsigned char imageOut[LCD_H][2]) {
    unsigned char i = 0, j = 0;

    RoadIsNoSide(Image_Use, imageOut, ROAD_START_ROW);

    /* �복ͷ����40�� Ѱ�ұ��� */
    for (i = ROAD_START_ROW - 1; i > ROAD_END_ROW; i--) {
        imageOut[i][0] = 0;
        imageOut[i][1] = 159;

        /* ���ݱ߽��������� Ѱ�ұ߽� */
        for (j = imageOut[i + 1][0] + 10; j > 0; j--) {
            if (!Image_Use[i][j]) {
                imageOut[i][0] = j;
                break;
            }
        }
        for (j = imageOut[i + 1][1] - 10; j < 160; j++) {
            if (!Image_Use[i][j]) {
                imageOut[i][1] = j;
                break;
            }
        }
        /* �����߽� ������������ �����Ƿ��Ҷ��� */
        if (imageOut[i][0] > (LCD_W / 2 - 10) && imageOut[i][1] > (LCD_W - 5)) {
            /* �Ҷ��ߴ��� */
            RoadNoSideProcess(Image_Use, imageOut, 2, i);

            if (i > 70) {
                imageOut[i][0] += 50;
            }
            return 1;
        }

        /* ����ұ߽� ������������ �����Ƿ����� */
        if (imageOut[i][1] < (LCD_W / 2 + 10) && imageOut[i][0] < (5)) {
            /* ���ߴ��� */
            RoadNoSideProcess(Image_Use, imageOut, 1, i);

            if (i > 70) {
                imageOut[i][1] -= 50;
            }
            return 2;

        }
    }
    return 0;
}

/*!
  * @brief    ��ȡ����
  *
  * @param    Image_Use �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  *
  * @return   �Ƿ���
  *
  * @note     ˼·���Ӿ��복ͷ�Ͻ����п�ʼ���м�����������
  *
  * @see
  *
  * @date     2021/11/30 ���ڶ�
  */
unsigned char UpdownSideGet(unsigned char Image_Use[LCD_H][LCD_W], unsigned char imageOut[2][LCD_W]) {
    unsigned char i = 0, j = 0;
    unsigned char last = 60;

    imageOut[0][159] = 0;
    imageOut[1][159] = 119;
    /* �����߱ȽϽ����� �ж��Ƿ��� */
    for (i = last; i >= 0; i--) {
        if (!Image_Use[i][80]) {
            imageOut[0][80] = i;
            break;
        }
    }

    for (i = last; i < 120; i++) {
        if (!Image_Use[i][80]) {
            imageOut[1][80] = i;
            break;
        }
    }

    /* �������� Ѱ�ұ��� */
    for (i = 80 - 1; i > 0; i--) {
        imageOut[0][i] = 0;
        imageOut[1][i] = 119;

        /* ���ݱ߽��������� Ѱ�ұ߽� */
        for (j = imageOut[0][i + 1] + 10; j > 0; j--) {
            if (!Image_Use[j][i]) {
                imageOut[0][i] = j;
                break;
            }
        }
        for (j = imageOut[1][i + 1] - 10; j < 120; j++) {
            if (!Image_Use[j][i]) {
                imageOut[1][i] = j;
                break;
            }
        }
    }
    /*�������� Ѱ�ұ���*/
    for (i = 80 + 1; i < 159; i++) {
        imageOut[0][i] = 0;
        imageOut[1][i] = 119;

        /* ���ݱ߽��������� Ѱ�ұ߽� */
        for (j = imageOut[0][i - 1] + 10; j > 0; j--) {
            if (!Image_Use[j][i]) {
                imageOut[0][i] = j;
                break;
            }
        }
        for (j = imageOut[1][i - 1] - 10; j < 120; j++) {
            if (!Image_Use[j][i]) {
                imageOut[1][i] = j;
                break;
            }
        }
    }
    return 0;
}

//*****************************************======*************************************************************************


//****************************************Ԫ��ʶ��***************************************************************************

/****************************��������************************************/
/*!
  * @brief    �жϱ����Ƿ���ڻ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    status     �� 1�������  2���ұ���
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */
unsigned char
RoundaboutGetArc(unsigned char imageSide[LCD_H][2], unsigned char status, unsigned char num, unsigned char *index) {
    int i = 0;
    unsigned char inc = 0, dec = 0, n = 0;
    switch (status) {
        case 1:
            for (i = ROAD_START_ROW - 1; i > ROAD_END_ROW; i--) {
                if (imageSide[i][0] != 0 && imageSide[i + 1][0] != 0) {
                    if (imageSide[i][0] == imageSide[i + 1][0]) {
                        n++;
                        continue;
                    }
                    if (imageSide[i][0] > imageSide[i + 1][0]) {
                        inc++;
                        inc += n;
                        n = 0;
                    } else {
                        dec++;
                        dec += n;
                        n = 0;
                    }

                    /* �л��� */
                    if (inc > num && dec > num) {
                        *index = i + num;
                        return 1;
                    }
                } else {
                    inc = 0;
                    dec = 0;
                    n = 0;
                }
            }

            break;

        case 2:
            for (i = ROAD_START_ROW - 1; i > ROAD_END_ROW; i--) {
                if (imageSide[i][1] != 159 && imageSide[i + 1][1] != 159) {
                    if (imageSide[i][1] == imageSide[i + 1][1]) {
                        n++;
                        continue;
                    }
                    if (imageSide[i][1] > imageSide[i + 1][1]) {
                        inc++;
                        inc += n;
                        n = 0;
                    } else {
                        dec++;
                        dec += n;
                        n = 0;
                    }

                    /* �л��� */
                    if (inc > num && dec > num) {
                        *index = i + num;
                        return 1;
                    }
                } else {
                    inc = 0;
                    dec = 0;
                    n = 0;
                }
            }

            break;
    }

    return 0;
}

/*!
  * @brief    �жϱ����Ƿ���ڻ���
  *
  * @param    SideInput �� �ϱ�������
  * @param    num       �� ������
  * @param    index     �� ��͵�
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2021/12/01 ������
  */
unsigned char
UpSideErr(unsigned char SideInput[2][LCD_W], unsigned char status, unsigned char num, unsigned char *index) {
    unsigned char dec = 0, inc = 0, i;
    //�����Ƿ���ͻ��
    switch (status) {
        case 1:
            for (i = 159 - 1; i > 0; i--) {
                if (UpdowmSide[0][i] > 1 && UpdowmSide[0][i + 1] > 1) {
                    if (UpdowmSide[0][i] >= UpdowmSide[0][i + 1])
                        inc++;
                    else
                        dec++;
                    /* �л��� */
                    if (inc > num && dec > num) {
                        *index = i + num;
                        return 1;
                    }
                } else {
                    inc = 0;
                    dec = 0;
                }
            }
            break;
            //�±���
        case 2:
            for (i = 159 - 1; i > 0; i--) {
                if (UpdowmSide[1][i] != 1 && UpdowmSide[1][i + 1] != 1) {
                    if (UpdowmSide[1][i] >= UpdowmSide[1][i + 1])
                        inc++;
                    else
                        dec++;
                    /* �л��� */
                    if (inc > num && dec > num) {
                        *index = i + num;
                        return 1;
                    }
                } else {
                    inc = 0;
                    dec = 0;
                }
            }
            break;
    }

    return 0;
}

/*!
  * @brief    �ж��ϱ����Ƿ񵥵�
  * @param    X1 :��ʼX��
  * @param    X2 :��ֹX��              X1 < X2
  * @param    imageIn �� ��������
  *
  * @return   0��������or���� 1������������ 2�������ݼ�
  *
  * @note
  *
  * @see
  *
  * @date     2021/11/30 ���ڶ�
  */
unsigned char RoadUpSide_Mono(unsigned char X1, unsigned char X2, unsigned char imageIn[2][LCD_W]) {
    unsigned char i = 0, num = 0;

    for (i = X1; i < X2 - 1; i++) {
        if (imageIn[0][i] >= imageIn[0][i + 1])
            num++;
        else
            num = 0;
        if (num >= (X2 - X1) * 4 / 5)
            return 1;
    }

    for (i = X1; i < X2 - 1; i++) {
        if (imageIn[0][i] <= imageIn[0][i + 1])
            num++;
        else
            num = 0;
        if (num >= (X2 - X1) * 4 / 5)
            return 2;
    }
    return 0;
}

/*!
  * @brief    ��⻷��
  *
  *
  * @see
  *
  * @date     2020/6/24 ������
  */

unsigned char RoadIsRoundabout(unsigned char Upimage[2][LCD_W], unsigned char imageInput[LCD_H][LCD_W],
                               unsigned char image[LCD_H][2], unsigned char *flag) {
    unsigned char i = 0;
    unsigned char errL = 0, errR = 0;
    unsigned char leftState = 0, rightState = 0;
    unsigned char count = 0;
    unsigned char num = 0, py;

    if (RoadUpSide_Mono(5, 120, Upimage))
        return 0;
    /* �ӳ�ͷ��ǰ ������Ƿ񵥵� */
    for (i = ROAD_START_ROW - 1; i > ROAD_END_ROW; i--) {
        if (image[i][0] == 0)
            continue;
        if (image[i][0] >= image[i + 1][0])    // i��Y����ֵ  0 ��ͼ������X����
        {
            num++;
            if (num == 50) {
                num = 0;
                leftState = 1;   // �󵥵���־
                break;
            }
        } else {
            num = 0;
        }
        if (i == ROAD_END_ROW + 1)  // Y�ӵ�11  ��0
            num = 0;
    }
    errL = RoundaboutGetArc(image, 1, 5, &py);
    errR = RoundaboutGetArc(image, 1, 5, &py);

    /* �ұ����Ƿ񵥵� */
    for (i = ROAD_START_ROW - 1; i > ROAD_END_ROW; i--) {
        if (image[i][1] + 3 < image[i + 1][1]) {
            num++;
            if (num == 50) {
                num = 0;
                rightState = 1;
                break;
            }
        }
        if (i == ROAD_END_ROW + 1)
            num = 0;
    }

    /* ��ߵ����� ����Ҳ��Ƿ��ǻ��� */
    if (leftState == 1 && rightState == 0 && errL == 0) {
        count = 0;

        if (RoundaboutGetArc(image, 2, 5, &count))//��Բ����� (5�������� �� 5��������)
        {
            *flag = 1;
            return 1;
        } else {
            return 0;
        }
    }

    /* �ұߵ����� �������Ƿ��ǻ��� */
    if (rightState == 1 && leftState == 0) {
        count = 0;
        if (RoundaboutGetArc(image, 1, 5, &count))//��Բ����� (5�������� �� 5��������)
        {
            *flag = 2;
            return 2;
        }
    }
    return 0;
}

/*!
  * @brief    ��ȡ��������
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    status     �� 1���󻷵�(����)  2���һ���(����)
  *
  * @return
  *
  * @note     ˼·������һ�߱����ϸ񵥵�������һ�߱��ߣ���ȡ��һ����
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */
void
RoundaboutGetSide(unsigned char imageInput[LCD_H][LCD_W], unsigned char imageSide[LCD_H][2], unsigned char status) {
    unsigned char i = 0, j = 0;

    switch (status) {

        /* �󻷵� */
        case 1: {
            /* ����ȷ����߽� */
            for (i = ROAD_START_ROW; i > ROAD_END_ROW; i--) {
                for (j = LCD_W / 2; j > 0; j--) {
                    if (!imageInput[i][j]) {
                        imageSide[i][0] = j;
                        break;
                    }
                }
            }
            break;
        }

        case 2: {
            /* ����ȷ���ұ߽� */
            for (i = ROAD_START_ROW; i > ROAD_END_ROW; i--) {
                for (j = LCD_W / 2; j < LCD_W; j++) {
                    if (!imageInput[i][j]) {
                        imageSide[i][1] = j;
                        break;
                    }
                }
            }
            break;
        }
    }
}

/*!
  * @brief    ���ߴ���
  *
  * @param    imageSide  : ����
  * @param    status     : 1������߲���   2���ұ��߲���
  * @param    startX     : ��ʼ�� ����
  * @param    startY     : ��ʼ�� ����
  * @param    endX       : ������ ����
  * @param    endY       : ������ ����
  *
  * @return
  *
  * @note     endY һ��Ҫ���� startY
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
void
ImageAddingLine(unsigned char imageSide[LCD_H][2], unsigned char status, unsigned char startX, unsigned char startY,
                unsigned char endX, unsigned char endY) {
    int i = 0;

    /* ֱ�� x = ky + b*/
    float k = 0.0f, b = 0.0f;
    switch (status) {
        case 1://����
        {
            k = (float) ((float) endX - (float) startX) / (float) ((float) endY - (float) startY);
            b = (float) startX - (float) startY * k;

            for (i = startY; i < endY; i++) {
                imageSide[i][0] = (unsigned char) (k * i + b);
            }
            break;
        }

        case 2://�Ҳ���
        {
            k = (float) ((float) endX - (float) startX) / (float) ((float) endY - (float) startY);
            b = (float) startX - (float) startY * k;

            for (i = startY; i < endY; i++) {
                imageSide[i][1] = (unsigned char) (k * i + b);
            }
            break;
        }

    }
}

void Roundabout_Get_UpDowmSide(unsigned char imageInput[LCD_H][LCD_W], unsigned char imageSide[2][LCD_W],
                               unsigned char status) {
    unsigned char i = 0, j = 0;

    switch (status) {
        case 1: {
            /* ����ȷ���ϱ߽� */
            for (i = 159; i > 0; i--) {
                for (j = LCD_H / 2; j > 0; j--) {
                    if (!imageInput[j][i]) {
                        imageSide[0][i] = j;
                        break;
                    }
                }
            }
            break;
        }

        case 2: {
            /* ����ȷ���±߽� */
            for (i = 159; i > 0; i--) {
                for (j = LCD_H / 2; j < LCD_H; j++) {
                    if (!imageInput[j][i]) {
                        imageSide[1][i] = j;
                        break;
                    }
                }
            }
            break;
        }
    }
}

/*!
  * @brief    �������ߴ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageSide  �� ��������
  * @param    status     ��������־λ   ����Ϊ�һ�����ż��Ϊ�󻷵���0Ϊ�������󻷵�û���޸ģ�
  *
  * @return
  *
  * @note     ����ֻд���󻷵����һ�����ҿ��Բο��󻷵��Լ�����
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
void RoundaboutProcess(unsigned char imageInput[LCD_H][LCD_W], unsigned char imageSide[LCD_H][2],
                       unsigned char UpdowmSide[2][LCD_W], unsigned char *state) {
    unsigned char i = 0, err5 = 0;
    unsigned char pointX = 0, pointY = 0, inc = 0, dec = 0;
    unsigned char flag = 0, Down_flag = 0;
    static unsigned char finderr = 0, Up_flag = 0, err1 = 0;
    switch (*state) {
        /* �����һ��� �������ڴ����� */
        case 1:

            /* ����ȷ���ұ߽� */
            RoundaboutGetSide(imageInput, imageSide, 2);

            /* ��黡�� */
            err1 = RoundaboutGetArc(imageSide, 2, 5, &pointY);

            /* �л��� ���в��� ���ӻ������ҵ� �� ͼ�����½� */
            if (err1) {
                pointX = imageSide[pointY][1];
//            UART_PutStr(UART0, "err\r\n");
//
//            /* ׼���뻷�� */
//            if((pointY + 10) > ROAD_MAIN_ROW)
//            {
//                * state = 3;
//            }
                //����
                ImageAddingLine(imageSide, 2, pointX, pointY, 159, ROAD_START_ROW);
                finderr = 1;
            } else {
                if (finderr)
                    *state = 3;//׼�����뻷��
            }

            break;

            /* �����󻷵� �������ڴ����� */
        case 2:

            /* ����ȷ����߽� */
            RoundaboutGetSide(imageInput, imageSide, 1);

            /* ��黡�� */
            err1 = RoundaboutGetArc(imageSide, 1, 5, &pointY);

            /* �л��� ���в��� ���ӻ������ҵ� �� ͼ�����½� */
            if (err1) {
                pointX = imageSide[pointY][0];
                ImageAddingLine(imageSide, 1, 160 - pointX, 160 - pointY, 0, ROAD_START_ROW);
                finderr = 1;
            } else {
                if (finderr)
                    *state = 4;
            }
            break;


            /* ׼�����뻷���� ���� */
        case 3:
            /* ����ȷ���ϱ߽� */
            Roundabout_Get_UpDowmSide(imageInput, UpdowmSide, 1);
            pointY = 0;
            pointX = 0;

            /* �ϱ߽���͵� */
            for (i = 40; i < 100; i++) {
                if (UpdowmSide[0][i] > pointY) {
                    pointX = i;
                    pointY = UpdowmSide[0][i];
                }
            }
            if (pointY >= 50)//��͵����50�������Լ�ʵ������޸ģ�
            {
                if (RoadUpSide_Mono(5, 100, UpdowmSide) == 1)//���ߵ�����������һ��
                    *state = 5;
                ImageAddingLine(imageSide, 1, 100 + 30, 40 - 10, 0, ROAD_START_ROW);//���ߣ������޸ģ�
            } else
                ImageAddingLine(imageSide, 1, 60, 40 - 15, 0, ROAD_START_ROW); //���ߣ����߽Ƕ������޸ģ�
            break;

        case 4:
            /* ����ȷ���ϱ߽� */
            Roundabout_Get_UpDowmSide(imageInput, UpdowmSide, 1);
            pointY = 0;
            pointX = 0;

            /* �ϱ߽���͵� */
            for (i = 40; i < 100; i++) {
                if (UpdowmSide[0][i] > pointY) {
                    pointX = i;
                    pointY = UpdowmSide[0][i];
                }
            }
            if (pointY >= 50) {
                if (RoadUpSide_Mono(5, 100, UpdowmSide) == 1)
                    *state = 6;
                ImageAddingLine(imageSide, 2, 10, 40 - 10, 159, ROAD_START_ROW);
            } else
                ImageAddingLine(imageSide, 2, 100, 40 - 15, 159, ROAD_START_ROW);
            break;
            /* �������� ֱ�������� */
        case 5:
            flag = 0;
            /* ��黡�� */
            for (i = ROAD_START_ROW - 1; i > ROAD_END_ROW; i--) {
                if (imageSide[i][0] != 0 && imageSide[i + 1][0] != 0) {
                    if (imageSide[i][0] >= imageSide[i + 1][0])
                        inc++;
                    else
                        dec++;
                    /* �л��� */
                    if (inc > 10 && dec > 10)err5 = 1;//������10�����ķ��ȣ��������޸ģ�
                } else {
                    inc = 0;
                    dec = 0;
                }
            }

            //����Ϊ119
            for (i = 159; i > 0; i--) {
                if (UpdowmSide[1][i] == 119)
                    inc++;
                else
                    dec++;
                if (dec <= 15) {
                    Down_flag = 1;
                    break;
                }

            }

            //������ߵ�����
            flag = RoadUpSide_Mono(20, 155, UpdowmSide);

            if (flag && err5 && Down_flag) {
                *state = 7;
            }
            break;

            /* �������� ֱ�������� */
        case 6:
            flag = 0;
            /* ��黡�� */
            for (i = ROAD_START_ROW - 1; i > ROAD_END_ROW; i--) {
                if (imageSide[i][1] != 159 && imageSide[i + 1][1] != 159) {
                    if (imageSide[i][1] > imageSide[i + 1][1])
                        inc++;
                    else
                        dec++;
                    /* �л��� */
                    if (inc > 8 && dec > 8)err5 = 1;
                } else {
                    inc = 0;
                    dec = 0;
                }
            }

            //����Ϊ119
            for (i = 159; i > 0; i--) {
                if (UpdowmSide[1][i] == 119)
                    inc++;
                else
                    dec++;
                if (dec <= 15) {
                    Down_flag = 1;
                    break;
                }

            }

            //������ߵ�����
            flag = RoadUpSide_Mono(20, 155, UpdowmSide);

            if (flag && err5 && Down_flag) {
                *state = 8;
//                  ImageAddingLine(imageSide, 1, 145, 30,0, ROAD_START_ROW);
            }
            break;
            //����
        case 7:

            ImageAddingLine(imageSide, 1, 100, 30, 0, ROAD_START_ROW);//���������޸�

            //�ж������Ƿ���ͻ��
            for (i = 159 - 1; i > 0; i--) {
                if (UpdowmSide[0][i] != 0 && UpdowmSide[0][i + 1] != 0) {
                    if (UpdowmSide[0][i] >= UpdowmSide[0][i + 1])
                        inc++;
                    else
                        dec++;
                    if (inc > 20 && dec > 20) {
                        finderr = 0;
                        Up_flag = 0;
                        err1 = 0; //��վ�̬�����Ա��´�ʹ��
//                        Target_Speed1 = 25;               //�ٶȻظ�
//                        Target_Speed2 = 25;
//                        Servo_P = 18;                     //ת��ظ�
                        *state = 0;                         //��������
                        break;
                    }
                } else {
                    inc = 0;
                    dec = 0;
                }

            }
            break;

        case 8:
            Servo_P = 20;
            ImageAddingLine(imageSide, 1, 30, 30, 159, ROAD_START_ROW);
//          Up_flag = RoadUpSide_Mono(20, 155,UpdowmSide);
//          if(flag == 1)
//          {
            for (i = 159 - 1; i > 0; i--) {
                if (UpdowmSide[0][i] != 0 && UpdowmSide[0][i + 1] != 0) {
                    if (UpdowmSide[0][i] >= UpdowmSide[0][i + 1])
                        inc++;
                    else
                        dec++;
                    if (inc > 20 && dec > 20) {
                        finderr = 0;
                        Up_flag = 0;
                        err1 = 0; //��վ�̬�����Ա��´�ʹ��
                        Servo_P = 15;                      //ת��ظ�
                        *state = 0;
                        break;
                    }
                } else {
                    inc = 0;
                    dec = 0;
                }

            }
            break;
//          }
    }
}




/****************************T�ֲ���************************************/
/*!
  * @brief    �ж��Ƿ���T��
  *
  * @param    imageSide �� ͼ�������Ϣ
  * @param    flag      �� T��״̬��Ϣ
  *
  * @return   0�����ǣ� 1����
  *
  * @note     ˼·������0-80�������� 80-159������ ��������һ���󻡣��ұ���ȫ���������115-50������
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */
unsigned char RoadIsT(unsigned char imageUp[2][LCD_W], unsigned char imageSide[LCD_H][2], unsigned char *flag) {
    unsigned char i = 0;
    unsigned char errU1 = 0, errU2 = 0, errR1 = 0, errL1 = 0;
    unsigned char leftState = 0, rightState = 0;
    unsigned char count = 0, num = 0, py;
    unsigned char index = 0;

    /* ��������߾��복ͷ�����ж��� --  */
    for (i = ROAD_START_ROW - 1; i > ROAD_END_ROW; i--) {
        if (imageSide[i][1] == 159)
            num++;
        if (num >= 130) {
            rightState = 1;//��Ϊ����
            break;
        }
    }

    for (i = ROAD_START_ROW - 1; i > 20; i--) {
        if (imageSide[i][0] <= imageSide[i + 1][0]) {
            if (index < 4)index = 0;
            count++;
        } else {
            if (count < 15)count = 0;
            index++;
        }
        if (count >= 15 && index >= 4) {
            leftState = 1;   // ��hu��־
            break;
        }
    }
    errL1 = RoundaboutGetArc(imageSide, 1, 5, &py);    //�����л�
    errR1 = RoundaboutGetArc(imageSide, 2, 5, &py);    //�����л�
    errU1 = RoadUpSide_Mono(10, 70, imageUp);       //�ϵ�����
    errU2 = RoadUpSide_Mono(80, 150, imageUp);     //�ϵ�����
    if (rightState == 1 && errU2 == 2 && errL1 == 1) {
        *flag = 1;
        return 1;
    }
    return 0;

}

/*!
  * @brief    �ж��Ƿ���ʮ��
  *
  * @param    imageSide �� ͼ�������Ϣ
  * @param    flag      �� ʮ��״̬��Ϣ
  *
  * @return   0�����ǣ� 1����
  *
  * @note     ˼·���������߾��복ͷ�����ж��� -- Ȼ��һ�����в�����  --  �����ֶ��� ��֤����ʮ��
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */
unsigned char RoadIsCross(unsigned char imageSide[LCD_H][2], unsigned char *flag) {
    int i = 0;
    unsigned char errR = 0, errF = 0;
    unsigned char rightState = 0, leftState = 0;
    int start[5] = {0, 0, 0, 0, 0}, end[5] = {0, 0, 0, 0, 0};
    unsigned char count = 0;
    unsigned char index = 0;

    /* ����Ҳ���߾��복ͷ�����ж��� -- Ȼ��һ�����в�����  --  �����ֶ��� */
    for (i = ROAD_START_ROW - 1; i > ROAD_END_ROW; i--) {
        if (imageSide[i][1] == 159)
            count++;
        else {
            if (count > 10 && index < 5) {
                start[index] = i + count;
                end[index] = i;
                index++;
            }
            count = 0;
        }
    }
    if (index > 1) {
        if (end[0] - start[1] > 10)
            rightState = 1;
    }
    index = 0;

    /* ��������߾��복ͷ�����ж��� -- Ȼ��һ�����в�����  --  �����ֶ��� */
    for (i = ROAD_START_ROW - 1; i > ROAD_END_ROW; i--) {
        if (imageSide[i][0] == 0)
            count++;
        else {
            if (count > 10 && index < 5) {
                start[index] = i + count;
                end[index] = i;
                index++;
            }
            count = 0;
        }
    }
    if (index > 1) {
        if (end[0] - start[1] > 10)
            leftState = 1;
    }

    if (errR && errF) {
        count = 0;
        index = 0;
        //�����Ƿ���ͻ��
        for (i = 159 - 1; i > 0; i--) {
            if (UpdowmSide[0][i] != 1 && UpdowmSide[0][i + 1] != 1) {
                if (UpdowmSide[0][i] >= UpdowmSide[0][i + 1])
                    index++;
                else
                    count++;
                /* �л��� */
                if (index > 20 && count > 20) {
                    *flag = 1;
                    return 1;
                }
            } else {
                index = 0;
                count = 0;
            }
        }
    }

    return 0;

}

/*!
  * @brief    ��ȡʮ�ֱ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  *
  * @return
  *
  * @note     ˼·�����м�����������
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */
void CrossGetSide(unsigned char imageInput[LCD_H][LCD_W], unsigned char imageSide[LCD_H][2])
{
    unsigned char i = 0, j = 0;

    for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
    {
        for(j = 78; j > 1; j--)
        {
            if(imageInput[i][j])
            {
                imageSide[i][0] = j;
                break;
            }
        }

        for(j = 78; j < 159; j++)
        {
            if(imageInput[i][j])
            {
                imageSide[i][1] = j;
                break;
            }
        }
    }

}

/*!
  * @brief    Ѱ�������
  *
  * @param    imageSide   �� ��������
  * @param    status      ��1����߽�   2���ұ߽�
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
unsigned char ImageGetHop(unsigned char imageSide[LCD_H][2], unsigned char state, unsigned char *x, unsigned char *y)
{
    int i = 0;
    unsigned char px = 0, py = 0;
    unsigned char count = 0;
    switch(state)
    {
      case 1:
        /* Ѱ������� */
        for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
        {
            if(imageSide[i][0] == 0 && i > (ROAD_END_ROW + 5))
            {
                count++;

                if(count > 5)
                {
                    if(imageSide[i-1][0] > (imageSide[i][0] + 20))
                    {
                        py = i - 1;
                        px = imageSide[py][0];
                        break;
                    }
                    if(imageSide[i-2][0] > (imageSide[i-1][0] + 20))
                    {
                        py = i - 2;
                        px = imageSide[py][0];
                        break;
                    }
                    if(imageSide[i-3][0] > (imageSide[i-2][0] + 20))
                    {
                        py = i - 3;
                        px = imageSide[py][0];
                        break;
                    }
                    if(imageSide[i-4][0] > (imageSide[i-3][0] + 20))
                    {
                        py = i - 4;
                        px = imageSide[py][0];
                        break;
                    }

                }

            }
            else
            {
                count = 0;
            }
        }

        if(py != 0)
        {
            *x = px;
            *y = py;
            return 1;
        }

        break;


      case 2:
        /* Ѱ������� */
        for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
        {
            if(imageSide[i][1] == 159 && i > (ROAD_END_ROW + 5))
            {
                count++;

                if(count > 5)
                {
                    if(imageSide[i-1][1] < (imageSide[i][1] - 20))
                    {
                        py = i - 1;
                        px = imageSide[py][1];
                        break;
                    }
                    if(imageSide[i-2][1] < (imageSide[i-1][1] - 20))
                    {
                        py = i - 2;
                        px = imageSide[py][1];
                        break;
                    }
                    if(imageSide[i-3][1] < (imageSide[i-2][1] - 20))
                    {
                        py = i - 3;
                        px = imageSide[py][1];
                        break;
                    }
                    if(imageSide[i-4][1] < (imageSide[i-3][1] - 20))
                    {
                        py = i - 4;
                        px = imageSide[py][1];
                        break;
                    }

                }

            }
            else
            {
                count = 0;
            }
        }

        if(py != 0)
        {
            *x = px;
            *y = py;
            return 1;
        }

        break;
    }

    return 0;

}

/*!
  * @brief    ʮ�ֲ��ߴ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageSide  �� ��������
  * @param    status     ��ʮ�ֱ�־λ   1������ʮ��    2������ʮ��   3����ʮ��
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
void CrossProcess(unsigned char  imageInput[LCD_H][LCD_W], unsigned char  imageSide[LCD_H][2], unsigned char * state)
{

    unsigned char  pointX = 0, pointY = 0;
    unsigned char  leftIndex = 0;
    static unsigned char  count  = 0;
    switch(*state)
    {
      case 1:
        {
            /* ���»�ȡ���� */
            CrossGetSide(imageInput, imageSide);

            /* Ѱ������� */
            if(ImageGetHop(imageSide, 1, &pointX, &pointY))
            {
                /* ���� */
                ImageAddingLine(imageSide, 1, pointX, pointY, 0, ROAD_START_ROW);
            }

            leftIndex = pointY;
            pointX = 0;
            pointY = 0;

            /* Ѱ������� */
            if(ImageGetHop(imageSide, 2, &pointX, &pointY))
            {
                /* ���� */
                ImageAddingLine(imageSide, 2, pointX, pointY, (LCD_W - 1), ROAD_START_ROW);
            }

            if(leftIndex != 0 && pointY != 0 && leftIndex >= ROAD_MAIN_ROW && pointY >= ROAD_MAIN_ROW)
            {
                * state = 2;
                count = 0;
            }

            if(count++ > 20)
            {
                * state = 2;
                count = 0;
            }

            break;
        }

      case 2:
        {

            /* ��黡�� */
            if(RoundaboutGetArc(imageSide, 1, 5, &leftIndex))
            {
                /* ����ȷ����߽� */
                RoundaboutGetSide(imageInput, imageSide, 1);

                if(ImageGetHop(imageSide, 1, &pointX, &pointY))
                {
                    /* ���� */
                    ImageAddingLine(imageSide, 1, pointX, pointY, imageSide[leftIndex][0], leftIndex);

                    * state = 3;

                    count = 0;
                }
                else
                {
                    imageSide[ROAD_MAIN_ROW][0] = LCD_W/2;
                    imageSide[ROAD_MAIN_ROW][1] = LCD_W - 1;
                }
            }

            break;
        }

      case 3:
        {

            /* ����ȷ����߽� */
            RoundaboutGetSide(imageInput, imageSide, 1);


            if(ImageGetHop(imageSide, 1, &pointX, &pointY))
            {
                /* ��黡�� */
                if(RoundaboutGetArc(imageSide, 1, 5, &leftIndex))
                {
                    /* ���� */
                    ImageAddingLine(imageSide, 1, pointX, pointY, imageSide[leftIndex][0], leftIndex);
                }
                else
                {
                    /* ���� */
                    ImageAddingLine(imageSide, 1, pointX, pointY, 0, ROAD_START_ROW);
                }

                if(pointY >= ROAD_MAIN_ROW)
                {
                    * state = 0;
                    count = 0;
                }
            }
            else
            {
                imageSide[ROAD_MAIN_ROW][0] = 120;
                imageSide[ROAD_MAIN_ROW][1] = LCD_W - 1;
            }

            if(count++ > 10)
            {
                *state = 0;
                count = 0;
            }

            break;
        }
    }

}

/*!
  * @brief    T�ִ���
  *
  *
  *
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */
unsigned char
TProcess(unsigned char imageInput[LCD_H][LCD_W], unsigned char imageUp[2][LCD_W], unsigned char imageSide[LCD_H][2],
         unsigned char *flag) {
    unsigned char py, i, num = 0;
    unsigned char errU1 = 0, errU2 = 0, errL1 = 0;
    switch (*flag) {
        case 1:
            //����ȷ���ϱ���
            Roundabout_Get_UpDowmSide(imageInput, imageUp, 1);
            errL1 = RoundaboutGetArc(imageSide, 1, 5, &py);    //�����л�
            errU1 = RoadUpSide_Mono(10, 140, imageUp);      //�ϵ�����

            if (errU1 == 2 && errL1 == 0)
                *flag = 2;

            //������ת������ת��뾶
            ImageAddingLine(imageSide, 1, 90, 30, 0, ROAD_START_ROW);
            break;

        case 2:
            errU2 = RoundaboutGetArc(imageSide, 2, 5, &py);//����ұ����Ƿ��л�
            for (i = 0; i < 159; i++) {
                if (imageUp[1][i] <= 118)
                    num++;
                if (num >= 140)
                    errU2 = 1;
            }
            if (errU2) {
                Servo_P = 12;
                *flag = 0;
                break;
            }
            ImageAddingLine(imageSide, 2, 60, 30, 159, ROAD_START_ROW);//���������޸�
            break;
    }
    return 0;
}

/*!
  * @brief    �ж��Ƿ���Y�Ͳ��
  *
  * @param    imageSide �� ͼ�������Ϣ
  * @param    flag      �� Y��״̬��Ϣ
  *
  * @return   0�����ǣ� 1����
  *
  * @note     ˼·�����߳ɻ�
  *
  * @see
  *
  * @date     2021/12/8 ������
  */
unsigned char RoadIsFork(unsigned char imageInput[2][LCD_W], unsigned char imageSide[LCD_H][2], unsigned char *flag,
                         unsigned char *pY) {

    unsigned char i = 0, errR = 0, errF = 0;
    unsigned char inc = 0, dec = 0, num = 0;
    unsigned char pointY;

    /* ��黡�� */
    errR = RoundaboutGetArc(imageSide, 2, 5, &pointY);
    errF = RoundaboutGetArc(imageSide, 1, 5, &pointY);

    if (errR) {
        if (UpSideErr(imageInput, 1, 20, &pointY)) {
            for (i = 110; i > 40; i--) {
                if (imageSide[i][0] == 0)
                    num++;
                if (num == 65) {
                    *flag = 1;
                    return 1;
                }
            }
        }
    }
    num = 0;
    if (errR && errF) {
        //�ж������Ƿ��л� ���Ƽ��жϻ��������ְ취�����Լ���װ��һ��������֮ǰ�İ취��һ���ľ����ԣ����Լ��滻��
        for (i = 159 - 1; i > 0; i--) {
            if (UpdowmSide[0][i] != 0 && UpdowmSide[0][i + 1] != 0) {
                if (UpdowmSide[0][i] == UpdowmSide[0][i + 1]) {
                    num++;
                    continue;
                }
                if (UpdowmSide[0][i] > UpdowmSide[0][i + 1]) {
                    inc++;
                    inc += num;
                    num = 0;
                }
                if (UpdowmSide[0][i] < UpdowmSide[0][i + 1]) {
                    dec++;
                    dec += num;
                    num = 0;
                }
                /* �л��� */
                if (inc > 15 && dec > 15) {
                    *flag = 1;
                    return 1;
                }
            } else {
                inc = 0;
                dec = 0;
                num = 0;
            }
        }
    }
    return 0;
}

/*!
  * @brief    Y�ֲ��ߴ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageSide  �� ��������
  * @param    status     ��ʮ�ֱ�־λ   1������ʮ��    2������ʮ��   3����ʮ��
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */

long RAllFork = 0;

void ForkProcess(unsigned char UpSideInput[2][LCD_W], unsigned char imageSide[LCD_H][2], unsigned char *state) {

    unsigned char pointY;
    static unsigned char D_flag = 0, dou_flag;

    //���»�ȡ�ϱ���
    UpdownSideGet(Binary_Image, UpdowmSide);

    switch (*state) {
        case 1://�жϹյ� ����յ�
            UpSideErr(UpSideInput, 1, 15, &pointY);
            if ((UpSideInput[0][pointY] > 30) || (D_flag)) {
                ImageAddingLine(imageSide, 1, 110, 35, 0, ROAD_START_ROW);  // ��Ļ���½����յ㣨�������޸ģ�
                D_flag = 1;
            }
            if (D_flag == 1 && RoadUpSide_Mono(30, 150, UpSideInput) == 2) {
                *state = 2;
            }
            break;
        case 2://�� ����

            if ((dou_flag == 1) && (!RoundaboutGetArc(imageSide, 2, 5, &pointY)))
                *state = 3;
            if (RoundaboutGetArc(imageSide, 2, 5, &pointY))
                dou_flag = 1;
            break;
        case 3://�� ����
            ImageAddingLine(imageSide, 1, 100, 30, 0, ROAD_START_ROW);//�������޸�
            if (RoadUpSide_Mono(5, 90, UpSideInput)) //�жϳ��ڽ��������
            {
                Servo_P = 12;
                if (g_ucForkNum == 2) {
                    Servo_P = 12;
                }
                D_flag = 0;
                dou_flag = 0;
                *state = 0;
            }
            break;
    }
}

/*!
  * @brief    ͣ���ߴ���
  *
  * @param    imageSide  �� ��������
  * @param    state      �� ͣ��״̬  1�����������   2���������Ҳ�
  * @param    speed      �� �ٶ�
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
void ZebraProcess(unsigned char imageSide[LCD_H][2], unsigned char state, short int *speed) {
    static short int count = 0;

    count++;

    if (state == 1) {
        imageSide[ROAD_MAIN_ROW][0] = 0;
        imageSide[ROAD_MAIN_ROW][1] = LCD_W / 2;
    } else {
        imageSide[ROAD_MAIN_ROW][0] = LCD_W / 2;
        imageSide[ROAD_MAIN_ROW][1] = LCD_W - 1;
    }

    if (count > 100) {
        *speed = 0;
        while (1);
    }

}

//*****************************************======*************************************************************************

//*****************************************��������*************************************************************************
/*!
  * @brief    ���������У���ȡ���ƫ��
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
short int RoadGetSteeringError(unsigned char imageSide[LCD_H][2], unsigned char lineIndex) {

    return imageSide[lineIndex][0] + imageSide[lineIndex][1] - 158;

}

static unsigned char g_ucFlagRoundabout_flag = 0;

//����Ԫ�ش������������޸ĵò�����С�����ٶȣ����Ŵ�����йأ���Ҫͬѧ���Լ�����ʵ��������޸�
void CameraCar(void) {
    unsigned char pointY;

    if (g_ucFlagRoundabout == 0 && g_ucFlagFork == 0 && g_ucFlagT == 0) {
        // ��⻷��
        RoadIsRoundabout(UpdowmSide, Binary_Image, ImageSide, &g_ucFlagRoundabout);
    }
    if (g_ucFlagRoundabout) {
        g_ucFlagRoundabout_flag = 1;

        //   ��������
        Servo_P = 12;           //���Ŵ�
        RoundaboutProcess(Binary_Image, ImageSide, UpdowmSide, &g_ucFlagRoundabout);

    }

/*********************************************************************************************************************************/
/*********************************************************************************************************************************/
    //ʮ�ֲ���δ�õ�
    if (g_ucFlagRoundabout == 0 && g_ucFlagCross == 0 && g_ucFlagFork == 0) {
        /* ���ʮ�� */
        RoadIsCross(ImageSide, &g_ucFlagCross);
    }
    if (g_ucFlagCross) {
        /* ʮ�ִ��� */
        CrossProcess(Small_Image, ImageSide, &g_ucFlagCross);
    }

    /********************************************************************************************/
    /********************************T��·��**********************************************/

    if (g_ucFlagRoundabout == 0 && g_ucFlagFork == 0 && g_ucFlagT == 0) {
        //���T��
        RoadIsT(UpdowmSide, ImageSide, &g_ucFlagT);
    }
    if (g_ucFlagT) {
        Servo_P = 12;
        //T�ִ���
        TProcess(Binary_Image, UpdowmSide, ImageSide, &g_ucFlagT);
    }

    /************************************************************************
      2021/7/19���Դ���  Y��·��
      ************************************************************************/
    if (g_ucFlagRoundabout == 0 && g_ucFlagFork == 0 && g_ucFlagT == 0) {
        RoadIsFork(UpdowmSide, ImageSide, &g_ucFlagFork, &pointY);
    }
    if (g_ucFlagFork == 1) {
        g_ucForkNum += 1;
    }

    if (g_ucFlagFork)//�������
    {

        Servo_P = 10;
        // Y�ִ���
        ForkProcess(UpdowmSide, ImageSide, &g_ucFlagFork);
    }
    ////////////////////////����ʶ�����///////////////////////////
    ////////////////////////�ⲿ��δ�޸�///////////////////////////
    if (g_ucFlagRoundabout == 0 && g_ucFlagCross == 0 && g_ucFlagZebra == 0 && g_ucFlagFork == 0) {
        /* ��⳵�� */
        RoadIsCross(ImageSide, &g_ucFlagZebra);
    }
    if (g_ucFlagZebra) {
        ZebraProcess(Small_Image, 1, 1200);
    }


    /* ���������У���ȡ���ƫ�� */
    g_sSteeringError = RoadGetSteeringError(ImageSide, ROAD_MAIN_ROW);
    //ƫ��Ŵ�
    ServoDuty = g_sSteeringError * Servo_P / 10;
    /*//ƫ���޷�
    if (ServoDuty > 170) ServoDuty = 170;
    if (ServoDuty < -170) ServoDuty = -170;*/

    if (ServoDuty > Servo_Delta) ServoDuty = Servo_Delta;
    if (ServoDuty < -Servo_Delta) ServoDuty = -Servo_Delta;
    //������
    ServoCtrl(Servo_Center_Mid - ServoDuty);
}

//*****************************************======*************************************************************************

//*****************************************������ʾ*************************************************************************
image_t img_line = DEF_IMAGE((unsigned char *) line_data, LCD_W, LCD_H);

int clip(int x, int low, int up) {
    return x > up ? up : x < low ? low : x;
}

void draw_line(image_t *img) {

    // assert(img && img->data);
    if (img->width == img->step) {
        memset(img->data, 0, img->width * img->height);
    } else {
        for (int y = 0; y < img->height; y++) {
            memset(&AT_IMAGE(img, 0, y), 0, img->width);
        }
    }

    for (int i = 0; i < LCD_H; i++) {
        AT_IMAGE(&img_line, clip(ImageSide[i][0], 0, img_line.width - 1),
                 clip(ImageSide[i][1], 0, img_line.height - 1)) = 255;
    }
    for (int i = 0; i < LCD_W; i++) {
        AT_IMAGE(&img_line, clip(UpdowmSide[i][0], 0, img_line.width - 1),
                 clip(UpdowmSide[i][1], 0, img_line.height - 1)) = 255;
    }
}

//*****************************************======*************************************************************************

