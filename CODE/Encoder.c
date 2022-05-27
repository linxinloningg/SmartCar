#include "headfile.h"
#include "Binarization.h"
#include "camera.h"
#include "Encoder.h"
#include "Motor.h"
#include "Servo.h"
#include "Variable.h"
#include "SJUOpenSource.h"
#include "LQOpenSource.h"

void EncoderInit(void) {
    //��һ������ ��ʾѡ��ʹ�õĶ�ʱ��
    //�ڶ������� ��ʾѡ��ļ�������    ���������뷽�����Ų��ɽ���
    //���������� ��ʾѡ��ķ�������
    gpt12_init(GPT12_T5, GPT12_T5INB_P10_3, GPT12_T5EUDB_P10_1);        //���
    gpt12_init(GPT12_T6, GPT12_T6INA_P20_3, GPT12_T6EUDA_P20_0);        //�ұ�
}

/*�ٶȲɼ���������ʱ��������*/
void speed_get(void) {
//    Left_rear_speed = 65536 - gpt12_get(GPT12_T5);
//    Right_rear_speed = gpt12_get(GPT12_T6);
    Left_rear_speed = -(65536 - gpt12_get(GPT12_T5));   //��������װλ�õ����˼�һ�����ž�����ȷ�ķ���
    Right_rear_speed = -(gpt12_get(GPT12_T6));
    gpt12_clear(GPT12_T5);
    gpt12_clear(GPT12_T6);

}


