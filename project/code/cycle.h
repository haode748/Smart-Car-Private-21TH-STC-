#ifndef __cycle_H_
#define __cycle_H_

#include "hexin.h"

//#define   GYRO_SENS             1/16.4
//#define   DT                    0.01

//extern int ZeroDrift_gyro_z;
//extern int16 FJ_gyro_z ;
extern float FJ_Angle ;//????????????
//extern float FJ_LastPitchSpeed ;
//extern float FJ_AngleSpeed ;
//extern float FJ_LastAngleSpeed ;
//extern float FJ_Angle_Max;
//extern float FJ_Angle_Min ;
//extern float AngleSpeed ;



//extern int Cross_Flag;                      // 十字赛道标志位
extern int Zebra_State;                     // 斑马线标志位
extern int Ramp_Flag;                       // 坡道标志位
extern int h;
extern int count_flag;
// 环岛状态机专用变量，外部可访问
extern int Island_State;                    // 环岛状态机（0~9）
extern int Left_Island_Flag;                // 左环岛标志
extern int Right_Island_Flag;               // 右环岛标志
extern int Left_Up_Guai[2];                 // 左上方拐点坐标 [y,x]
extern int Right_Up_Guai[2];                // 右上方拐点坐标 [y,x]

void Island_Detect(void);

// 边界连续性判断函数
int Continuity_Change_Left(int start,int end);       // 左边界连续性检测
int Continuity_Change_Right(int start,int end);      // 右边界连续性检测

// 拐点检测函数
int Find_Left_Down_Point(int start,int end);         // 寻找左下角拐点
int Find_Left_Up_Point(int start,int end);           // 寻找左上角拐点
int Find_Right_Down_Point(int start,int end);        // 寻找右下角拐点
int Find_Right_Up_Point(int start,int end);          // 寻找右上角拐点

// 单调性突变检测函数
int Monotonicity_Change_Left(int start,int end);     // 左边界单调性变化检测
int Monotonicity_Change_Right(int start,int end);    // 右边界单调性变化检测

//// 状态显示函数（屏幕/图像叠加显示）
//void Image_Flag_Show(uint8 MT9V03X_W,uint8(*InImg)[MT9V03X_W],uint8 image_flag);

// 补线函数（环岛丢线时用）
void K_Add_Boundry_Left(float k,int startX,int startY,int endY);   // 斜率补左边界
void K_Add_Boundry_Right(float k,int startX,int startY,int endY);  // 斜率补右边界
void K_Draw_Line(float k, int startX, int startY,int endY);         // 根据斜率画直线
void Left_Add_Line(int x1,int y1,int x2,int y2);
void Right_Add_Line(int x1,int y1,int x2,int y2);
void Lengthen_Left_Boundry(int start,int end);
void Lengthen_Right_Boundry(int start,int end);
void Draw_Line(int startX, int startY, int endX, int endY);
// 赛道参数计算工具函数
int Get_Road_Wide(int start_line,int end_line);       // 获取平均赛道宽度
float Get_Left_K(int start_line,int end_line);        // 计算左边界斜率
float Get_Right_K(int start_line,int end_line);       // 计算右边界斜率


#endif 
