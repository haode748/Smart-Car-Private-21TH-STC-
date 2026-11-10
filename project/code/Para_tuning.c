#include "Para_tuning.h"
uint16 Arrow_point_old;
PID_Const constPid_wheel_turn;
PID_Const constPid_turn;
PID_Const constPid_vr;
PID_Const constPid_vl;
int temp_change = 0;
int temp_change_old = 0;//加的
uint8 lcd_enabled = 1;   // 默认屏幕启用
static uint8 display_stage = 0;  // 0:图像, 1:调参, 2:熄屏

static uint8 key3_pressed = 0;
static uint8 key4_pressed = 0;


// 只保存调参所需的常数
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    int32 Target;
} PID_Save_Compact;

// 完整保存数据块
typedef struct {
    uint16 magic;                  // 固定魔数 0xA5A5
    PID_Save_Compact pid_vl;
    PID_Save_Compact pid_vr;
    PID_Save_Compact pid_turn;
    PID_Save_Compact pid_wheel_turn;
    float step_num;                  // 调参步进值
    float float_step;                // 步长
		float kp2;                        //kp2
		float kd2;
    uint16 checksum;               // 校验和
} SavedParams;
//typedef unsigned char      uint8;
//typedef unsigned int       uint16;
//typedef unsigned long      uint32;
//typedef signed char        int8;
//typedef signed int         int16;
//typedef signed long        int32;
//编码器暂存值
float  step_num=0;
int8  point_num=0;
int8  change_num=0;

uint16 run_car_num = 0;  

float  float_step=0.1;

RS_RUNCAR RUNCAR = none_run_car;

//按键初始化
void Key_init(void)
{
//	tsl1401_finish_flag= 1;
//gpio_init(IO_P70, GPI, 0, GPI_PULL_UP);
//gpio_init(IO_P71, GPI, 0, GPI_PULL_UP);
//gpio_init(IO_P72, GPI, 0, GPI_PULL_UP);
//gpio_init(IO_P73, GPI, 0, GPI_PULL_UP);
	

	gpio_init(KEY1_PIN, GPI, 1, GPI_PULL_UP);
	gpio_init(KEY2_PIN, GPI, 1, GPI_PULL_UP);
	gpio_init(KEY3_PIN, GPI, 1, GPI_PULL_UP);
	gpio_init(KEY4_PIN, GPI, 1, GPI_PULL_UP);
	gpio_init(KEY5_PIN, GPI, 1, GPI_PULL_UP);
	

}
uint8 is_auto_save_enabled(void) {
    // 拨码开关闭合（ON）时引脚为低电平（0），所以取反
    return (gpio_get_level(KEY5_PIN) == 0) ? 1 : 0;
}
//按键控制 如果小车正在运行(run_car状态)，计数器递增当运行500次循环后，自动停止小车
// 辅助函数：阻塞检测长按（ms为长按时间阈值）
static uint8 is_long_press(uint8 pin, uint32 ms) {
    uint32 cnt = 0;
    while (gpio_get_level(pin) == 0) {
        system_delay_ms(1);
        if (++cnt >= ms) return 1;
    }
    return 0;
}


static uint16 calc_checksum(SavedParams *p) {
    uint16 sum = 0;
    uint8 *ptr = (uint8*)p;
    uint16 i;
    for (i = 0; i < sizeof(SavedParams) - sizeof(p->checksum); i++) {
        sum += ptr[i];
    }
    return sum;
}

// 选择 EEPROM 起始地址（建议使用最后一页，此处以 0x0E00 为例，您可根据实际芯片调整）

void save_pid_params_to_flash(void) {
    SavedParams params;
    
    // 1. 填充魔数
    params.magic = 0xA5A5;
    
    // 2. 填充左轮速度环参数
    params.pid_vl.Kp = Pid_vl.Kp;
    params.pid_vl.Ki = Pid_vl.Ki;
    params.pid_vl.Kd = Pid_vl.Kd;
    params.pid_vl.Target = Pid_vl.Target;
    
    // 3. 填充右轮速度环参数
    params.pid_vr.Kp = Pid_vr.Kp;
    params.pid_vr.Ki = Pid_vr.Ki;
    params.pid_vr.Kd = Pid_vr.Kd;
    params.pid_vr.Target = Pid_vr.Target;
    
    // 4. 填充转向环参数
    params.pid_turn.Kp = Pid_turn.Kp;
    params.pid_turn.Ki = Pid_turn.Ki;
    params.pid_turn.Kd = Pid_turn.Kd;
    params.pid_turn.Target = Pid_turn.Target;
    
    // 5. 填充差速转向环参数
    params.pid_wheel_turn.Kp = Pid_wheel_turn.Kp;
    params.pid_wheel_turn.Ki = Pid_wheel_turn.Ki;
    params.pid_wheel_turn.Kd = Pid_wheel_turn.Kd;
    params.pid_wheel_turn.Target = Pid_wheel_turn.Target;
    
    // 6. 步长变量
    params.step_num = step_num;
    params.float_step = float_step;
		params.kp2=kp2;
		params.kd2=kd2;
    // 7. 计算校验和并填充
    params.checksum = calc_checksum(&params);
    
    // 8. 写入 EEPROM（自动擦除整页，512字节）
    extern_iap_write_buff(PARAM_SAVE_ADDR, (uint8*)&params, sizeof(SavedParams));
    
}

