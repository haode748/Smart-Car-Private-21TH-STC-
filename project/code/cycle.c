#include "cycle.h"

int Island_State=0;     //??????
int Left_Island_Flag=0; //??????
int Right_Island_Flag=0;//??????
int Left_Up_Guai[2];    //?????????,[0]?y,???,{1}?x,???
int Right_Up_Guai[2];   //?????????,[0]?y,???,{1}?x,???
int Ramp_Flag=0; 
uint16 add_line_counter = 0;
int h=0;
int count_flag=0;
//int b,c;
float a;

//   ????????
int Continuity_Change_Left(int start,int end)
{
    int i;
    int t;
    int continuity_change_flag=0;
//    int err_cnt = 0;  // ??????

    if(Left_Lost_Time>=0.9*MT9V03X_H)//??????,??????
       return 0;
    if(Search_Stop_Line<=5)//???????
       return 0;
    if(start>=MT9V03X_H-1-5)//??????
        start=MT9V03X_H-1-5;
    if(end<=5)
       end=5;
    if(start<end)//?????????,???????
    {
       t=start;
       start=end;
       end=t;
    }

    for(i=start;i>=end;i--)
    {
        if(abs(left_edge_line[i]-left_edge_line[i-1])>=3)
        {
//            err_cnt++;  // ??+1

            // ?? 2 ???? ? ????????
//            if(err_cnt >= 3)
//            {
                continuity_change_flag = i;
                break;
//            }
        }
//        else
//        {
//            err_cnt = 0;  // ?????,??????
//        }
    }
    return continuity_change_flag;
}

// ????????
int Continuity_Change_Right(int start,int end)
{
    int i;
    int t;
    int continuity_change_flag=0;
//    int err_cnt = 0;  // ??:??????
    
    if(Right_Lost_Time>=0.9*MT9V03X_H)
       return 0;  // ????? ? ??0(????)
    if(start>=MT9V03X_H-5)
        start=MT9V03X_H-5;
    if(end<=5)
       end=5;
    if(start<end)
    {
       t=start;
       start=end;
       end=t;
    }
    for(i=start;i>=end;i--)
    {
        // ??????
        if(abs(right_edge_line[i]-right_edge_line[i-1])>=3)
        {
//            err_cnt++;  // ??+1
            
            // ??2???? ? ????????
//            if(err_cnt >= 3) 
//            {
                continuity_change_flag = i;
                break;
//            }
        }
//        else
//        {
//            err_cnt = 0; // ??????
//        }
    }
    return continuity_change_flag;
}

 //??????
int Find_Left_Down_Point(int start,int end)//?????,???????????
{
    int i,t;
    int left_down_line=0;
    if(Left_Lost_Time>=0.9*MT9V03X_H)//??????,?????????
       return left_down_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_H-1-5)//??5??????,??????????,??
        start=MT9V03X_H-1-5;
    if(end<=MT9V03X_H-Search_Stop_Line)
        end=MT9V03X_H-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(left_down_line==0&&//???????????
           abs(left_edge_line[i]-left_edge_line[i+1])<=5&&//?????????
           abs(left_edge_line[i+1]-left_edge_line[i+2])<=5&&  
           abs(left_edge_line[i+2]-left_edge_line[i+3])<=5&&
              (left_edge_line[i]-left_edge_line[i-2])>=5&&
              (left_edge_line[i]-left_edge_line[i-3])>=10&&
              (left_edge_line[i]-left_edge_line[i-4])>=10)
        {
            left_down_line=i;//??????
            break;
        }
    }
    return left_down_line;
}

// ??????

int Find_Left_Up_Point(int start,int end)//?????,???????????
{
    int i,t;
    int left_up_line=0;
    if(Left_Lost_Time>=0.9*MT9V03X_H)//??????,?????????
       return left_up_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(end<=MT9V03X_H-Search_Stop_Line)//????????????
        end=MT9V03X_H-Search_Stop_Line;
    if(end<=5)//?????????,???????,??????
        end=5;
    if(start>=MT9V03X_H-1-5)
        start=MT9V03X_H-1-5;
    for(i=start;i>=end;i--)
    {
        if(left_up_line==0&&//???????????
           abs(left_edge_line[i]-left_edge_line[i-1])<=5&&
           abs(left_edge_line[i-1]-left_edge_line[i-2])<=5&&  
           abs(left_edge_line[i-2]-left_edge_line[i-3])<=5&&
              (left_edge_line[i]-left_edge_line[i+2])>=8&& 
              (left_edge_line[i]-left_edge_line[i+3])>=15&&
              (left_edge_line[i]-left_edge_line[i+4])>=15)
        {
            left_up_line=i;//??????
            break;
        }
    }
    return left_up_line;//???MT9V03X_H-1,?????????
}

   // ??????
