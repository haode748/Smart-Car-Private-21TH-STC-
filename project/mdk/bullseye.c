#include "bullseye.h"

uint8 wd_flag = 0;   
uint8 bullseye_flag = 0;        // 1 = ?????
uint16 bullseye_center_x = 0;   // ???? X ??
uint16 bullseye_center_y = 0;
int region;
int8 laser_on = 0;          // ??????
uint16 laser_delay_count = 0; // ??????
static int shot_cooldown = 0;

// ??????
void detect_bullseye(const uint8 *image)
{
    const int STEP = 2;                     // ????
    int start_row = 45, end_row = 75;       // ??????

    int row, col;
    int left_sum = 0, right_sum = 0;        // ?????????
    int row_sum = 0;                        // ?????????
    int valid_row_count = 0;                // ????
    int black_left = -1, black_right = -1;
    int avg_left, avg_right, center_x, center_y;
    int safe_left, safe_right, safe_row;

    if (shot_cooldown > 0) {
        shot_cooldown--;
        bullseye_flag = 0;
        bullseye_center_x = 0;
        bullseye_center_y = 0;
        return;
    }

    // ========== ???:????????????? ==========
    for (row = end_row; row >= start_row; row -= STEP)
    {
        int left_edge = left_edge_line[row];
        int right_edge = right_edge_line[row];

        // ???????
        for (col = left_edge + 2; col < right_edge - 2; col += STEP)
        {
            uint8 gray = *(image + row * SEARCH_IMAGE_W + col);
            if (gray < white_min_point - 5)
            {
                black_left = col;
                break;
            }
        }

        // ???????
        for (col = right_edge - 2; col > left_edge + 2; col -= STEP)
        {
            uint8 gray = *(image + row * SEARCH_IMAGE_W + col);
            if (gray < white_min_point - 5)
            {
                black_right = col;
                break;
            }
        }

        // ??????????
        if (black_left != -1 && black_right != -1)
        {
            left_sum += black_left;
            right_sum += black_right;
            row_sum += row;
            valid_row_count++;
        }
    }

    // ???????????
    if (valid_row_count == 0)
    {
        bullseye_flag = 0;
        bullseye_center_x = 0;
        bullseye_center_y = 0;
        return;
    }

    // ?????
    avg_left = left_sum / valid_row_count;
    avg_right = right_sum / valid_row_count;
    center_x = (avg_left + avg_right) / 2;
    center_y = row_sum / valid_row_count;

    // ========== ?? X ?????????? ==========
    safe_row = center_y;
    safe_left = left_edge_line[safe_row] + 2;
    safe_right = right_edge_line[safe_row] - 2;

    if (center_x < safe_left || center_x > safe_right)
    {
        bullseye_flag = 0;
        bullseye_center_x = 0;
        bullseye_center_y = 0;
        return;
    }

    // ??????????
    bullseye_flag = 1;
    bullseye_center_x = center_x;
    bullseye_center_y = center_y;

//    // ========== ????(region) ==========
//    if (bullseye_center_x < 115)
//    {
//        wd_flag = 0;
//        if (bullseye_center_x < 53)
//        {
//            region = 1;
//        }
//        else if (bullseye_center_x < 66)
//        {
//            region = 2;
//        }
//        else if (bullseye_center_x < 84)
//        {
//            region = 3;
//        }
//        else if (bullseye_center_x < 96)
//        {
//            region = 4;
//        }
//        else if (bullseye_center_x < 100)
//        {
//            region = 5;
//        }
//        else if (bullseye_center_x < 123)
//        {
//            region = 6;
//        }
//        else
//        {
//            region = 7;
//        }
//    }
//    else
//    {
//        wd_flag = 1;
//        if (bullseye_center_x < 120)
//        {
//            region = 1;
//        }
//        else if (bullseye_center_x < 140)
//        {
//            region = 6;
//        }
//        else if (bullseye_center_x < 180)
//        {
//            region = 2;
//        }
//    }

//    // ========== ?? region ?? JG1 to JG7 ==========
//    if (wd_flag == 1)
//    {
//        wd_flag = 0;
//        if (center_y > 58)
//        {
//            JG1 = JG2 = JG3 = JG4 = JG5 = JG6 = JG7 = 0;
//            switch (region)
//            {
//                case 1:
//                    JG1 = 1;
//                    break;
//                case 2:
//                    JG7 = 1;
//                    break;
//                case 3:
//                    JG3 = 1;
//                    break;
//                case 4:
//                    JG4 = 1;
//                    break;
//                case 5:
//                    JG5 = 1;
//                    break;
//                case 6:
//                    JG6 = 1;
//                    break;
//                case 7:
//                    JG7 = 1;
//                    break;
//            }
//            laser_on = 1;
//            laser_delay_count = 2;   // ??????
//        }
//        else if (center_y > 72)
//        {
//            JG1 = JG2 = JG3 = JG4 = JG5 = JG6 = JG7 = 0;
//            switch (region)
//            {
//                case 1:
//                    JG1 = 1;
//                    break;
//                case 2:
//                    JG2 = 1;
//                    break;
//                case 3:
//                    JG3 = 1;
//                    break;
//                case 4:
//                    JG4 = 1;
//                    break;
//                case 5:
//                    JG5 = 1;
//                    break;
//                case 6:
//                    JG6 = 1;
//                    break;
//                case 7:
//                    JG7 = 1;
//                    break;
//            }
//            laser_on = 1;
//            laser_delay_count = 2;   // ??????
//        }
//    }
//    else
//    {
//        if (center_y > 63)
//        {
//            JG1 = JG2 = JG3 = JG4 = JG5 = JG6 = JG7 = 0;
//            switch (region)
//            {
//                case 1:
//                    JG1 = 1;
//                    break;
//                case 2:
//                    JG2 = 1;
//                    break;
//                case 3:
//                    JG3 = 1;
//                    break;
//                case 4:
//                    JG4 = 1;
//                    break;
//                case 5:
//                    JG5 = 1;
//                    break;
//                case 6:
//                    JG6 = 1;
//                    break;
//                case 7:
//                    JG7 = 1;
//                    break;
//            }
//            laser_on = 1;
//            laser_delay_count = 2;  
//        }
//    }
   if (bullseye_center_x < 51)
        {
            region = 1;
        }
        else if (bullseye_center_x < 66)
        {
            region = 2;
        }
        else if (bullseye_center_x < 84)
        {
            region = 3;
        }
        else if (bullseye_center_x < 95)
        {
            region = 4;
        }
        else if (bullseye_center_x < 110)
        {
            region = 5;
        }
        else if (bullseye_center_x < 123)
        {
            region = 6;
        }
        else
        {
            region = 7;
        }
				 if (center_y > 55)
        {
            JG1 = JG2 = JG3 = JG4 = JG5 = JG6 = JG7 = 0;
            switch (region)
            {
                case 1:
                    JG2 = 1;
                    break;
                case 2:
                    JG2 = 1;
                    break;
                case 3:
                    JG4 = 1;
                    break;
                case 4:
                    JG4 = 1;
                    break;
                case 5:
                    JG4 = 1;
                    break;
                case 6:
                    JG6 = 1;
                    break;
                case 7:
                    JG6 = 1;
                    break;
            }
            laser_on = 1;
            laser_delay_count = 2;  
        }
}