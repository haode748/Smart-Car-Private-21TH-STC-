#ifndef __Para_tuning_H_
#define __Para_tuning_H_

#include "hexin.h"


// 定义取绝对值的宏
#define Absoult_val(x) ((x) < 0 ? -(x) : (x))
//#define P7_0  IO_P70
//#define P7_1  IO_P71
//#define P7_2  IO_P72
//#define P7_3  IO_P73

#define GPIO_WAY 1		// 1-SEEKFREE V3库的方式操作引脚
						// 0-传统的位绑定的方式操作引脚

//#if GPIO_WAY
	// 定义按键引脚
	#define KEY1_PIN    IO_P32
	#define KEY2_PIN    IO_PB2
	#define KEY3_PIN    IO_PB4
	#define KEY4_PIN    IO_PB3
	#define KEY5_PIN    IO_PB1
//#else
//	// 定义按键引脚
//	#define KEY1_PIN    P70
//	#define KEY2_PIN    P71
//	#define KEY3_PIN    P72
//	#define KEY4_PIN    P73
//#endif
#define PARAM_SAVE_ADDR  0x3D000  // 根据 STC 数据手册确认有效范围

typedef enum
{
none_run_car,
run_car,
	
}RS_RUNCAR ;




extern float  float_step;
extern float  step_num;
extern uint8 lcd_enabled ;
void Key_down(void);
void Key_init(void);
//void Tire_tuning(void);

typedef struct {
float	Target ;
    float Kp;   // 比例系数
    float Ki;   // 积分系数
    float Kd;   // 微分系数
} PID_Const;



// PID参数
extern PID_Const constPid_vl;
extern PID_Const constPid_vr;
extern PID_Const constPid_turn;
extern PID_Const constPid_wheel_turn;

// 控制变量
extern uint16  Arrow_point;
extern uint16 Arrow_point_old;
extern int temp_change;
extern int temp_change_old;
//extern int point_num;
//extern int none_run_car;

// 菜单变量
//extern int image_1;
//extern int image_2;
extern int Patrol_mode;
extern int Travel_mode;

// 道路识别变量
extern int RS_width_cacul;
extern int RS_left_cacul;
extern int RS_right_cacul;















#endif