int Find_Right_Down_Point(int start,int end)//?????,???????????
{
    int i,t;
    int right_down_line=0;
    if(Right_Lost_Time>=0.9*MT9V03X_H)//??????,?????????
        return right_down_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_H-1-5)//??5??????,??????????,??
        start=MT9V03X_H-1-5;
    if(end<=MT9V03X_H-Search_Stop_Line)
        end=MT9V03X_H-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(right_down_line==0&&//???????????
           abs(right_edge_line[i]-right_edge_line[i+1])<=5&&//?????????
           abs(right_edge_line[i+1]-right_edge_line[i+2])<=5&&  
           abs(right_edge_line[i+2]-right_edge_line[i+3])<=5&&
              (right_edge_line[i]-right_edge_line[i-2])<=-5&&
              (right_edge_line[i]-right_edge_line[i-3])<=-10&&
              (right_edge_line[i]-right_edge_line[i-4])<=-10)
        {
            right_down_line=i;//??????
            break;
        }
    }
    return right_down_line;
}

//??????
int Find_Right_Up_Point(int start,int end)//?????,???????????
{
    int i,t;
    int right_up_line=0;
    if(Right_Lost_Time>=0.9*MT9V03X_H)//??????,?????????
        return right_up_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }    
    if(end<=MT9V03X_H-Search_Stop_Line)//????????????
        end=MT9V03X_H-Search_Stop_Line;
    if(end<=5)//?????????,???????,??????
        end=5;
    if(start>=MT9V03X_H-1-5)
        start=MT9V03X_H-1-5;
    for(i=start;i>=end;i--)
    {
        if(right_up_line==0&&//???????????
           abs(right_edge_line[i]-right_edge_line[i-1])<=5&&//?????????
           abs(right_edge_line[i-1]-right_edge_line[i-2])<=5&&  
           abs(right_edge_line[i-2]-right_edge_line[i-3])<=5&&
              (right_edge_line[i]-right_edge_line[i+2])<=-8&&
              (right_edge_line[i]-right_edge_line[i+3])<=-15&&
              (right_edge_line[i]-right_edge_line[i+4])<=-15)
        {
            right_up_line=i;//??????
            break;
        }
    }
    return right_up_line;
}


// ???????
int Monotonicity_Change_Left(int start,int end)//?????,???????????????
{
    int i;
    int monotonicity_change_line=0;
//	  int valid_count = 0;   // ??????,???
    if(Left_Lost_Time>=0.9*MT9V03X_H)//??????,??????????(??)
       return monotonicity_change_line;
    if(start>=MT9V03X_H-1-5)//??????,????i???
       start=MT9V03X_H-1-5; //??????5??,???????????????
    if(end<=5)
        end=5;
    if(start<=end)//????,????,????0
      return monotonicity_change_line;
    for(i=start;i>=end;i--)//????5?5??,????????????
    {
//			 printf("111");
        if(left_edge_line[i]==left_edge_line[i+5]&&left_edge_line[i]==left_edge_line[i-5]&&
        left_edge_line[i]==left_edge_line[i+4]&&left_edge_line[i]==left_edge_line[i-4]&&
        left_edge_line[i]==left_edge_line[i+3]&&left_edge_line[i]==left_edge_line[i-3]&&
        left_edge_line[i]==left_edge_line[i+2]&&left_edge_line[i]==left_edge_line[i-2]&&
        left_edge_line[i]==left_edge_line[i+1]&&left_edge_line[i]==left_edge_line[i-1])
        {//??????,???????????
//					  valid_count=0;
            continue;
        }
        else if(left_edge_line[i]>=left_edge_line[i+5]+3&&left_edge_line[i]>=left_edge_line[i-5]-3&&
        left_edge_line[i]>=left_edge_line[i+4]+3&&left_edge_line[i]>=left_edge_line[i-4]-3&&
        left_edge_line[i]>=left_edge_line[i+3]+3&&left_edge_line[i]>=left_edge_line[i-3]-3&&
        left_edge_line[i]>=left_edge_line[i+2]+3&&left_edge_line[i]>=left_edge_line[i-2]-3&&
        left_edge_line[i]>=left_edge_line[i+1]+3&&left_edge_line[i]>=left_edge_line[i-1]-3)
//				printf("222");
//        {//????,???????5,?5????(????),????????
//           valid_count++;

//            // ?? 2 ??? ? ???????
//            if(valid_count >= 2)
            {
                monotonicity_change_line = i;
                break;
            }
//        }
//        else
//        {
//            valid_count = 0; // ??????,??????
        }
//    }
// 	 printf("????=%d\r\n", monotonicity_change_line);
    return monotonicity_change_line;

}

