#include "screen.h"

#define POINT_DRAW_X_OFFSET  100 // 坐标绘制X偏移
#define POINT_DRAW_Y_OFFSET  50  // 坐标绘制Y偏移
// 屏幕映射：把摄像头图像缩放到屏幕



void Screen_Show_Edge_Line(void)
{
//	   int d;
//    uint16 cx = 130;
//     uint16 cy = 60;
       
    uint16 row=0;
    uint16 x_l, x_r, x_center=0;
    uint16 y_screen=0;

//	    ips200_show_string(0, 20, "S_L:");//编码器读值
//    ips200_show_string(120, 20, "S_R:");
//    
//    ips200_show_float(55, 20, Speed_L, 2, 3);
//  ips200_show_float(175, 20, Speed_R, 2, 3);
    // 可选：清屏（取消注释则每次显示前清空边线区域）
    // ips200_set_region(DISP_START_X, DISP_START_Y, 
    //                   ips200_x_max-1, ips200_y_max-1);
    // ips200_clear(COLOR_BG);
 
    // 2. 逐行绘制左右边线和中心线
    for(row = MT9V03X_H-2; row > 0; row--)
    {
			  y_screen = row;
        x_l = left_edge_line[row];
        x_r = right_edge_line[row];
//	
//       ips200_show_int16(0,100, Island_State);
//       ips200_show_int16(0,120, Left_Island_Flag);
////       ips200_show_int16(0,140, Right_Island_Flag);
//       ips200_show_int16(0,160,Both_Lost_Time);     // 最长白列
//       ips200_show_int16(0,180,Boundry_Start_Left);   // 左有效边界起始行
//       ips200_show_int16(0,200,Boundry_Start_Right);  // 右有效边界起始行
//			  ips200_show_int16(0,270,Img_error);
//			  ips200_show_uint8(0,220,Left_Lost_Time);
////			ips200_show_uint8(0,220,Zebra_State);
//			  ips200_show_int16(0,240,Right_Lost_Time);
//			  ips200_show_uint8(0,250,yuansu);
//        // 边界检查（避免超出屏幕范围）
//        if(y_screen < ips200_y_max)
//        {
//            // 绘制左边界点
//            if(x_l < ips200_x_max)
                ips200_draw_point(x_l, y_screen, RGB565_RED);
////            
////            // 绘制右边界点
////            if(x_r < ips200_x_max)
                ips200_draw_point(x_r, y_screen, RGB565_BLUE);
//            
//            // 绘制赛道中心线
                x_center = (left_edge_line[row] + right_edge_line[row])/2 ;
//            if(x_center < ips200_x_max)
                ips200_draw_point(x_center, y_screen, RGB565_GREEN);
//			       if(filled_line_left[row])
                ips200_draw_point(reference_col, y_screen, RGB565_YELLOW);
//        {
        
    }
			        
     if(bullseye_flag)   // 检测到靶子
    {
        uint16 cx = bullseye_center_x;
        uint16 cy = bullseye_center_y;
        int d;
        // 边界检查
       
            // 方式2（可选）：额外画一个红色十字
            for( d = -4; d <= 4; d++) {
//                if(cx + d < ips200_x_max && cx + d >= 0) {
                    ips200_draw_point(cx + d, cy, RGB565_RED);
                    ips200_draw_point(cx, cy + d, RGB565_RED);
                }
							}
//     if(bullseye_flag)   // 检测到靶子
//    {
//        // 边界检查
//       
//            // 方式2（可选）：额外画一个红色十字
//            for( d = -4; d <= 4; d++) {
////                if(cx + d < ips200_x_max && cx + d >= 0) {
//                    ips200_draw_point(cx + d, cy, RGB565_RED);
//                    ips200_draw_point(cx, cy + d, RGB565_RED);
//                }
//							}
//    
}
//		
//		
//}