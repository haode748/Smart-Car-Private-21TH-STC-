#ifndef __bullseye_H_
#define __bullseye_H_

#include "hexin.h"

// 靶子检测
#define SHOT_COOLDOWN_FRAMES 20
extern uint8 bullseye_flag;       // 1=检测到靶子
extern uint8 wd_flag;
extern uint16 bullseye_center_x; // 靶子中心 X
extern uint16 bullseye_center_y;
extern int8 laser_on;          // 激光是否点亮中
extern uint16 laser_delay_count; // 还需要亮多少毫秒
void detect_bullseye(const uint8 *image);

#endif