//???????
int Monotonicity_Change_Right(int start,int end)//?????,???????????????
{
    int i;
    int monotonicity_change_line=0;
//    int valid_count = 0;   // ????????
	
    if(Right_Lost_Time>=0.9*MT9V03X_H)//??????,??????????
        return monotonicity_change_line;
    if(start>=MT9V03X_H-1-5)//??????
        start=MT9V03X_H-1-5;
     if(end<=5)
        end=5;
    if(start<=end)
        return monotonicity_change_line;
    for(i=start;i>=end;i--)//????5?5??,????????????
    {
        if(right_edge_line[i]==right_edge_line[i+5]&&right_edge_line[i]==right_edge_line[i-5]&&
        right_edge_line[i]==right_edge_line[i+4]&&right_edge_line[i]==right_edge_line[i-4]&&
        right_edge_line[i]==right_edge_line[i+3]&&right_edge_line[i]==right_edge_line[i-3]&&
        right_edge_line[i]==right_edge_line[i+2]&&right_edge_line[i]==right_edge_line[i-2]&&
        right_edge_line[i]==right_edge_line[i+1]&&right_edge_line[i]==right_edge_line[i-1])
        {//??????,???????????
//					  valid_count=0;
            continue;
        }
        else if(right_edge_line[i]<=right_edge_line[i+5]+3&&right_edge_line[i]<=right_edge_line[i-5]-3&&
        right_edge_line[i]<=right_edge_line[i+4]+3&&right_edge_line[i]<=right_edge_line[i-4]-3&&
        right_edge_line[i]<=right_edge_line[i+3]+3&&right_edge_line[i]<=right_edge_line[i-3]-3&&
        right_edge_line[i]<=right_edge_line[i+2]+3&&right_edge_line[i]<=right_edge_line[i-2]-3&&
        right_edge_line[i]<=right_edge_line[i+1]+3&&right_edge_line[i]<=right_edge_line[i-1]-3)
        {//????,???????5,?5????,????????
//					  valid_count++;
//             if(valid_count >= 2)
//            {
                monotonicity_change_line = i;
                break;
//            }
//        }
//        else
//        {
//            valid_count = 0;
        }
    }

    return monotonicity_change_line;
}


//????,????--
void K_Add_Boundry_Left(float k,int startX,int startY,int endY)
{
    int i,t;
    if(startY>=MT9V03X_H-1)
        startY=MT9V03X_H-1;
    else if(startY<=0)
        startY=0;
    if(endY>=MT9V03X_H-1)
        endY=MT9V03X_H-1;
    else if(endY<=0)
        endY=0;
    if(startY<endY)//--??,start???
    {
        t=startY;
        startY=endY;
        endY=t;
    }
    for(i=startY;i>=endY;i--)
    {
        left_edge_line[i]=(int)((i-startY)/k+startX);//(y-y1)=k(x-x1)??,x=(y-y1)/k+x1
        if(left_edge_line[i]>=MT9V03X_W-1)
        {
            left_edge_line[i]=MT9V03X_W-1;
        }
        else if(left_edge_line[i]<=0)
        {
            left_edge_line[i]=0;
        }
    }
}

// ????,????
void K_Add_Boundry_Right(float k,int startX,int startY,int endY)
{
    int i,t;
	  add_line_counter++;
    if(startY>=MT9V03X_H-1)
        startY=MT9V03X_H-1;
    else if(startY<=0)
        startY=0;
    if(endY>=MT9V03X_H-1)
        endY=MT9V03X_H-1;
    else if(endY<=0)
        endY=0;
    if(startY<endY)
    {
        t=startY;
        startY=endY;
        endY=t;
    }
    for(i=startY;i>=endY;i--)
    {
        right_edge_line[i]=(int)((i-startY)/k+startX);//(y-y1)=k(x-x1)??,x=(y-y1)/k+x1
        if(right_edge_line[i]>=MT9V03X_W-1)
        {
            right_edge_line[i]=MT9V03X_W-1;
        }
        else if(right_edge_line[i]<=0)
        {
            right_edge_line[i]=0;
        }
    }
}

