#ifndef __MYMENU_H
#define __MYMENU_H

#include "ALL.h"
#include "menu.h"

void menu_Init(PID_t *pid1, PID_SERVO *pid2);
void Show_Key(void);
void Show_Number(void);
void menu_show(void);
void Key_down(void);
void Key_enter(void);
void Key_qiut(void);
void Key_select(void);
void Key_2(void);
void Key_variable_plusORsub_1(void);
void Key_3(void);

#endif
