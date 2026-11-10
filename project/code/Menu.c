

#include "Menu.h"


//屏幕显示集成
RS_Screen_mode Screen_mode = Image;
RS_data_mode   data_mode ;

uint8 Close_flag=0;
uint16 Arrow_point = 0;



//数据显示
void show_edge_data(int x, int y, const char* label, void* value, RS_data_mode data_mode) 
{
    uint8 i = 40; // 固定偏移量，避免重叠
    ips200_show_string(x, y, label);

    switch (data_mode) 
    {
        case int_:
            ips200_show_int32(x + i, y, *((int*)value), 3);
            break;
        case float_:
            ips200_show_float(x + i, y, *((float*)value), 3, 3);
            break;
        default:
            break;
    }
}


	

////图像界面
//void Screen_Image(void)
//{
//	ips200_show_string(100, 0, "Image:");
//	
//	
//	ips200_show_wave(40, 64, (uint8 *)&tsl1401_data[0][0], 128, 1500, 128, 64);
//	
//	ips200_show_wave(40, 128, (uint8 *)&tsl1401_data[1][0], 128, 1500, 128, 64);
//	
//	//显示边线数据
//		ips200_show_string(0, 200, "Left1:");
//		ips200_show_int32(50, 200, image_1.line_left, 3);  
//		ips200_show_string(110, 200, "Right1:");
//		ips200_show_int32(170, 200, image_1.line_right, 3);  
//	
//	  switch (image_1.left_state)
//		{
//			case exist: ips200_show_string(80, 200, "(E)");break;
//			case lost: ips200_show_string(80, 200, "(L)");break;
//		}
//		switch (image_1.right_state)
//		{
//			case exist: ips200_show_string(200, 200, "(E)");break;
//			case lost: ips200_show_string(200, 200, "(L)");break;
//		}
//	
//		ips200_show_string(0, 220, "Left2:");
//		ips200_show_int32(50, 220, image_2.line_left,3);  
//		ips200_show_string(110, 220, "Right2:");
//		ips200_show_int32(170, 220, image_2.line_right, 3);  
//		
//		switch (image_2.left_state)
//		{
//			case exist: ips200_show_string(80, 220, "(E)");break;
//			case lost: ips200_show_string(80, 220, "(L)");break;
//		}
//		switch (image_2.right_state)
//		{
//			case exist: ips200_show_string(200, 220, "(E)");break;
//			case lost: ips200_show_string(200, 220, "(L)");break;
//		}

//		ips200_show_string(0, 240, "W1:");
//		ips200_show_int32(35, 240, image_1.Patch_width,3);  
//		
//		ips200_show_string(80, 240, "W2:");
//		ips200_show_int32(115, 240, image_2.Patch_width, 3); 
//		
//		ips200_show_string(160, 240, "W_c:");
//		ips200_show_int32(200, 240, RS_width_cacul, 3);  
//		
//		ips200_show_string(0, 260, "L-C:");
//		ips200_show_int32(50, 260, RS_left_cacul,3);  
//		ips200_show_string(110, 260, "R-C:");
//		ips200_show_int32(170, 260, RS_right_cacul, 3); 
//		
////		ips200_show_string(0, 280, "CL_N:");
////		ips200_show_int(50, 280, left_none, 3); 
////		
////	  ips200_show_string(110, 280, "CR_N:");
////		ips200_show_int(170, 280, right_none, 3);  
//		
////		ips200_show_string(0, 300, "CL:");
////		ips200_show_int(50, 300, left_swell,3);  
////		ips200_show_string(110, 300, "CR:");
////		ips200_show_int(170, 300, right_swell, 3); 
//}


////参数显示界面
//void Screen_Para(void)
//{
//	ips200_show_string(100, 0, "Para:");//参数
//	
//	ips200_show_string(0, 20, "S_L:");
//	ips200_show_string(120, 20, "S_R:");
//	
//	ips200_show_float(55, 20, Speed_L, 2, 3);
//  ips200_show_float(175, 20, Speed_R, 2, 3);
//	
//	ips200_show_string(0, 40, "imu_g_z:");
//	ips200_show_int32(80, 40, imu660ra_gyro_z, 4);
//	
//}



