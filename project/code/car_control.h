//组    别：雁过留痕
//队    名：CWNU-探痕启航队
//作    者：钱镜慧
//创建时间：2025-11-24
// Car_Control.h
// Car_Control.h
#ifndef Car_Control_H
#define Car_Control_H

#include "zf_common_headfile.h"  // 先包含基础头文件
#include "hexin.h"
//陀螺仪
#define   DT                    0.005
//蜂鸣器
#define Beep			(P45)  
//激光

#define JG1        (P51)
#define JG2        (P50)
#define JG3        (P52)
#define JG4        (P53)
#define JG5        (P67)
#define JG       (P64)
#define JG6        (P01)
#define JG7        (P02)
//#define JG       (P64)

// 电机
#define PWM_L               (PWMB_CH1_P74)						//左电机速度
#define PWM_R               (PWMB_CH3_P76)
#define Dir_L_init  				(P7_5)
#define Dir_R_init  				(P7_7)
#define Dir_L               (IO_P75)									// 左电机方向
#define Dir_R               (IO_P77)

//测试
#define  test          ( P11 )

//编码器
#define ENCODER_DIR_2                 	(PWMA_ENCODER)              // 带方向编码器对应使用的编码器接口 
#define ENCODER_L_PULSE_PIN            	(PWMA_ENCODER_CH1P_P60)     // PULSE 对应的引脚
#define  ENCODER_L_DIR_PIN             	(PWMA_ENCODER_CH2P_P62)     // DIR 对应的引脚

#define ENCODER_DIR_1                 	(PWMC_ENCODER)              // 带方向编码器对应使用的编码器接口
#define ENCODER_R_PULSE_PIN       		(PWMC_ENCODER_CH1P_P40)     // PULSE 对应的引脚
#define  ENCODER_R_DIR_PIN           	(PWMC_ENCODER_CH2P_P42)     // DIR 对应的引脚

// 编码器模块定义							左右各一个编码器
#define ENCODER_R           PWMC_ENCODER
#define ENCODER_L          PWMA_ENCODER

// 舵机
#define SERVO_PWM               (PWME_CH1P_PA0)           // 定义主板上舵机对应引脚
#define SERVO_FREQ              (300)                     // 定义主板上舵机频率  请务必注意范围 50-300
#define Min_servo               (75)                     // 定义主板上舵机活动范围 
#define Max_servo               (103)                    // 定义主板上舵机活动范围 
#define SERVO_DUTY(x)           ((float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_FREQ) * (0.5 + (float)(x) / 90.0))
//#define angle_rad   ( ( -40.0 + (Servo_duty - Min_servo) * (90.0 / (Max_servo - Min_servo)) ) * (3.1415926 / 180.0) )
#if ((SERVO_FREQ < 50) || (SERVO_FREQ > 300))
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif

typedef enum
{
    Go,
    Back,
} RS_Direct;

typedef enum
{
    L_wheel,  // 左轮
    R_wheel,  // 右轮
} RS_Car_wheel;

typedef enum
{
    Run,   // 行驶模式
    Lock,  // 锁车模式
} RS_CarMode;

// 注意宏定义，结构体，枚举要放在前面，否则会报错
extern int32 Speed_L;
extern int32 Speed_R;
extern int32 PWM_L_set;
extern int32 PWM_R_set;
extern RS_CarMode Car_Mode;
extern RS_Direct dir_l_state;   // 保存左轮方向状态
extern RS_Direct dir_r_state;   // 保存右轮方向状态
extern int32 encoder_data_dir_1;
extern int32 encoder_data_dir_2;
extern int16 ticker_count;
extern uint16 Median_servo;
extern uint16 Servo_duty;
extern float groyz;                // Z轴角速度（°/s）
extern float car_angle;            // 车身转向角度（累计角度）
extern float gyro_offset_z;        // Z轴零偏（校准用）
void JGD(void);                     //激光


void encoder_get(void);
void Car_inital(void);
void car_pwm_set(RS_Car_wheel car_wheel, RS_Direct car_dir, uint32 RS_PWM);
void car_control_n(int32 L, int32 R);
void car_control(void);
void gyro_get(void);
void gyro_calibrate_z(void);
void get_car_angle(void);
void Clear_Gyroscope_Angle(void);
void Clear_Gyroscope_Pitch(void);
void Get_Gyroscope_Pitch(void);
#endif