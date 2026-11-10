#ifndef __PID_H_
#define __PID_H_
#include "hexin.h"

// 全局参数定义
#define LDISTANCE 200.0f   // 前后轮轴距 (mm);;
#define BORDWIDTH 155.0f   // 左右后轮中心距 (mm)
#define INTEGRAL_SEP_THRESHOLD   10



typedef struct{
  float Kp;
  float Ki;
  float Kd;
  int32 p_out;
  int32 i_out;
  int32 d_out;
  int32 Err;
  int32 Last_Err;       // 上次误差
  int32 Previous_Err;   // 上上次误差
  int32 Output;
	int32 Target;
	int32 Measure;				// 测量值（编码器读数）
	
}RS_Incremental_PID;
extern RS_Incremental_PID Pid_vl;				// 左轮速度环
extern RS_Incremental_PID Pid_vr;				// 右轮速度环				系统有 4 个 PID 控制器。
extern RS_Incremental_PID Pid_turn;			// 舵机转向环				每个都有 Kp/Ki/Kd + Target + Measure + Output。
extern RS_Incremental_PID Pid_wheel_turn;// 后轮差速环			PID 就是"根据 (Target - Measure) 算出一个修正量"的数学工具

extern	int32 car_dir,vl_ex,vr_ex , car_wheel_dir, pwm_r;
extern  int32 Absolute(int32 value);
extern float kd2;
extern float kp2;
extern uint8 flage;
extern RS_Incremental_PID Pidzd_vl;
extern RS_Incremental_PID Pidzd_vr;
extern RS_Incremental_PID Pidzd_turn;
//PID控制器初始化
void PID_Start_init(void);


//增量式PID
int32 IncrementalPID_Calculate(RS_Incremental_PID *pid , int32 wheel_ex );

//位置式PD
int32 Postion_PID_Cal(RS_Incremental_PID *pid , int32 error);
int32 Postion_P2ID_Cal(RS_Incremental_PID *pid , int32 error ,float groyz , float kp2, float kd2);








#endif