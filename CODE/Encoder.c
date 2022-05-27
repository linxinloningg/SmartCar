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
    //第一个参数 表示选择使用的定时器
    //第二个参数 表示选择的计数引脚    计数引脚与方向引脚不可交换
    //第三个参数 表示选择的方向引脚
    gpt12_init(GPT12_T5, GPT12_T5INB_P10_3, GPT12_T5EUDB_P10_1);        //左边
    gpt12_init(GPT12_T6, GPT12_T6INA_P20_3, GPT12_T6EUDA_P20_0);        //右边
}

/*速度采集函数，定时器内运行*/
void speed_get(void) {
//    Left_rear_speed = 65536 - gpt12_get(GPT12_T5);
//    Right_rear_speed = gpt12_get(GPT12_T6);
    Left_rear_speed = -(65536 - gpt12_get(GPT12_T5));   //编码器安装位置导致了加一个符号就是正确的方向
    Right_rear_speed = -(gpt12_get(GPT12_T6));
    gpt12_clear(GPT12_T5);
    gpt12_clear(GPT12_T6);

}


