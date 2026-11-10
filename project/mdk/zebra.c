#include "zebra.h"

int Zebra_State=0;
int Barricade_Flag=0;
uint8 Stop_Flag=0;

void Zebra_idntify(void)
{
    int i = 0;
	  uint16 j = 0;
    int change_count = 0;
    int start_line = 0;
    int end_line = 0;
    int narrow_road_count = 0;

//    // ===================== 调试输出 =====================
//    printf("\n【斑马线】进入检测\n");
//    printf("Search_Stop_Line = %d\n", Search_Stop_Line);
//    printf("reference_col = %d\n", reference_col);
//    printf("Boundry_Left = %d\n", Boundry_Start_Left);
//    printf("Boundry_Right = %d\n", Boundry_Start_Right);
//    // ====================================================

    /* 1. 互斥判断 */
    if (Cross_flag != 0 || (1 <= Ramp_Flag && Ramp_Flag <= 3) || Zebra_State != 0 ||
        Stop_Flag != 0 || Barricade_Flag != 0)
    {
//        printf("【斑马线】被互斥条件挡住，退出\n"); // ?? 打印
        return;
    }
//    printf("【斑马线】通过互斥判断\n"); // ?? 打印

    /* 2. 主条件 */
    if (Search_Stop_Line >= 60 &&
        30 <= reference_col && reference_col <= MT9V03X_W - 30 &&
        Boundry_Start_Left >= MT9V03X_H - 15 && Boundry_Start_Right >= MT9V03X_H - 15)
    {
//        printf("【斑马线】通过主条件，开始找变窄\n"); // ?? 打印

        for (i = 65; i >= 20; i--)
        {
            if ((Standard_Road_Wide[i] - Road_Wide[i]) > 10)
            {
                narrow_road_count++;
                if (narrow_road_count >= 5)
                {
                    start_line = i;
//                    printf("【斑马线】找到变窄！start_line = %d\n", start_line); // ?? 打印
                    break;
                }
            }
        }
    }
    else
    {
//        printf("【斑马线】主条件不满足！\n"); // ?? 打印
    }

    /* 3. 统计跳变 */
    if (start_line != 0)
    {
//        printf("【斑马线】开始统计跳变\n"); // ?? 打印

        start_line = start_line + 8;
        end_line = start_line - 15;

        if (start_line >= MT9V03X_H - 1) start_line = MT9V03X_H - 1;
        if (end_line <= 0) end_line = 0;

        for (i = start_line; i >= end_line; i--)
        {
            for (j = left_edge_line[i]; j <= right_edge_line[i]; j++)
            {
                if (mt9v03x_image[i][j+1] - mt9v03x_image[i][j] != 0)
                {
                    change_count++;
                }
            }
        }
//        printf("【斑马线】跳变次数 = %d\n", change_count); // ?? 打印
    }

    /* 4. 最终判断 */
    if (change_count > 30)
    {
        Zebra_State = 1;
//			   Beep=1;
//				 system_delay_ms(50); // 延时50ms
//				 Beep=0;
//        printf("【斑马线】检测成功！！！\n"); // ?? 打印
    }
    else
    {
//        printf("【斑马线】跳变不足\n"); // ?? 打印
    }
}