void load_pid_params_from_flash(void) {
    SavedParams params;
    
    // 1. 从 EEPROM 读取数据
    iap_read_buff(PARAM_SAVE_ADDR, (uint8*)&params, sizeof(SavedParams));
    
    // 2. 验证魔数和校验和
    if (params.magic == 0xA5A5 && params.checksum == calc_checksum(&params)) {
        // 数据有效，恢复参数
        Pid_vl.Kp = params.pid_vl.Kp;
        Pid_vl.Ki = params.pid_vl.Ki;
        Pid_vl.Kd = params.pid_vl.Kd;
        Pid_vl.Target = params.pid_vl.Target;
        
        Pid_vr.Kp = params.pid_vr.Kp;
        Pid_vr.Ki = params.pid_vr.Ki;
        Pid_vr.Kd = params.pid_vr.Kd;
        Pid_vr.Target = params.pid_vr.Target;
        
        Pid_turn.Kp = params.pid_turn.Kp;
        Pid_turn.Ki = params.pid_turn.Ki;
        Pid_turn.Kd = params.pid_turn.Kd;
        Pid_turn.Target = params.pid_turn.Target;
        
        Pid_wheel_turn.Kp = params.pid_wheel_turn.Kp;
        Pid_wheel_turn.Ki = params.pid_wheel_turn.Ki;
        Pid_wheel_turn.Kd = params.pid_wheel_turn.Kd;
        Pid_wheel_turn.Target = params.pid_wheel_turn.Target;
        
        step_num = params.step_num;
        float_step = params.float_step;
				kp2= params.kp2;
				kd2= params.kd2;
    } else {

        // 可选：自动将当前默认参数保存一次，便于后续使用
         save_pid_params_to_flash();
    }
}