// ??????
int Get_Road_Wide(int start_line,int end_line)
{
	  int i=0,t=0;
    int road_wide=0;
    if(start_line>=MT9V03X_H-1)
        start_line=MT9V03X_H-1;
    else if(start_line<=0)
        start_line=0;
    if(end_line>=MT9V03X_H-1)
        end_line=MT9V03X_H-1;
    else if(end_line<=0)
        end_line=0;
    
    if(start_line>end_line)//++??,??????
    {
        t=start_line;
        start_line=end_line;
        end_line=t;
    }
    for(i=start_line;i<=end_line;i++)
    {
        road_wide+=right_edge_line[i]-left_edge_line[i];
    }
    road_wide=road_wide/(end_line-start_line+1);//????
    return road_wide;
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     ?????
  @param     ??????,?????
  @return    null
  Sample     Stop_Detect(void)
  @note      ???????5??,????,????,?????
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Left_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=MT9V03X_H-1)//???????,?????????
        start=MT9V03X_H-1;
    else if(start<=0)
        start=0;
    if(end>=MT9V03X_H-1)
        end=MT9V03X_H-1;
    else if(end<=0)
        end=0;
    if(end<start)//++??,????
    {
        t=end;
        end=start;
        start=t;
    }

    if(start<=5)//???????????3??,?????????,?????,??????
    {
         Left_Add_Line(left_edge_line[start],start,left_edge_line[end],end);
    }

    else
    {
        k=(float)(left_edge_line[start]-left_edge_line[start-4])/5.0;//???k?1/??
        for(i=start;i<=end;i++)
        {
            left_edge_line[i]=(int)(i-start)*k+left_edge_line[start];//(x=(y-y1)*k+x1),?????
            if(left_edge_line[i]>=MT9V03X_W-1)
            {
                left_edge_line[i]=MT9V03X_W-1;
            }
            else if(left_edge_line[i]<=0)
            {
                left_edge_line[i]=0;
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ??????
  @param     ??????,?????
  @return    null
  Sample     Stop_Detect(void)
  @note      ???????3??,????,????,?????
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Right_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=MT9V03X_H-1)//???????,?????????
        start=MT9V03X_H-1;
    else if(start<=0)
        start=0;
    if(end>=MT9V03X_H-1)
        end=MT9V03X_H-1;
    else if(end<=0)
        end=0;
    if(end<start)//++??,????
    {
        t=end;
        end=start;
        start=t;
    }

    if(start<=5)//???????????3??,?????????,?????,??????
    {
        Right_Add_Line(right_edge_line[start],start,right_edge_line[end],end);
    }
    else
    {
        k=(float)(right_edge_line[start]-right_edge_line[start-4])/5.0;//???k?1/??
        for(i=start;i<=end;i++)
        {
            right_edge_line[i]=(int)(i-start)*k+right_edge_line[start];//(x=(y-y1)*k+x1),?????
            if(right_edge_line[i]>=MT9V03X_W-1)
            {
                right_edge_line[i]=MT9V03X_W-1;
            }
            else if(right_edge_line[i]<=0)
            {
                right_edge_line[i]=0;
            }
        }
    }
}

//???
void Left_Add_Line(int x1,int y1,int x2,int y2)//???,?????
{
    int i,max,a1,a2;
    int hx;
    if(x1>=MT9V03X_W-1)//???????,?????????
       x1=MT9V03X_W-1;
    else if(x1<=0)
        x1=0;
     if(y1>=MT9V03X_H-1)
        y1=MT9V03X_H-1;
     else if(y1<=0)
        y1=0;
     if(x2>=MT9V03X_W-1)
        x2=MT9V03X_W-1;
     else if(x2<=0)
             x2=0;
     if(y2>=MT9V03X_H-1)
        y2=MT9V03X_H-1;
     else if(y2<=0)
             y2=0;
    a1=y1;
    a2=y2;
    if(a1>a2)//????
    {
        max=a1;
        a1=a2;
        a2=max;
    }
    for(i=a1;i<=a2;i++)//????????
    {
        hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
        if(hx>=MT9V03X_W)
            hx=MT9V03X_W;
        else if(hx<=0)
            hx=0;
        left_edge_line[i]=hx;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ???
  @param     ?????,??
  @return    null
  Sample     Right_Add_Line(int x1,int y1,int x2,int y2);
  @note      ???????,?????????,????
-------------------------------------------------------------------------------------------------------------------*/
void Right_Add_Line(int x1,int y1,int x2,int y2)//???,?????
{
    int i,max,a1,a2;
    int hx;
    if(x1>=MT9V03X_W-1)//???????,?????????
       x1=MT9V03X_W-1;
    else if(x1<=0)
        x1=0;
    if(y1>=MT9V03X_H-1)
        y1=MT9V03X_H-1;
    else if(y1<=0)
        y1=0;
    if(x2>=MT9V03X_W-1)
        x2=MT9V03X_W-1;
    else if(x2<=0)
        x2=0;
    if(y2>=MT9V03X_H-1)
        y2=MT9V03X_H-1;
    else if(y2<=0)
         y2=0;
    a1=y1;
    a2=y2;
    if(a1>a2)//????
    {
        max=a1;
        a1=a2;
        a2=max;
    }
    for(i=a1;i<=a2;i++)//????????
    {
        hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
        if(hx>=MT9V03X_W)
            hx=MT9V03X_W;
        else if(hx<=0)
            hx=0;
        right_edge_line[i]=hx;
    }
}
//????
void Island_Detect()
{ 
    static float k=0;//3?5???k
    static int island_state_5_down[2]={0};//??5???????,????????,[0]?y,???,{1}?x,???
    static int island_state_3_up[2]={0};//??3????????,??????[0]?y,???,{1}?x,???
    int left_down_guai[2]={0};//?????????,[0]?y,???,{1}?x,???
    int right_down_guai[2]={0};//?????????,[0]?y,???,{1}?x,???
    int monotonicity_change_line[2];//????????,[0]???,[1]???
    int monotonicity_change_left_flag=0;//????0
    int monotonicity_change_right_flag=0;//????0
    int continuity_change_right_flag=0; //???0
    int continuity_change_left_flag=0;  //???0
//    int count_flag=0;
//    ips200_show_float(50,220,FJ_Angle);
//		printf("error = %d\r\n", Img_error);
//    printf("Island_State = %d\r\n", Island_State);
    //????????
    continuity_change_left_flag=Continuity_Change_Left(MT9V03X_H-10,10);//?????
    continuity_change_right_flag=Continuity_Change_Right(MT9V03X_H-10,10);
    monotonicity_change_right_flag=Monotonicity_Change_Right(MT9V03X_H-10,10);
    monotonicity_change_left_flag=Monotonicity_Change_Left(MT9V03X_H-10,10);
		
    
//    printf(" L_C=%d R_C=%d L_M=%d | R_M=%d\r\n",
//       continuity_change_left_flag,
//       continuity_change_right_flag,
//       monotonicity_change_left_flag,
//       monotonicity_change_right_flag);
//		   ips200_show_int16(0,260,continuity_change_left_flag);  // ????
//       ips200_show_int16(0,280,continuity_change_right_flag); // ????
//       ips200_show_int16(0,300,monotonicity_change_left_flag);
////       ips200_show_int16(60,300,monotonicity_change_right_flag);
//    //
    if(Straight_Flag==0&&Island_State==0)//&&Ramp_Flag==0
    {
//        continuity_change_left_flag=Continuity_Change_Left(MT9V03X_H-1-5,10);//?????              // ?
//        continuity_change_right_flag=Continuity_Change_Right(MT9V03X_H-1-5,10);
        if(Left_Island_Flag==0)//??
        {
            if(monotonicity_change_right_flag==0&& //??????
               continuity_change_left_flag!=0&& //???????
               continuity_change_right_flag==0&& //?????????
//			   monotonicity_change_left_flag!=0&&
               Left_Lost_Time>=20&& //??????
               Left_Lost_Time<=50&& //??????
               Right_Lost_Time<=10&&//??????
               Search_Stop_Line>=70&& //?????????
               Boundry_Start_Left>=MT9V03X_H-20&&
			         Boundry_Start_Right>=MT9V03X_H-20&& //???????
               Both_Lost_Time<=10
						   )//?????
						// ???????????
            {
                left_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);//?????
                if(left_down_guai[0]>=30)//??1??,???????,????,????,??
                {

                    Island_State=1;
                    Left_Island_Flag=1;
                }
                else//??,??
                {
                    Island_State=0;
                    Left_Island_Flag=0;
                }
            }
        }
        if(Right_Island_Flag==0)//??
        {
            if(monotonicity_change_left_flag==0&&
               continuity_change_left_flag==0&& //?????????
               continuity_change_right_flag!=0&& //???????
//			   monotonicity_change_right_flag!=0&&
               Right_Lost_Time>=20&&           //????
               Right_Lost_Time<=50&&           //???????
               Left_Lost_Time<=10&&            //????
               Search_Stop_Line>=70&& //?????????
               Boundry_Start_Left>=MT9V03X_H-20&&
			         Boundry_Start_Right>=MT9V03X_H-20&& //???????
               Both_Lost_Time<=10
						   )
            {
                right_down_guai[0]=Find_Right_Down_Point(MT9V03X_H-1,20);//???
                if(right_down_guai[0]>=30)//??1??,???????,????,????,??
                {
                    Island_State=1;
                    Right_Island_Flag=1;
                }
                else
                {
                    Island_State=0;
                    Right_Island_Flag=0;
                }
            }
        }
    }

    if(Left_Island_Flag==1)//1???????,???
    {
        if(Island_State==1)
        {
            monotonicity_change_line[0]=Continuity_Change_Left(30,2);//????????	
            monotonicity_change_line[1]=left_edge_line[monotonicity_change_line[0]];
					  if(monotonicity_change_line[1]<30)
						{
							h=1;
//							h=4;
//						}else{
//							Left_Add_Line((int)(monotonicity_change_line[1]*0.29),MT9V03X_H-1,monotonicity_change_line[1],monotonicity_change_line[0]);
						}
		        if(Boundry_Start_Left<40)
						{
					   if((continuity_change_right_flag!=0)&&(monotonicity_change_right_flag!=0))
						 {
							  Island_State=0;
							  Left_Island_Flag=0;
							  FJ_Angle=0;
						 }else{
							  Island_State=2;
//							  Beep=1;
//								system_delay_ms(50); // ??50ms
//							  Beep=0;
						 }
					 }	
        }
        else if(Island_State==2)//??????,2??????????,?????
        {
            monotonicity_change_line[0]=Continuity_Change_Left(70,5);//????????
            monotonicity_change_line[1]=left_edge_line[monotonicity_change_line[0]];
//					  ips200_show_int16(0,260,monotonicity_change_line[0]);  // ????
//            Left_Add_Line((int)(monotonicity_change_line[1]*0.1),MT9V03X_H-1,monotonicity_change_line[1],monotonicity_change_line[0]);
//					  h=2;
            if(monotonicity_change_line[0]>65)//???????,?3
            {
                Island_State=3;//???????????,?camera.c
//							  Beep=1;
//								system_delay_ms(50); // ??50ms
//							  Beep=0;
            }
        }
        else if(Island_State==3)//3??????,?????,??
        {
//					h=3;
            if(k!=0)
            {
//					 ips200_show_float(50,140,k);  
//						ips200_show_uint8(50,160,island_state_3_up[0]);
							K_Add_Boundry_Right(k,MT9V03X_W-10,MT9V03X_H-1,island_state_3_up[0]);//?????????????,???????????,??
            }
            else
            {
                Left_Up_Guai[0]=Find_Left_Up_Point(75,5);//?????
//							  ips200_show_uint8(50,100,Left_Up_Guai[0]);
                Left_Up_Guai[1]=left_edge_line[Left_Up_Guai[0]];
//							  ips200_show_uint8(50,120,Left_Up_Guai[1]);
                if(k==0&&(10<=Left_Up_Guai[0]&&Left_Up_Guai[0]<60)&&(20<Left_Up_Guai[1]&&Left_Up_Guai[1]<120))//??????????,???????
                {
                    island_state_3_up[0]= Left_Up_Guai[0];
                    island_state_3_up[1]= Left_Up_Guai[1];
//									  ips200_show_uint8(50,160,island_state_3_up[1]);
                    k=(float)((float)(MT9V03X_H-island_state_3_up[0])/(float)(MT9V03X_W-20-island_state_3_up[1]));
//									  ips200_show_float(50,180,k);  
									  K_Add_Boundry_Right(k,MT9V03X_W-10,MT9V03X_H-1,island_state_3_up[0]);//?????????????,???????????,??
//                }else{
//									h=12;
								}
            }
            if(abs(FJ_Angle)>=50) 
            {							
                k=0;//????
                Island_State=4;//???????????????????
//							  Beep=1;
//								system_delay_ms(50); // ??50ms
//							  Beep=0;
                Image_Pro_1();//????
            }
        }
        else if(Island_State==4)//??4?????   
        {
					
            if(abs(FJ_Angle)>200)//??200??????????
            {
                monotonicity_change_line[0]=Continuity_Change_Right(MT9V03X_H-10,10);//?????
//							  ips200_show_int16(0,260,monotonicity_change_line[0]);
                monotonicity_change_line[1]=right_edge_line[monotonicity_change_line[0]];
//							  ips200_show_int16(0,280,monotonicity_change_line[1]);
                if((Island_State==4)&&(35<=monotonicity_change_line[0]&&monotonicity_change_line[0]<=70&&monotonicity_change_line[1]>=10))//?????,??5
                {//monotonicity_change_line[1]>=90&&
                    island_state_5_down[0]=MT9V03X_H-5; 
                    island_state_5_down[1]=right_edge_line[MT9V03X_H-5];
									   
                    k=(float)((float)(MT9V03X_H-monotonicity_change_line[0])/(float)(island_state_5_down[1]-monotonicity_change_line[1]));
//									ips200_show_float(0,220,k);
//									ips200_show_uint16(0,240,island_state_5_down[1]);
//									ips200_show_uint16(0,260,island_state_5_down[0]);
                    K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],30);//???3??,????
                    Island_State=5;
//									   Beep=1;
//								     system_delay_ms(50); // ??50ms
//							       Beep=0;
                }
            }
        }
        else if(Island_State==5)//??
        {
//					  ips200_show_uint16(0, 270, add_line_counter);
//					   ips200_show_float(80,220,k);
//					   ips200_show_uint16(80,240,island_state_5_down[1]);
//						ips200_show_uint16(80,260,island_state_5_down[0]);
//            K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],0);
            if((Island_State==5)&&(Boundry_Start_Right<MT9V03X_H-10))//?????
            {
                Island_State=6;
//							  Beep=1;
//								system_delay_ms(50); // ??50ms
//							  Beep=0;
            }
        }
        else if(Island_State==6)//???
        {
            K_Add_Boundry_Right(k,island_state_5_down[1],island_state_5_down[0],0);
//														  ips200_show_float(50,180,k);
            if((Island_State==6)&&((Boundry_Start_Right>MT9V03X_H-10)||(abs(FJ_Angle)>=300)))//?????
            {
                k=0;
                Island_State=7;
//							  Beep=1;
//								system_delay_ms(50); // ??50ms
							  Beep=0;
            }
        }
        else if(Island_State==7)//?????,?????,????????
        {
            Left_Up_Guai[0]=Find_Left_Up_Point(MT9V03X_H-10,0);//???????,??????8
            Left_Up_Guai[1]=left_edge_line[Left_Up_Guai[0]];
            if((Island_State==7)&&(Left_Up_Guai[1]<=90)&&(5<=Left_Up_Guai[0]&&Left_Up_Guai[0]<=MT9V03X_H-20))//????,?????????
            {
                Island_State=8;//?????????8
//							  Beep=1;
//								system_delay_ms(50); // ??50ms
//							  Beep=0;
            }
        }
        else if(Island_State==8)//??,??????
        {
            Left_Up_Guai[0]=Find_Left_Up_Point(MT9V03X_H-1,10);//???????
            Left_Up_Guai[1]=left_edge_line[Left_Up_Guai[0]];
            if((Island_State==8)&&(Left_Up_Guai[0]>=MT9V03X_H-20||(Left_Up_Guai[0]<10&&Boundry_Start_Left>=MT9V03X_H-10)))//???????,?????,????
            {//??????,??????,??????,???????
                FJ_Angle=0;//????
                Island_State=0;//8?????????,????????,8???9,?????????,
//							  Beep=1;
//								system_delay_ms(50); // ??50ms
//							  Beep=0;
                Left_Island_Flag=0;
//							  h=5;
							  count_flag++;
            }
        }   
    }           
    else if(Right_Island_Flag==1)
    {
			
        if(Island_State==1)//1???????,???
        {
            monotonicity_change_line[0]=Continuity_Change_Right(30,2);//?????
            monotonicity_change_line[1]=right_edge_line[monotonicity_change_line[0]];
//					  ips200_show_uint16(0,260,monotonicity_change_line[0]);
//					  ips200_show_uint16(50,260,monotonicity_change_line[1]);
					if(monotonicity_change_line[1]<30)
						{
//					  h=2;
//						}else{
//						 Right_Add_Line((int)(monotonicity_change_line[1]*1.3), MT9V03X_H-1,monotonicity_change_line[1],monotonicity_change_line[0]);	   
//					          Beep=1;
//									  system_delay_ms(50); // ??50ms
//									  Beep=0;
						}
					 if(Boundry_Start_Right<40)
						{
					   if((continuity_change_left_flag!=0)&&(monotonicity_change_left_flag!=0))
						 {
							 Island_State=0;
							 Right_Island_Flag=0;
							 FJ_Angle=0;
						 }else{
							 Beep=1;
//							 system_delay_ms(50); // ??50ms
//							 Beep=0;
							 Island_State=2;
						 }
					 }
				 }
        else if(Island_State==2)//2??????,?????????
        {
            monotonicity_change_line[0]=Continuity_Change_Right(70,5);//?????
            monotonicity_change_line[1]=right_edge_line[monotonicity_change_line[0]];
//					  a = monotonicity_change_line[0];
////					  ips200_show_uint16(0,280,monotonicity_change_line[0]);
////					  ips200_show_uint16(50,260,monotonicity_change_line[1]);
//            Right_Add_Line((int)(monotonicity_change_line[1]*1.3),MT9V03X_H-1,monotonicity_change_line[1],monotonicity_change_line[0]); 
					if((monotonicity_change_line[0]>65)
//						  && right_edge_line[79] <= 179
//              && right_edge_line[77] <= 179
					  )//????????3
            {
//							  Beep=1;
//								system_delay_ms(50); // ??50ms
							  Beep=0;
                Island_State=3;//????,??????????
                Right_Island_Flag=1;
            }
        }
        else if(Island_State==3)//?????????,???????
        {
//					  ips200_show_uint16(0,300,k);
            if(k!=0)//????????,?????
            {
							  K_Add_Boundry_Left(k,10,MT9V03X_H-1,island_state_3_up[0]);//?????????????,???????????,??
//							  h=14;
							
            }
            else
            {
                Right_Up_Guai[0]=Find_Right_Up_Point(75,5);//?????
                Right_Up_Guai[1]=right_edge_line[Right_Up_Guai[0]];
                if(k==0&&(10<=Right_Up_Guai[0]&&Right_Up_Guai[0]<60)&&(40<Right_Up_Guai[1]&&Right_Up_Guai[1]<160))//???????????,??
                {
                    island_state_3_up[0]= Right_Up_Guai[0];
                    island_state_3_up[1]= Right_Up_Guai[1];
//									  ips200_show_uint8(0,260,island_state_3_up[0]);
//									  ips200_show_uint8(50,260,island_state_3_up[1]);
                    k=(float)((float)(MT9V03X_H-island_state_3_up[0])/(float)(20-island_state_3_up[1]));
					   K_Add_Boundry_Left(k,10,MT9V03X_H-1,island_state_3_up[0]);//?????????????,???????????,??
									a=k;
//                } else{
//									h=13;
								}
            }
						
            if(abs(FJ_Angle)>=50)  //????????
            {
//							  Beep=1;
//								system_delay_ms(50); // ??50ms
//							  Beep=0;
                k=0;      //????
                Island_State=4;
                Image_Pro();//????
            }//????????????,???????
        }
        else if(Island_State==4)//4?????????
        {
            if(abs(FJ_Angle)>=195)//????200???????????
            {
//							  Beep=1;
//								system_delay_ms(50); // ??50ms
//							  Beep=0;
                monotonicity_change_line[0]=Continuity_Change_Left(70,10);//?????
//							  ips200_show_int16(0,260,monotonicity_change_line[0]);
                monotonicity_change_line[1]=left_edge_line[monotonicity_change_line[0]];
//                ips200_show_int16(60,260,monotonicity_change_line[1]);
                if((Island_State==4)&&(35<=monotonicity_change_line[0]&&monotonicity_change_line[0]<=70&&monotonicity_change_line[1]<MT9V03X_W-10))//?????,??5
                {
                   island_state_5_down[0]=MT9V03X_H-5;
                   island_state_5_down[1]=left_edge_line[MT9V03X_H-1]-5;//??????????,??
                   k=(float)((float)(MT9V03X_H-monotonicity_change_line[0])/(float)(island_state_5_down[1]-monotonicity_change_line[1]));
                   K_Add_Boundry_Left(k,island_state_5_down[1],island_state_5_down[0],30);
                   Island_State=5;
//				   Beep=1;
//							   	system_delay_ms(50); // ??50ms
//							     Beep=0;
                }
            }
        }
        else if(Island_State==5)//?????
        {
//            K_Add_Boundry_Left(k,island_state_5_down[1],island_state_5_down[0],0);
            if(Island_State==5&&Boundry_Start_Left<MT9V03X_H-15)//?????
            {
//							  Beep=1;
//							  system_delay_ms(50); // ??50ms
//			    Beep=0;
                Island_State=6;
            }
        }
        else if(Island_State==6)//???
        {
            K_Add_Boundry_Left(k,island_state_5_down[1],island_state_5_down[0],0);
            if((Island_State==6)&&(Boundry_Start_Left>MT9V03X_H-10||abs(FJ_Angle)>=300))//?????
            {
                k=0;
//							  Beep=1;
//							  system_delay_ms(50); // ??50ms
//							  Beep=0;
                Island_State=7;
            }
        }
        else if(Island_State==7)//?????,???
        {
            Right_Up_Guai[0]=Find_Right_Up_Point(MT9V03X_H-10,0);//???????,????8
            Right_Up_Guai[1]=right_edge_line[Right_Up_Guai[0]];
            if((Island_State==7)&&((Right_Up_Guai[1]>=MT9V03X_W-90&&(5<=Right_Up_Guai[0]&&Right_Up_Guai[0]<=MT9V03X_H-20))))//????,?????????,???????,???????
            {//????????,?8
//							  Beep=1;
//							  system_delay_ms(50); // ??50ms
//							  Beep=1;
                Island_State=8;
            }
        }
        else if(Island_State==8)//??8
        {
            Right_Up_Guai[0]=Find_Right_Up_Point(MT9V03X_H-1,10);//???????
            Right_Up_Guai[1]=right_edge_line[Right_Up_Guai[0]];
//            Lengthen_Right_Boundry(Right_Up_Guai[0]-1,MT9V03X_H-1);
            if((Island_State==8)&&(Right_Up_Guai[0]>=MT9V03X_H-20||(Right_Up_Guai[0]<10&&Boundry_Start_Left>=MT9V03X_H-10)))//???????,?????,????
            {//????,???????,?????
//                Beep=0;
                FJ_Angle=0;
                Island_State=0;
                Right_Island_Flag=0;
							  count_flag++;
            }
        }
    }
}
