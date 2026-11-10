#include <zf_common_headfile.h>


void Wu_motor_Init(void)
{
    pwm_set_duty(PWMF_CH1_PA1,500);
    pwm_set_duty(PWMF_CH2_PA3,500);
}