// 按键控制主函数
// 按键控制主函数
void Key_down(void) {
    // ========== KEY1：切换显示模式（图像→调参→熄屏） ==========
    if (gpio_get_level(KEY1_PIN) == 0) {
        system_delay_ms(20);
        if (gpio_get_level(KEY1_PIN) == 0) {
            if (RUNCAR == run_car) {
                while (gpio_get_level(KEY1_PIN) == 0);
            } else {
                display_stage++;
                if (display_stage > 2) display_stage = 0;
                switch (display_stage) {
                    case 0: // 图像
                        lcd_enabled = 1;
                        IPS200_BLK(1);
                        Screen_mode = Image;
                        ips200_clear(RGB565_WHITE);
                        break;
                    case 1: // 调参
                        lcd_enabled = 1;
                        IPS200_BLK(1);
                        Screen_mode = para_tun;
                        ips200_clear(RGB565_WHITE);
                        break;
                    case 2: // 熄屏
                        lcd_enabled = 0;
                        IPS200_BLK(0);
                        break;
                }
                while (gpio_get_level(KEY1_PIN) == 0);
            }
        }
    }

    // ========== KEY2：发车（同时熄屏） ==========
    if (gpio_get_level(KEY2_PIN) == 0) {
        system_delay_ms(20);
        if (gpio_get_level(KEY2_PIN) == 0) {
            lcd_enabled = 0;
            IPS200_BLK(0);
            RUNCAR = run_car;
            run_car_num = 0;
            while (gpio_get_level(KEY2_PIN) == 0);
        }
    }

    // ========== KEY3：上移 / 增加值 ==========
    if (gpio_get_level(KEY3_PIN) == 0) {
        if (!key3_pressed) {
            key3_pressed = 1;
            system_delay_ms(20);
            if (gpio_get_level(KEY3_PIN) == 0) {
                if (is_long_press(KEY3_PIN, 300)) {
                    // 长按增加参数
                    if (Screen_mode == para_tun) {
                        switch (Arrow_point) {
                            case 0:  Pid_vl.Target += (int)float_step; Pid_vr.Target = Pid_vl.Target; break;
                            case 20: Pid_vl.Kp     += float_step; break;
                            case 40: Pid_vr.Kd     += float_step; break;
                            case 60: Pid_vr.Kp     += float_step; break;
                            case 80: Pid_vr.Kd     += float_step; break;
                            case 100:Pid_turn.Kp   += float_step; break;
                            case 120:Pid_turn.Kd   += float_step; break;
                            case 140:Pid_wheel_turn.Kp += float_step; break;
                            case 160:Pid_wheel_turn.Kd += float_step; break;
                            case 180:kp2 += float_step; break;
                            case 200:kd2 += float_step; break;
                            default: break;
                        }
                        if (is_auto_save_enabled()) {
                            save_pid_params_to_flash();
                        }
                    }
                } else {
                    // 短按光标上移
                    if (Screen_mode == para_tun) {
                        if (Arrow_point >= 20) Arrow_point -= 20;
                        else Arrow_point = 200;
                    }
                }
                while (gpio_get_level(KEY3_PIN) == 0);
            }
        }
    } else {
        key3_pressed = 0;
    }

    // ========== KEY4：下移 / 减少值 ==========
    if (gpio_get_level(KEY4_PIN) == 0) {
        if (!key4_pressed) {
            key4_pressed = 1;
            system_delay_ms(20);
            if (gpio_get_level(KEY4_PIN) == 0) {
                if (is_long_press(KEY4_PIN, 300)) {
                    // 长按减少参数
                    if (Screen_mode == para_tun) {
                        switch (Arrow_point) {
                            case 0:  Pid_vl.Target -= (int)float_step; Pid_vr.Target = Pid_vl.Target; break;
                            case 20: Pid_vl.Kp     -= float_step; break;
                            case 40: Pid_vl.Ki     -= float_step; break;
                            case 60: Pid_vl.Kd     -= float_step; break;
                            case 80: Pid_vr.Kp     -= float_step; break;
                            case 100:Pid_vr.Ki     -= float_step; break;
                            case 120:Pid_vr.Kd     -= float_step; break;
                            case 140:Pid_turn.Kp   -= float_step; break;
                            case 160:Pid_turn.Kd   -= float_step; break;
                            case 180:Pid_wheel_turn.Kp -= float_step; break;
                            case 200:Pid_wheel_turn.Kd -= float_step; break;
                            default: break;
                        }
                        if (is_auto_save_enabled()) {
                            save_pid_params_to_flash();
                        }
                    }
                } else {
                    // 短按光标下移
                    if (Screen_mode == para_tun) {
                        if (Arrow_point <= 180) Arrow_point += 20;
                        else Arrow_point = 0;
                    }
                }
                while (gpio_get_level(KEY4_PIN) == 0);
            }
        }
    } else {
        key4_pressed = 0;
    }

    // ========== KEY5：手动保存参数 ==========
    if (gpio_get_level(KEY5_PIN) == 0) {
        system_delay_ms(20);
        if (gpio_get_level(KEY5_PIN) == 0) {
            if (Screen_mode == para_tun) {
                save_pid_params_to_flash();
            }
            while (gpio_get_level(KEY5_PIN) == 0);
        }
    }
}
////轮胎调参    轮子逆时针是向下顺时针向上移动->
//void Tire_tuning(void)
//{
//    if (Speed_L != 0)
//    {
//        // 正转=箭头下移，反转=箭头上移
//        if (Speed_L > 0)
//        {
//            point_num++;
//            if(point_num > 10) point_num = 0; // 到最下回到顶部
//        }
//        else
//        {
//            point_num--;
//            if(point_num < 0) point_num = 10; // 到最上回到底部
//        }
//        Arrow_point = 20 * point_num; // 行间距20，对应参数行

//        // 切换参数时同步暂存值（不变）
//        if (Screen_mode == para_tun)
//        {
//            switch (Arrow_point)
//            {
//                case 0:{  temp_change =  Pid_vl.Target  ;}break;
//                case 20:{  temp_change = Pid_vl.Kp   ;}break;
//                case 40:{ temp_change  =  Pid_vl.Ki;}break;
//                case 60:{ temp_change  = Pid_vl.Kd  ;}break;
//                case 80:{ temp_change = Pid_vr.Kp  ;}break;
//                case 100:{temp_change = Pid_vr.Ki  ;}break;
//                case 120:{ temp_change = Pid_vr.Kd  ;}break;
//                case 140:{ temp_change = Pid_turn.Kp ;}break;
//                case 160:{ temp_change = Pid_turn.Kd  ;}break;
//                case 180:{ temp_change = Pid_wheel_turn.Kp  ;}break;
//                case 200:{ temp_change = Pid_wheel_turn.Kd ;}break;
//                default:break;
//            }
//        }
//    }
//}