//调显示界面
void para_tuning_dis(uint16 arrow_point) {
    uint16 i = 0;
    // 清除旧的箭头
    for (i = 0; i <= 240; i += 20) {
        if (i != arrow_point)
            ips200_show_string(10, i, "  ");
        else
            ips200_show_string(10, arrow_point, "->");
    }

    // 显示所有参数（原 show_edge_data 部分保持不变）
		////期望
show_edge_data(25 ,0 , "Lex:" , &Pid_vl.Target ,int_);
show_edge_data(130 ,0 , "Rex:" , &Pid_vr.Target , int_);
//左轮速度环
show_edge_data(25 ,20 , "Lpkp:" ,&Pid_vl.Kp , float_);
show_edge_data(25 ,40 , "Lpkd:" , &Pid_vl.Kd, float_);
//右轮速度环
show_edge_data(25 ,60 , "Rpkp:" , &Pid_vr.Kp, float_);
show_edge_data(25 ,80 , "Rpkd:" , &Pid_vr.Kd, float_);
//舵机转向环
show_edge_data(25 ,100 , "tkp:" ,&Pid_turn.Kp, float_);
show_edge_data(25 ,120 , "tkd:" ,&Pid_turn.Kd, float_);
//差速转向环
show_edge_data(25 ,140 , "wtkp:" ,&Pid_wheel_turn.Kp, float_);
show_edge_data(25 ,160 , "wtkd:" ,&Pid_wheel_turn.Kd, float_);
show_edge_data(25 ,180 , "kd2:", &kd2,float_);	
show_edge_data(25 ,200 , "kp2:" ,&kp2, float_);
show_edge_data(25 ,220 , "num::" ,&step_num, float_);
show_edge_data(25 ,240 , "step::" ,&float_step, float_);
//    show_edge_data(25, 0, "Lex:", &Pid_vl.Target, int_);
//    show_edge_data(130, 0, "Rex:", &Pid_vr.Target, int_);
//    show_edge_data(25, 20, "Lpkp:", &Pid_vl.Kp, float_);
//    show_edge_data(25, 40, "Lpki:", &Pid_vl.Ki, float_);
//    show_edge_data(25, 60, "Lpkd:", &Pid_vl.Kd, float_);
//    show_edge_data(25, 80, "Rpkp:", &Pid_vr.Kp, float_);
//    show_edge_data(25, 100, "Rpki:", &Pid_vr.Ki, float_);
//    show_edge_data(25, 120, "Rpkd:", &Pid_vr.Kd, float_);
//    show_edge_data(25, 140, "tkp:", &Pid_turn.Kp, float_);
//    show_edge_data(25, 160, "tkd:", &Pid_turn.Kd, float_);
//    show_edge_data(25, 180, "wtkp:", &Pid_wheel_turn.Kp, float_);
//    show_edge_data(25, 200, "wtkd:", &Pid_wheel_turn.Kd, float_);
//    show_edge_data(25, 220, "num:", &step_num, float_);
//    show_edge_data(25, 240, "step:", &float_step, float_);
}

//}


//控制界面
//void Screen_Car_mode(void)
//{
//	//ips200_clear();  
//	
//	ips200_show_string(90, 0, "Car_mode:"); 
//	
//	//循线模式
//	ips200_show_string(0, 20, "Patrol_mode:");
//	ips200_show_int32(100, 20, Patrol_mode, 3);  
//	
//	switch (Patrol_mode)
//	{
//		case Near:	ips200_show_string(120, 20, "Near");break;
//		case Far:	  ips200_show_string(120, 20, "Far");break;
//	}
//	
//	//循线模式
//	ips200_show_string(0, 40, "Travel_mode:");

//	ips200_show_int32(100, 40, Travel_mode, 3);
//	switch (Travel_mode)
//	{
//		case General:	ips200_show_string(120, 40, "General");break;
//		case Left_line:	  ips200_show_string(120, 40, "Left_line");break;
//		case Right_line:	ips200_show_string(120, 40, "Right_line");break;
//	}
//	
	//
//	ips200_show_string(0, 60, "Cycle_state:");
//	ips200_show_uint(100, 60, Cycle_state, 3);  
//	
//		switch (Cycle_state)
//	{
//	  case L_start:	ips200_show_string(120, 800, "L_st");break;
//    case L_in:	ips200_show_string(120, 800, "L_in");break;
//		case L_run:	  ips200_show_string(120, 80, "L_RUN");break;
//		case L_end:	  ips200_show_string(120, 80, "L_end");break;
//		case L_out:	  ips200_show_string(120, 80, "L_out");break;
//  	case R_start:	ips200_show_string(120, 80, "R_st");break;
//		case R_in:	ips200_show_string(120, 80, "R_in");break;
//		case R_run:	  ips200_show_string(120, 80, "R_RUN");break;
//		case R_end:	  ips200_show_string(120, 80, "R_end");break;
//		case R_out:	ips200_show_string(120, 80, "R_out");break;
//		case Cycle_none:	  ips200_show_string(120, 80, "Cycle_none");break;
//			default : break;
//	}
//}


//void test_show_edge_data(void) {
//    float test_kp = 1.25;
//    
//    // 测试1：简单的标签
//    show_edge_data(25, 80, "KP:", &test_kp, float_);
//    test_kp = 1.25;
//    // 测试2：短标签
//    show_edge_data(25, 100, "Kp:", &test_kp, float_);
//    test_kp = 1.25;
//    // 测试3：原标签
//    show_edge_data(25, 120, "R_Speed_kp:", &test_kp, float_);
//}





//屏幕显示
void Screen_ips200(void) {
    // 如果屏幕已被禁用，直接返回，不进行任何显示操作
    if (lcd_enabled == 0) {
        return;
    }
    
    // 原有的显示逻辑（根据 Screen_mode 显示不同界面）
    switch(Screen_mode) {
        case Image:
				 ips200_displayimage03x(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);   //屏幕图像显示
        Screen_Show_Edge_Line();
            break;
        case para_tun:
            para_tuning_dis(Arrow_point);
            break;
        default:
            break;
    }
}