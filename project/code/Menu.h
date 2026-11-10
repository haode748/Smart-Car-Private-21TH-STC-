#ifndef __IPS200_H_
#define __IPS200_H_

#include "hexin.h"



typedef enum
{
	Image,
//	Car_mode,
	Para,
	para_tun,
}RS_Screen_mode;

typedef enum
{
	int_,
	float_,

}RS_data_mode;



extern RS_Screen_mode Screen_mode;
extern uint16  Arrow_point ;

//void Screen_Image(void);
//void Screen_Para(void);
//void Screen_Car_mode(void);
//void Screen_ips200(void);
void para_tuning_dis(uint16 arrow_point);
void show_edge_data(int x, int y, const char* label, void* value, RS_data_mode data_mode) ;
void Screen_ips200(void) ;














#endif