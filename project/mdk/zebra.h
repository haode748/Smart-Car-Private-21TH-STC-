#ifndef __ZEBRA_H
#define __ZEBRA_H

#include "hexin.h"

extern int Zebra_State;                     // 斑马线标志位
extern int Barricade_Flag;                  //路障标志位
extern uint8 Stop_Flag;                 // 停车标志

void Zebra_idntify(void);

#endif
