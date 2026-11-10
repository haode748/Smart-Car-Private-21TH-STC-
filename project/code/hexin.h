//组    别：雁过留痕
//队    名：CWNU-探痕启航队
//作    者：钱镜慧
//创建时间：2025-11-24

#ifndef __Hexin_Tanhen_H_
#define __Hexin_Tanhen_H_

//逐飞头文件
#include "zf_common_headfile.h"

//控制
#include "Car_Control.h"
#include "PID.h"

//屏幕显示
#include "screen.h"

//图像
#include "Image.h"
#include "cycle.h"
#include "cross.h"
#include "zebra.h"
#include "straight.h"
#include "bullseye.h"

//菜单
#include "Para_tuning.h"
#include "Menu.h"


#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define Clip(input, low, upper) Min(Max(input, low), upper)

#endif