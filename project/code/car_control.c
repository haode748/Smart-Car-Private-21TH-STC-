//组    别：雁过留痕
//队    名：CWNU-探痕启航队
//作    者：钱镜慧
//创建时间：2025-11-24
// Car_Control.c
// Car_Control.c
#include "Car_Control.h"

// ------------------ 舵机占空比计算方式 ------------------
int32 Speed_L = 0;
int32 Speed_R = 0;
int32 PWM_L_set = 0;
int32 PWM_R_set = 0;
RS_Direct dir_l_state = Go;
RS_Direct dir_r_state = Go;
float groyz =0;
float FJ_Angle = 0;
float FJ_Pitch = 0;
float FJ_AngleSpeed = 0;
float FJ_PitchSpeed = 0;
float FJ_LastAngleSpeed = 0;
float gyro_offset_z=0;
// 零偏
int ZeroDrift_gyro_y = 0;
int ZeroDrift_gyro_z = 0;
// 编码器
int32 encoder_data_dir_1 = 0;
int32 encoder_data_dir_2 = 0;
int16 ticker_count = 0;

// 舵机
uint16 Median_servo = 89;
uint16 Servo_duty = 0;

// 车辆行进模式
//RS_CarMode Car_Mode = Lock;
RS_CarMode Car_Mode =Run ;
void Car_inital(void)
{
    gpio_init(Dir_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);  // GPIO 初始化为输出 默认上拉输出高
    gpio_init(Dir_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);  // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L, 17000, 0);                       // PWM 通道初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_R, 17000, 0);                       // PWM 通道初始化频率 17KHz 占空比初始为 0

// 编码器初始化
    encoder_dir_init(ENCODER_L,ENCODER_L_PULSE_PIN, ENCODER_L_DIR_PIN);   // 初始化左编码器
    encoder_dir_init(ENCODER_R,ENCODER_R_PULSE_PIN, ENCODER_R_DIR_PIN);   // 初始化右编码器
// 舵机初始化  
    pwm_init(SERVO_PWM, SERVO_FREQ, Median_servo);  
    pwm_set_duty(SERVO_PWM, (uint32)SERVO_DUTY(89));
//姿态传感器	IMU
    imu660rb_init();                                 
    gyro_get();         // 先读一次
    gyro_calibrate_z();   // 开机校准Z轴
	
	
	//无线串口初始化
 wireless_uart_init();


  //wushuadainji_init
//    gpio_init(IO_PA1,GPO,0,GPO_PUSH_PULL);
//    gpio_init(IO_PA3,GPO,0,GPO_PUSH_PULL);
	pwm_init(PWMF_CH1_PA1, 50, 0); 
    pwm_init(PWMF_CH2_PA3, 50, 0);

//	//RGB初始化  用于指示车辆巡线状态
	//蜂鸣器初始化
	gpio_init(IO_P45, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // P32引脚设置为推挽输出  引脚要改   
	Beep=0;//蜂鸣器关
	
//	gpio_init(IO_P64, GPO, GPIO_HIGH, GPO_PUSH_PULL);

	gpio_init(IO_P11, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(IO_P67, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_init(IO_P53, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_init(IO_P50, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_init(IO_P51, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(IO_P52, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(IO_P01, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(IO_P02, GPO, GPIO_HIGH, GPO_PUSH_PULL);         
              
//     JG=1;
	  JG1=0;
		JG2=0;
		JG3=0;
		JG4=0;
		JG5=0;
   	JG6=0;
		JG7=0;   
//	P11=P14=P15=0;//初始化RGB
test =0;
}

void encoder_get(void)
{
    // 获取编码器计数
    encoder_data_dir_1 = encoder_get_count(ENCODER_L);  // 获取左编码器计数
    encoder_data_dir_2 = -encoder_get_count(ENCODER_R);    // 获取右编码器计数
    
    Speed_L = encoder_data_dir_1;
    Speed_R =encoder_data_dir_2;
    
    // 清空编码器计数							每 5ms 就清零Q
    encoder_clear_count(ENCODER_L);
    encoder_clear_count(ENCODER_R);
    
    // 自定义PID输入
    Pid_vl.Measure = Speed_L;
    Pid_vr.Measure = Speed_R;
//	printf("l:%d\r\n",encoder_data_dir_1);
//	printf("r:%d\r\n",encoder_data_dir_2);
	
}

//激光
void JGD (void)
{
    if (laser_on) {
    laser_delay_count--;
    if (laser_delay_count == 0) {
        JG1 = JG2 = JG3 = JG4 = JG5 = JG6 = JG7 =0;   // 关激光
//			  JG=0;
        laser_on = 0;
    }
  }
}

void gyro_get(void)
{
	   imu660rb_get_acc();                                                         // 获取 imu660rb 的加速度测量数值  不用就注释掉，，
     imu660rb_get_gyro();                                                        // 获取 imu660rb 的角速度测量数值
	   groyz = imu660rb_gyro_transition(imu660rb_gyro_z);
}

// 陀螺仪Z轴零偏校准
void gyro_calibrate_z(void)									////////////////////////////////////////////////////////////////////////////
{
    int i;
    float sum = 0;

    // 连续采样200次求平均
    for(i=0; i<200; i++)
    {
        imu660rb_get_gyro();
        sum += imu660rb_gyro_transition(imu660rb_gyro_z);
        system_delay_ms(1);
    }

    gyro_offset_z = sum / 200.0f; // 得到零偏
    FJ_Angle = 0;                // 角度清零
}

// 计算车身转向角度（核心函数）
void get_car_angle(void)
{
	// 状态不等于0，才计算角度
	if(Island_State > 2)
	{
		
		if(Island_State == 8)
		{
			FJ_Angle = 0;  // 状态8直接归零，不再积分
		}
		else
		{
			// 1. 读取陀螺仪
			imu660rb_get_gyro();
			groyz = imu660rb_gyro_transition(imu660rb_gyro_z);

			// 2. 零偏校准
			groyz -= gyro_offset_z;

			// 3. 5ms 积分算角度
			FJ_Angle += groyz * 0.005f;

			// ======================
			// 4. 角度限幅
		if(FJ_Angle > 360)   FJ_Angle = 360;
		if(FJ_Angle < -360)  FJ_Angle = -360;
		}
	}
	else
	{
		// 状态=0 → 清零角度
		FJ_Angle = 0;
	}
}


void car_pwm_set(RS_Car_wheel car_wheel, RS_Direct car_dir, uint32 RS_PWM)
{
    switch (car_wheel)
    {
        // 左电机
        case L_wheel:
            pwm_set_duty(PWM_L, RS_PWM);
            if (car_dir == Go)
                dir_l_state = Go;
            else if (car_dir == Back)
                dir_l_state = Back;
            break;
        // 右电机    
        case R_wheel:
            pwm_set_duty(PWM_R, RS_PWM);
            if (car_dir == Go)
                dir_r_state = Go;
            else if (car_dir == Back)
                dir_r_state = Back;
            break;
        default:
            break;
    }
}

void car_control_n(int32 L, int32 R)  // 控制电机运动
{
    if (L > 0)
    {
        gpio_set_level(Dir_L, GPIO_HIGH);
        car_pwm_set(L_wheel, Go, L);
    }
    else
    {
        gpio_set_level(Dir_L, GPIO_LOW);
        car_pwm_set(L_wheel, Back, -L);
    }
    
    if (R > 0)
    {
        gpio_set_level(Dir_R, GPIO_HIGH);
        car_pwm_set(R_wheel, Go, R);
    }
    else
    {
        gpio_set_level(Dir_R, GPIO_LOW);
        car_pwm_set(R_wheel, Back, -R);
    }
}

void car_control(void)
{
    car_control_n(PWM_L_set, PWM_R_set);
}