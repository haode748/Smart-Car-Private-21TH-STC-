#include "straight.h"

static uint8 Straight_Cnt = 0;  
int Straight_Flag=0;

// 获取左赛道边界斜率
float Get_Left_K(int start_line,int end_line)
{
	  float k=0;
    int t=0;
    if(start_line>=MT9V03X_H-1)
        start_line=MT9V03X_H-1;
    else if(start_line<=0)
        start_line=0;
    if(end_line>=MT9V03X_H-1)
        end_line=MT9V03X_H-1;
    else if(end_line<=0)
        end_line=0;
    if(start_line>end_line)//++访问，坐标反了互换
    {
        t=start_line;
        start_line=end_line;
        end_line=t;
    }
    k=(float)(((float)left_edge_line[start_line]-(float)left_edge_line[end_line])/(end_line-start_line+1));
    return k;
}


// 获取右赛道边界斜率
float Get_Right_K(int start_line,int end_line)
{
	  float k=0;
    int t=0;
    if(start_line>=MT9V03X_H-1)
        start_line=MT9V03X_H-1;
    else if(start_line<=0)
        start_line=0;
    if(end_line>=MT9V03X_H-1)
        end_line=MT9V03X_H-1;
    else if(end_line<=0)
        end_line=0;
    if(start_line>end_line)//++访问，坐标反了互换
    {
        t=start_line;
        start_line=end_line;
        end_line=t;
    }
    k=(float)(((float)right_edge_line[start_line]-(float)right_edge_line[end_line])/(end_line-start_line+1));
    return k;
}

void Straight_Detect(void)
{
	float k_left,k_right;
	float abs_sum;
	Straight_Flag=0;
	k_left=Get_Left_K(3,MT9V03X_H-3);
	k_right=Get_Right_K(3,MT9V03X_H-3);
  abs_sum = fabs(k_left + k_right);
//  printf(" k_left:%.2f\r\n",k_left);
//	printf(" k_right:%.2f\r\n",k_right);
//	printf("L3=%d  L_end=%d    ", left_edge_line[3], left_edge_line[MT9V03X_H-3]);
//	printf("R3=%d  R_end=%d\r\n", right_edge_line[3], right_edge_line[MT9V03X_H-3]);
	if(bullseye_flag==0&&Island_State==0)
	{
	if(abs_sum < 0.2&&abs(Img_error) < 10&&k_left>0&&k_right<0)  // ?? 直行：数值小
	{
		if(Straight_Cnt < STRAIGHT_COUNT_MAX)
			Straight_Cnt++;
	}
	else  // ?? 不直：数值大
	{
		if(Straight_Cnt > 0)
			Straight_Cnt--;
	}

	// 输出
	Straight_Flag = (Straight_Cnt >= STRAIGHT_COUNT_MAX) ? 1 : 0;
//	if(Straight_Flag == 1)
//	{
//		Beep=1;
//	}else{
//		Beep=0;
//	}
	}
}



