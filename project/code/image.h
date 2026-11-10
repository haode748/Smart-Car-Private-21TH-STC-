#ifndef __Image_H_
#define __Image_H_

#include "hexin.h"

#define SEARCH_IMAGE_W        MT9V03X_W       // 搜线宽度（与摄像头输出宽度一致）
#define SEARCH_IMAGE_H        MT9V03X_H       // 搜线高度（与摄像头输出高度一致）
#define BLACKPOINT            195              // 黑点阈值（低于此值判定为黑色）
#define WHITEMAXMUL           15              // 白点最大值基于参考点的放大倍数（10为不放大）
#define WHITEMINMUL           8               // 白点最小值基于参考点的放大倍数（10为不放大）
#define REFERENCEROW          3               // 参考点统计行数
#define REGION_WIDTH          90              // 中间区域宽度（列数）
#define SEARCHRANGE           10              // 搜线半径
#define STOPROW               2               // 搜线停止行
#define CONTRASTOFFSET        5               // 搜线对比偏移量，				每5列/行扫一次
//#define Search_Stop_Line      60              // 参与计算的行数
//#define SEARCH_COL_START      30              // 参考列搜索起始
//#define SEARCH_COL_END      (MT9V03X_W - 30)  // 参考列搜索结束
//#define STOPROW_1              20               // 搜线停止行


//typedef enum
//{
//	lost ,   //丢线
//	exist  //有线
//}RS_line_state;

//typedef struct
//{
//  uint8 right_point;
//	uint8 left_point;
//	
//  uint8 Last_right_point;
//	uint8 Last_left_point;	
//}RS_line_point;

//typedef struct
//{
//  RS_line_state  left_state;
//	RS_line_state  right_state;
//	RS_line_point  line_point;
//	
//	int16 line_left ;
//  int16 line_right;
//	
//	uint8 Patch_width;
//	uint8 Last_Patch_width;
//	
//}RS_line_message;

extern int32 cycle_error   ,
             Img_error     ,
             Img_error_last;
//             

//extern RS_line_message image;



// 全局外部变量声明
extern int Left_Lost_Time;    //左丢线
extern int Right_Lost_Time;   //右丢线
extern int Both_Lost_Time;    //左右丢线
extern int Boundry_Start_Left;              // 左边界有效起始行
extern int Boundry_Start_Right;             // 右边界有效起始行
extern int Search_Stop_Line;                //搜索截止行
extern uint8 SEARCH_COL_START;  // 先定义
extern uint8 SEARCH_COL_END;
extern uint8 reference_point;               // 动态参考点（图像灰度基准）
extern uint8 reference_col;                 // 动态参考列（中心线基准列）
extern uint8 white_max_point;               // 动态白点最大值（二值化上限）
extern uint8 white_min_point;               // 动态白点最小值（二值化下限）
extern uint8 reference_contrast_ratio;      // 参考对比度（边界检测阈值）
extern uint8 reference_col_line[SEARCH_IMAGE_H];  // 参考列绘制数组
extern uint8 remote_distance[SEARCH_IMAGE_W];     // 白点远端距离数组
extern uint16 left_edge_line[SEARCH_IMAGE_H];      // 左边界数组（行→列）
extern uint16 right_edge_line[SEARCH_IMAGE_H];     // 右边界数组（行→列）
extern uint32 if_count;                       // 条件计数（通用计数变量）
extern uint8 Cross_flag;                     // 十字检测标志位
extern uint8 start_flag;
extern uint8 Weight[MT9V03X_H];              // 外部声明：权重数组
extern uint8 Standard_Road_Wide[MT9V03X_H];   //赛道宽度
extern uint8 current_col;                    
extern uint16 Road_Wide[SEARCH_IMAGE_H];           //路宽
//void left_buline(void);                      //左环岛补线
//void right_buline(void);                     //右环岛补线
uint8 find_extreme_value(uint8 *array_value, uint8 num0, uint8 num1, uint8 model);
uint8 find_jump_point(uint16 *array_value, uint8 num0, uint8 num1, uint8 jump_num, uint8 model);// 跳变点检测
void get_reference_point(const uint8 *image);   // 获取图像灰度基准点（带图像指针参数）
void search_reference_col(const uint8 *image);                // 搜索参考列（中心线，依赖全局参考点）
void search_line(const uint8 *image);           // 搜索赛道左右边界（带图像指针参数）
void cross_analysis(void);                      // 十字检测与补线（依赖全局边界数组）
void connect_point(uint8 *array_value, uint8 num0, uint8 num1); // 线性插值补线（带参数）
void error_calcul(void);                        // 误差求取（依赖全局边界/参考列）
void Image_Pro(void);             // 图像处理主函数（带图像指针参数，统一传参）
void Image_Pro_1(void);
void UART_Print_Full_Image_Gray(void);







#endif





