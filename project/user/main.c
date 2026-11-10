/*********************************************************************************************************************
* STC32G144K Opensourec Library 即（STC32G144K 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2025 SEEKFREE 逐飞科技
*
* 本文件是STC32G144K开源库的一部分
*
* STC32G144K 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK FOR C251
* 适用平台          STC32G144K
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者           备注
* 2025-11-20        大W            first version
********************************************************************************************************************/
// main.c
#include "zf_common_headfile.h"
#include "hexin.h"
#include "wu_motor.h"

#define PIT_CH TIM0_PIT  // 定时器通道（去掉多余括号，避免宏解析错误）
#define PIT_MS 5       // 中断周期5ms
#define LED1                    (IO_P52 )

uint8 set_flage=0;
uint8 data_buffer[32];
uint8 data_len;
uint8  speed=0;
uint16 hServo_duty=0;

//uint8  new_speed=0;
////测试
//int p_;
//int d_;
extern uint8 speed;
//extern new_speed;
void pit_handler(void);
//测试
static uint16 count=0;
extern float p_;
extern float d_;
extern  uint16 hServo_duty;
extern float kd2;
void pit_handler(void);

extern int Island_State;     //环岛状态标志
extern int Left_Island_Flag; //左右环岛标志
extern int Right_Island_Flag;//左右环岛标志
extern float a;
extern RS_RUNCAR RUNCAR ;
void main(void)
{
    clock_init(SYSTEM_CLOCK_96M);  // 时钟配置及系统初始化<务必保留>
    debug_init();                  // 调试串口信息初始化 
	  mt9v03x_init();
		Car_inital();                  // 电机驱动初始（要用，先注释掉）
    PID_Start_init();							// 4 个 PID 的参数初始化
    pit_ms_init(PIT_CH, 5 , pit_handler);	// ★ 5ms 定时中断，执行 pit_handler			/////////////////////////////////////////
    ips200_init();    
		Key_init();
		gpio_init(LED1, GPO, 0, GPO_PUSH_PULL);
    
//    Wu_motor_Init();
    

//    pit_ms_init(TIM1_PIT, 1 ,Wu_motor_Init); 
//      pwm_set_duty(SERVO_PWM, (uint32)SERVO_DUTY(89));

	
	
//	
//	   pwm_init(SERVO_PWM, SERVO_FREQ, Median_servo);  // 舵机初始化
//    pwm_set_duty(SERVO_PWM, (uint32)SERVO_DUTY(88));//test
// while(1)
//    {
//        system_delay_ms(100);   
//        if(mt9v03x_init())
////				 printf("111");
//        else
//            break;
//		gpio_toggle_level(LED1);                                 // 短延时快速闪灯表示异常
//    }
////    printf("222");
//   
				
//中断（编码器、PID、电机输出）主循环（图像处理、屏幕刷）
    while (1)
    {
			if(mt9v03x_finish_flag)
			{ 
//				test =1;
//				printf("333");
		ips200_displayimage03x(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);   //屏幕图像显示,修改二值化显示        调图像
			Image_Pro();
//			Screen_Show_Edge_Line();	
      mt9v03x_finish_flag=0;	
//				  test =0;
//				if(Island_State==3){data_len=1;}
			}
			
//			
						Key_down();
		  Screen_ips200() ;			// 刷新屏幕

//			printf("a: %f\n",a);
//			printf("Left_Island_Flag: %d,Right_Island_Flag: %d,Island_State: %d\n",Left_Island_Flag,Right_Island_Flag,Island_State);
//			printf("right_edge_line[79]: %d,right_edge_line[77]: %d\n",right_edge_line[79],right_edge_line[79]);
//			printf("%d\n",Img_error);
			
			
			data_len = wireless_uart_read_buffer(data_buffer, 32);
			if(data_len != 0 && data_buffer[0] == 's')
			{
//				wireless_uart_send_buffer((const uint8 *)"s", 1); 
	//    RUNCAR = none_run_car;
			Car_Mode =Lock;
			// 立刻刹车：清零 PID 和 PWM 输出，不等增量式衰减
			PWM_L_set = 0;
			PWM_R_set = 0;
			Pid_vl.Output       = 0;
			Pid_vr.Output       = 0;
			Pid_vl.Last_Err     = 0;
			Pid_vr.Last_Err     = 0;
			Pid_vl.Previous_Err = 0;
			Pid_vr.Previous_Err = 0;  
			}

//			if(count_flag==2&&bullseye_flag==1 )// 完成任务（环岛2次+靶心
//			{
////    RUNCAR = none_run_car;
//			Car_Mode =Lock;										// → 停车
//       }
    }
}
void pit_handler(void) 
{
	   
// 1定义所有局部变量
    char buf[32];
    char buff[32];
	  
    float real_angle_deg;
    float angle_rad;
    float abs_angle;
	
//	test =0;
//	   if(count < 250)
//{
//    count++;
//    start_flag = 1;   // 前3000：开启
//	  
//}
//else
//{
////    count = 0;        // 满3000：清零！
//    start_flag = 0;   // 关闭
//}

    if(bullseye_flag==1)
         {
             JGD();
         }
	

// 2. 获取传感器数据
    encoder_get();
    gyro_get();
    get_car_angle();

//    // 3. 方向环（舵机）
//    if(Img_error > 10 || Img_error < -10) {kd2 = 0;}
//		else{kd2  = 0.022;}
    car_dir = Postion_P2ID_Cal(&Pid_turn, Img_error-11, groyz, kp2, kd2);
				 
				 // ★ 新增：直道死区，小误差不动舵机
if (abs(Img_error - 11) < 3)
{
    car_dir = 0;
}
				 
    hServo_duty = Median_servo + car_dir;
    Servo_duty = Clip(hServo_duty, Min_servo, Max_servo);
    pwm_set_duty(SERVO_PWM, (uint32)SERVO_DUTY(Servo_duty));

    // 4. 计算实际角度
real_angle_deg = ((float)Servo_duty - (float)Median_servo) * (90.0f / (float)(Max_servo - Min_servo));
angle_rad = real_angle_deg * 3.1415926f / 180.0f;

//    // 5. 速度环差速
//if ( set_flage == 1 ) {
//    speed = 20;
//    vl_ex = Pid_vl.Target + speed;
//    vr_ex = Pid_vl.Target + speed;
//	
//} else {
//    // 死区：绝对值小于 3 度时，视为直行，不加差速
//    if (fabs(real_angle_deg) < 3.0f) {
//        vl_ex = vr_ex = Pid_vl.Target;
//    } else if (real_angle_deg > 0) { // 左转
//        vl_ex = Pid_vl.Target * (1.0f - ((BORDWIDTH * tan(angle_rad)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kp);//减速
//        vr_ex = Pid_vl.Target * (1.0f + ((BORDWIDTH * tan(angle_rad)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kd);//加速
//    } else if (real_angle_deg <0) { // 右转
//        abs_angle = -angle_rad;
//        vl_ex = Pid_vl.Target * (1.0f + ((BORDWIDTH * tan(abs_angle)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kd);//加速
//        vr_ex = Pid_vl.Target * (1.0f - ((BORDWIDTH * tan(abs_angle)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kp);//减速
//    }else  {vl_ex = vr_ex = Pid_vl.Target;}
//	}
//    if (Car_Mode == Lock) vl_ex = vr_ex = 0;
//    PWM_L_set = IncrementalPID_Calculate(&Pid_vl, vl_ex);
//    PWM_R_set = IncrementalPID_Calculate(&Pid_vr, vr_ex);

//    // 5. 速度环差速
//if (Car_Mode == Lock) {vl_ex = vr_ex = 0;
// PWM_L_set = IncrementalPID_Calculate(&Pid_vl, vl_ex);
//    PWM_R_set = IncrementalPID_Calculate(&Pid_vr, vr_ex);}

    // 5. 速度环差速
//	   if (Car_Mode == Lock){ vl_ex = vr_ex = 0;}
//if ( start_flag == 1 ) {
//    speed = 0;
//    vl_ex = Pid_vl.Target - speed;
//    vr_ex = Pid_vl.Target - speed;
//	
//} else if (fabs(real_angle_deg) < 3.0f)// 死区：绝对值小于 3 度时，视为直行，不加差速
//{
//        vl_ex = vr_ex = Pid_vl.Target;
//    } else if (real_angle_deg > 0) { // 左转
//        vl_ex = Pid_vl.Target * (1.0f - ((BORDWIDTH * tan(angle_rad)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kp);//减速
//        vr_ex = Pid_vl.Target * (1.0f + ((BORDWIDTH * tan(angle_rad)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kd);//加速
//    } else if (real_angle_deg <0) { // 右转
//        abs_angle = -angle_rad;
//        vl_ex = Pid_vl.Target * (1.0f + ((BORDWIDTH * tan(abs_angle)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kd);//加速
//        vr_ex = Pid_vl.Target * (1.0f - ((BORDWIDTH * tan(abs_angle)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kp);//减速
//    }else  {vl_ex = vr_ex = Pid_vl.Target;}
////	}

//    PWM_L_set = IncrementalPID_Calculate(&Pid_vl, vl_ex);
//    PWM_R_set = IncrementalPID_Calculate(&Pid_vr, vr_ex);

//    // 5. 速度环差速
//if (Car_Mode == Lock) {vl_ex = vr_ex = 0;
// PWM_L_set = IncrementalPID_Calculate(&Pid_vl, vl_ex);
//    PWM_R_set = IncrementalPID_Calculate(&Pid_vr, vr_ex);}
////    // 5. 速度环差速
//else if ( start_flag ==1 ) {
// PWM_L_set =50 ;//要具体打印一下
// PWM_R_set=50 ;

////	
//} else if( fabs(real_angle_deg) < 3.0f)  // 死区：绝对值小于 3 度时，视为直行，不加差速
//{
//        vl_ex = vr_ex = Pid_vl.Target;
//			    PWM_L_set = IncrementalPID_Calculate(&Pid_vl, vl_ex);
//    PWM_R_set = IncrementalPID_Calculate(&Pid_vr, vr_ex);
//    } else if (real_angle_deg > 0) { // 左转
//        vl_ex = Pid_vl.Target * (1.0f - ((BORDWIDTH * tan(angle_rad)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kp);//减速
//        vr_ex = Pid_vl.Target * (1.0f + ((BORDWIDTH * tan(angle_rad)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kd);//加速
//			    PWM_L_set = IncrementalPID_Calculate(&Pid_vl, vl_ex);
//    PWM_R_set = IncrementalPID_Calculate(&Pid_vr, vr_ex);
//    } else if (real_angle_deg <0) { // 右转
//        abs_angle = -angle_rad;
//        vl_ex = Pid_vl.Target * (1.0f + ((BORDWIDTH * tan(abs_angle)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kd);//加速
//        vr_ex = Pid_vl.Target * (1.0f - ((BORDWIDTH * tan(abs_angle)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kp);//减速
//			    PWM_L_set = IncrementalPID_Calculate(&Pid_vl, vl_ex);
//    PWM_R_set = IncrementalPID_Calculate(&Pid_vr, vr_ex);
//    }else  {vl_ex = vr_ex = Pid_vl.Target;
//			    PWM_L_set = IncrementalPID_Calculate(&Pid_vl, vl_ex);
//    PWM_R_set = IncrementalPID_Calculate(&Pid_vr, vr_ex);
//		}
	
    
    // 5. 速度环差速
//	  if (RUNCAR == run_car) {	


if (flage == 1) {
    speed = 20;
    vl_ex = Pid_vl.Target + speed;
    vr_ex = Pid_vl.Target + speed;
} else {
    // 死区：绝对值小于 3 度时，视为直行，不加差速
    if (fabs(real_angle_deg) < 3.0f) {
        vl_ex = vr_ex = Pid_vl.Target;
    } else if (real_angle_deg > 0) { // 左转
        vl_ex = Pid_vl.Target * (1.0f - ((BORDWIDTH * tan(angle_rad)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kp);//减速
        vr_ex = Pid_vl.Target * (1.0f + ((BORDWIDTH * tan(angle_rad)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kd);//加速
    } else if (real_angle_deg <0) { // 右转
        abs_angle = -angle_rad;
        vl_ex = Pid_vl.Target * (1.0f + ((BORDWIDTH * tan(abs_angle)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kd);//加速
        vr_ex = Pid_vl.Target * (1.0f - ((BORDWIDTH * tan(abs_angle)) / (2.0f * LDISTANCE)) * Pid_wheel_turn.Kp);//减速
    }else  {vl_ex = vr_ex = Pid_vl.Target;}
	}

	
	
//    if (Car_Mode == none_run_car) vl_ex = vr_ex = 0;
	if (Car_Mode == Lock) vl_ex = vr_ex = 0;
	
	
	
    PWM_L_set = IncrementalPID_Calculate(&Pid_vl, vl_ex);
    PWM_R_set = IncrementalPID_Calculate(&Pid_vr, vr_ex);
	
	
//}else{
// PWM_L_set =0;
//	 PWM_R_set=0;
//}
	
	
	
	
//  	printf("%d\r\n",bullseye_center_x);
    // 打印调试信息
//sprintf(buf, " duty=%d\r\n", Img_error);
//wireless_uart_send_buffer((const uint8*)buf, strlen(buf));
//sprintf(buf, " x=%f\r\n",FJ_Angle);
//wireless_uart_send_buffer((const uint8*)buf, strlen(buf));
//    sprintf(buf, " y= %d\r\n",bullseye_center_y  );
//    wireless_uart_send_buffer((const uint8*)buf, strlen(buf));
//    sprintf(buf, "  b=%d\r\n",  bullseye_center_x );
//    wireless_uart_send_buffer((const uint8*)buf, strlen(buf));
//	sprintf(buf, " Continuity_Change_Right(70,5)= %d\r\n", Continuity_Change_Right(70,5) );
//	wireless_uart_send_buffer((const uint8*)buf, strlen(buf));
//    sprintf(buf, "  w=%d\r\n", bullseye_center_x);
//    wireless_uart_send_buffer((const uint8*)buf, strlen(buf));
	//    sprintf(buf, "  w=%d\r\n",bullseye_center_y );
//    wireless_uart_send_buffer((const uint8*)buf, strlen(buf));
//    if(Island_State==5){
//	   sprintf(buf, " zs= %d\r\n", Boundry_Start_Right );
//    wireless_uart_send_buffer((const uint8*)buf, strlen(buf));}
//    sprintf(buf, "  Island_State=%d\r\n",  Island_State);
//    wireless_uart_send_buffer((const uint8*)buf, strlen(buf));

//	printf("%d,%d\n", Speed_L, vl_ex);
//	printf("lzs:%d\r\n",Speed_L);
//	printf("lmub:%d\r\n",vl_ex);





    car_control();
	
//     test =1;
}