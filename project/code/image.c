#include "Image.h"

uint8 reference_point = 0;
uint8 reference_col = 0;
uint8 white_max_point = 0;
uint8 white_min_point = 0;
uint8 reference_contrast_ratio = 30;                // ³õÊ¼»¯¶Ô±È¶ÈãÐÖµ£¨¿É¸ù¾ÝÊµ¼Êµ÷ÊÔ£©
uint8 reference_col_line[SEARCH_IMAGE_H] = {0};
uint8 remote_distance[SEARCH_IMAGE_W] = {0};
uint16 left_edge_line[SEARCH_IMAGE_H] = {0};
uint16 right_edge_line[SEARCH_IMAGE_H] = {0};
uint16 Road_Wide[SEARCH_IMAGE_H] = {0};           //Â·¿í
uint32 if_count = 0;
uint8 Cross_flag = 0;                  // Ê®×Ö¼ì²â±êÖ¾Î»
uint8 current_col = 0;  
int Left_Lost_Time = 0;          //×ó¶ªÏß
int Right_Lost_Time = 0;         //ÓÒ¶ªÏß
int Both_Lost_Time = 0;          //×óÓÒ¶¼¶ªÏß
int	Boundry_Start_Left = STOPROW;    // ×óÓÐÐ§Æðµã£¬Ä¬ÈÏ×îÔ¶ÐÐ
int Boundry_Start_Right = STOPROW;   // ÓÒÓÐÐ§Æðµã£¬Ä¬ÈÏ×îÔ¶ÐÐ
int Search_Stop_Line=0;                //ËÑË÷½ØÖ¹ÐÐ
uint8 start_flag=0;
// Í¼ÏñÎó²î
int32 cycle_error=0 ;
int32  Img_error=0 ;
int32  Img_error_1=0 ;
int32  Img_error_last=0 ;

//// ÈüµÀ¿í¶È ×óÓÒ±ßÏß±ä»¯
//int16 RS_width_cacul = 0;
//int16 RS_left_cacul = 0;
//int16 RS_right_cacul = 0;
uint8 Weight_1[MT9V03X_H] = {						///	//////////////////////////////////////////////ÖÐÏß¼ÓÈ¨ÇóÈ¨ÖØÔ½ÍùÉÏ¿¿£¬³µ×ªÍäÔ½Ôç
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //??í¨?°?°
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                                    // 10-19DD?oè¨??1
            1, 1, 1, 1, 1, 1, 6, 7,8,  13, 
            13,13,10,11,12,13, 14,15,1, 16,
            17, 18, 19, 20, 20,20, 1,1,1,1,     // 0-9DD?¨×????????oè¨??1
            1, 1, 1, 1,1, 1, 1, 1, 1, 1, 
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      // 0-9DD?¨×????????oè¨??1
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

};
//uint8 Weight[MT9V03X_H] = {
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //ÆÕÍ¨Ç°Õ°    //0~0
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                                    // 10-19ÐÐ£ºÈ¨ÖØ1
//            8, 9, 10, 13, 13, 13, 5, 5,7,  8, 
//        9,10,10,11,12,13, 14,15,1, 16,
//        17, 18, 19, 7, 7,7, 1,1,1,1,     // 0-9ÐÐ£¨×îÔ¶¶Ë£©£ºÈ¨ÖØ1
//            1, 1, 1, 1,1, 1, 1, 1, 1, 1, 
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      // 0-9ÐÐ£¨×îÔ¶¶Ë£©£ºÈ¨ÖØ1
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//};

uint8 Weight[MT9V03X_H] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //??í¨?°?°
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                                    // 10-19DD?oè¨??1
            1, 1, 1, 1, 1, 10, 10, 10,13,  13, 
            13,13,10,11,12,13, 14,15,1, 16,
            17, 20, 20, 20, 20,20, 1,1,1,1,     // 0-9DD?¨×????????oè¨??1
            1, 1, 1, 1,1, 1, 1, 1, 1, 1, 
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      // 0-9DD?¨×????????oè¨??1
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
//uint8 Weight[MT9V03X_H] = {
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //ÆÕÍ¨Ç°Õ°    //0~0
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                                    // 10-19ÐÐ£ºÈ¨ÖØ1
//            1, 1,8,11,12,13,14,14,14,15, 
//            15,16,16,17,17,18,20,20,20, 20,
//            10, 10, 10, 10, 10, 1, 1, 1, 1, 1,     // 0-9ÐÐ£¨×îÔ¶¶Ë£©£ºÈ¨ÖØ1
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      // 0-9ÐÐ£¨×îÔ¶¶Ë£©£ºÈ¨ÖØ1
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//};
//uint8 Weight[MT9V03X_H] = {
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //ÆÕÍ¨Ç°Õ°    //0~9
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                 //10-19
//            9,11,16,18,20,21,22,23,24,25,               //20~29 
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                //30~39
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                  //40~49 
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                  //50~59
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                 //60~69
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                 //70~79
//};
//uint8 Weight_2[MT9V03X_H] = {
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,     //×î¿ªÊ¼Ç°Õ°
//            1, 1, 1, 1, 1, 1, 1, 13, 13, 13,                                    // 10-19ÐÐ£ºÈ¨ÖØ1
//            10, 13, 17, 13, 10, 13, 11, 13, 14, 14,     // 60-69ÐÐ£ºÈ¨ÖØ1  
//            16, 17, 18,18,19,19,10,1, 1,1, 
//            1,1, 1, 1, 1, 1,1, 1,1,1,     // 0-9ÐÐ£¨×îÔ¶¶Ë£©£ºÈ¨ÖØ1
//            1, 1, 1, 1,1, 1, 1, 1, 1, 1, 
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,      // 0-9ÐÐ£¨×îÔ¶¶Ë£©£ºÈ¨ÖØ1
//            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//};

//±ê×¼Èü¿í£¬½«³µ×Ó·ÅÔÚ³¤Ö±µÀÉÏÃæÊµ²â£¬ÒÔÏÂÊýÖµ½ö¹©²Î¿¼
uint8 Standard_Road_Wide[MT9V03X_H]=
{  0,   0, 23, 24, 26, 27, 28, 30, 32, 32,
   34, 36, 37, 39, 40, 42, 43, 45, 46, 48,
   49, 50, 52, 53, 55, 56, 58, 59, 61, 62,
   64, 65, 67, 68, 69, 71, 73, 74, 75, 77,
   79, 79, 81, 83, 85, 85, 87, 89, 91, 92,
   93, 95, 96, 98, 99,101,102,104,105,107,
  108,110,111,113,114,116,117,118,120,122,
	123,124,126,128,129,130,132,133,134,136
};

//ÏÞ·ùº¯Êý
int16 clamp(int16 value, int16 min, int16 max) 
	{
    if (value < min) 
		{
        return min;
    } 
			else if (value > max) 
		{
        return max;
    } 
				else 
		{
        return value;
    }
}


//»ñÈ¡²Î¿¼µãÎ»				ãÐÖµ
void get_reference_point(const uint8 *image)					//ÔÚÍ¼Ïñµ×²¿²ÉÑù 3 ÐÐ£¬ËãÆ½¾ù»Ò¶È		//////////////////////////Ðè¸Ä
{
	  uint16 i;
    // Í¼ÏñÊý×éÖ¸ÕëÖ¸Ïò´ýÍ³¼ÆÐÐ
    const uint8 *p = image + (SEARCH_IMAGE_H - REFERENCEROW) * SEARCH_IMAGE_W;
    uint16 temp;          // ±£´æ´ýÍ³¼ÆµãÊýÁ¿
	  uint16 temp2; 
    uint32 temp1;         // ±£´æËùÓÐÍ³¼Æµã¼ÓÆðÀ´µÄºÍ
	  temp1=0;
	  temp2=0;
    temp = REFERENCEROW * SEARCH_IMAGE_W;  // ¼ÆËã´ýÍ³¼Æµã×ÜÊýÁ¿
    for(i = 0; i < temp; i +=20)
    {
        temp1 += *(p + i);    // Í³¼ÆµãÇóºÍ
			  temp2++;
    }
    // ¼ÆËãµãÆ½¾ùÖµ£¬×÷Îª±¾·ùÍ¼ÏñµÄ²Î¿¼µã
    reference_point = (uint8)(temp1 / temp2);
    // ¼ÆËã°×µã×î´óÖµ£º²Î¿¼µã·Å´óºó£¬ÓÃfunc_limit_abÏÞÖÆÔÚ0~BLACKPOINT
    white_max_point = clamp((int32)reference_point * WHITEMAXMUL / 10,BLACKPOINT,250); 
    // ¼ÆËã°×µã×îÐ¡Öµ£º²Î¿¼µãËõÐ¡ºó£¬ÓÃfunc_limit_abÏÞÖÆÔÚ0~BLACKPOINT
    white_min_point = clamp((int32)reference_point * WHITEMINMUL / 10, 0, BLACKPOINT);
//  	printf("ref=%d, min=%d, max=%d\n", reference_point, white_min_point, white_max_point);
		 // ===================== ÐÂÔö´òÓ¡ =====================
//    printf("ref=%d, white_min=%d, white_max=%d\n", reference_point, white_min_point, white_max_point);
}

//void get_reference_point(const uint8 *image)
//{
//	  uint16 i;
//    // Í¼ÏñÊý×éÖ¸ÕëÖ¸Ïò´ýÍ³¼ÆÐÐ
//   const int start_row = SEARCH_IMAGE_H - REFERENCEROW;  // ×îºó REFERENCEROW ÐÐ
//   const int start_col = (SEARCH_IMAGE_W - REGION_WIDTH) / 2; // ¾ÓÖÐ
//   const uint8 *p = image + start_row * SEARCH_IMAGE_W + start_col;
//    uint16 temp  = 0;          // ±£´æ´ýÍ³¼ÆµãÊýÁ¿
//    uint32 temp1 = 0;         // ±£´æËùÓÐÍ³¼Æµã¼ÓÆðÀ´µÄºÍ
//    temp = REFERENCEROW * SEARCH_IMAGE_W;  // ¼ÆËã´ýÍ³¼Æµã×ÜÊýÁ¿
//    for(i = 0; i < temp; i++)
//    {
//        temp1 += *(p + i);    // Í³¼ÆµãÇóºÍ
//    }
//    // ¼ÆËãµãÆ½¾ùÖµ£¬×÷Îª±¾·ùÍ¼ÏñµÄ²Î¿¼µã
//    reference_point = (uint8)(temp1 / temp);
//    // ¼ÆËã°×µã×î´óÖµ£º²Î¿¼µã·Å´óºó£¬ÓÃfunc_limit_abÏÞÖÆÔÚ0~BLACKPOINT
//    white_max_point = clamp((int32)reference_point * WHITEMAXMUL / 100,BLACKPOINT,255); 
//    // ¼ÆËã°×µã×îÐ¡Öµ£º²Î¿¼µãËõÐ¡ºó£¬ÓÃfunc_limit_abÏÞÖÆÔÚ0~BLACKPOINT
//    white_min_point = clamp((int32)reference_point * WHITEMINMUL / 100, 0, BLACKPOINT);
////  	printf("ref=%d, min=%d, max=%d\n", reference_point, white_min_point, white_max_point);
       // ===================== ÐÂÔö´òÓ¡ =====================
//    printf("ref=%d, white_min=%d, white_max=%d\n", reference_point, white_min_point, white_max_point);
//	}

// ±éÀúÊý×é×î´óÖµ»ò×îÐ¡Öµ
uint8 find_extreme_value(uint8 *array_value, uint8 num0, uint8 num1, uint8 model)
{
    uint8 i = 0, temp = 0, temp1 = 0, temp2 = 0, value = 0;
    uint8 SEARCH_COL_START= 30;  // ÏÈ¶¨Òå
    uint8 SEARCH_COL_END = MT9V03X_W - 30;

		 if(Right_Island_Flag==1)//ÓÒ»·
    {
        if(Island_State==3)
        {
            SEARCH_COL_START=70;                        //¸ÄÁË
            SEARCH_COL_END=MT9V03X_W-5;
        }
    }
    else if(Left_Island_Flag==1)//×ó»·
    {
        if(Island_State==3)
        {
            SEARCH_COL_START=5;
            SEARCH_COL_END=MT9V03X_W-70;
        }
    }

    // ´¦Àí·´Ïò±éÀú£¨num0 > num1£©
    if(num0 > num1)
    {
        temp1 = num0 - num1;
        temp2 = num1;
        array_value += num0;
        value = *array_value;

        // Ñ°ÕÒ×î´óÖµ
        if(model)
        {
            for(i = 0; i <= temp1; i++)
            {
                current_col = num0 - i; // µ±Ç°±éÀúµÄÁÐË÷Òý
                // Ìø¹ý³¬³ö20~MT9V03X_W-20µÄÁÐ
                if(current_col < SEARCH_COL_START || current_col > SEARCH_COL_END) continue;
                
                temp = *(array_value - i);
                if(temp > value)
                {
                    temp2 = current_col;
                    value = temp;
                }
            }
        }
        // Ñ°ÕÒ×îÐ¡Öµ
        else
        {
            for(i = 0; i <= temp1; i++)
            {
                current_col = num0 - i;
                if(current_col < SEARCH_COL_START || current_col > SEARCH_COL_END) continue;
                
                temp = *(array_value - i);
                if(temp < value)
                {
                    temp2 = current_col;
                    value = temp;
                }
            }
        }
    }
    // ´¦ÀíÕýÏò±éÀú£¨num0 <= num1£©
    else
    {
        temp1 = num1 - num0;
        temp2 = num0;
        array_value += num0;
        value = *array_value;

        // Ñ°ÕÒ×î´óÖµ
        if(model)
        {
            for(i = 0; i <= temp1; i++)
            {
                uint8 current_col = num0 + i;
                if(current_col < SEARCH_COL_START || current_col > SEARCH_COL_END) continue;
                
                temp = *(array_value + i);
                if(temp > value)
                {
                    temp2 = current_col;
                    value = temp;
                }
            }
        }
        // Ñ°ÕÒ×îÐ¡Öµ
        else
        {
            for(i = 0; i <= temp1; i++)
            {
                uint8 current_col = num0 + i;
                if(current_col < SEARCH_COL_START || current_col > SEARCH_COL_END) continue;
                
                temp = *(array_value + i);
                if(temp < value)
                {
                    temp2 = current_col;
                    value = temp;
                }
            }
        }
    }

    // ×îÖÕÐ£Ñé£ºÈ·±£·µ»ØÖµÔÚÏÞÖÆ·¶Î§ÄÚ
    if(temp2 < SEARCH_COL_START) temp2 = SEARCH_COL_START;
    if(temp2 > SEARCH_COL_END)   temp2 = SEARCH_COL_END;

    // ·µ»Ø¼«ÖµËùÔÚµÄË÷ÒýÎ»ÖÃ
    return temp2;
}

//ËÑË÷²Î¿¼ÁÐ
void search_reference_col(const uint8 *image)
{
    int col, row;
    int16 temp1 = 0, temp2 = 0, temp3 = 0;
    int found;
    // ³õÊ¼»¯ËùÓÐÁÐµÄ¼ì²â¾àÀëÎªÍ¼Ïñ×îµ×²¿
    for(col = 0; col < SEARCH_IMAGE_W; col++)
    {
        remote_distance[col] = SEARCH_IMAGE_H - 1;
    }
	

    // ÖðÁÐ¼ä¸ôÉ¨Ãè£¬²½³¤Îª CONTRASTOFFSET
    for(col = 0; col < SEARCH_IMAGE_W; col += CONTRASTOFFSET)					////////////////////////////////////////////////////////
    {
			  found=0;
        // ´ÓÍ¼Ïñµ×²¿ÏòÉÏ¼ä¸ôÉ¨Ãè
        for(row = SEARCH_IMAGE_H - 1; row >= STOPROW; row -= CONTRASTOFFSET)
        {
            // »ñÈ¡µ±Ç°µã»Ò¶ÈÖµ
            temp1 = *(image + row * SEARCH_IMAGE_W + col);
            // »ñÈ¡¶Ô±Èµã»Ò¶ÈÖµ£¨ÉÏ·½ STOPROW ¾àÀë´¦£©
            temp2 = *(image + (row - STOPROW) * SEARCH_IMAGE_W + col);

            // ÅÐ¶Ï¶Ô±ÈµãÊÇ·ñÎª°×µã£¬ÈôÊÇÔòÌø¹ýµ±Ç°µã
            if(temp2 > white_max_point)
            {
                continue;
            }

            // ÅÐ¶Ïµ±Ç°µãÊÇ·ñÎªºÚµã£¬ÈôÊÇÔò¼ÇÂ¼ÐÐ×ø±ê²¢ÍË³öÉ¨Ãè
            if(temp1 < white_min_point)
            {
                remote_distance[col] = row;
							  found=1;
                break;
            }

            // ¼ÆËã¶Ô±È¶È
            temp3 = (temp1 - temp2) * 200 / (temp1 + temp2);

            // ÅÐ¶Ï¶Ô±È¶ÈÊÇ·ñ´óÓÚãÐÖµ£¬»òÒÑÉ¨Ãèµ½¶¥²¿
            if(temp3 > reference_contrast_ratio || row == STOPROW)
            {
                remote_distance[col] = row;
							  found=1;
                break;
            }
        }
				if(found!=1)
					remote_distance[col] = STOPROW;
    }

    // ´Ó¼ì²â½á¹ûÖÐÕÒµ½×îÎÈ¶¨µÄ²Î¿¼ÁÐ
    reference_col = find_extreme_value(remote_distance, 10, SEARCH_IMAGE_W - 10, 0) ;
    // ¶Ô²Î¿¼ÁÐ½øÐÐ±ß½çÏÞÖÆ
    reference_col = clamp(reference_col, 1, SEARCH_IMAGE_W - 2);
    Search_Stop_Line = SEARCH_IMAGE_H - remote_distance[reference_col];
//    printf("reference_col = %d\n", remote_distance[reference_col]);     // ×îÖÕ³¤¶È
//    printf("Search_Stop_Line = %d\n", Search_Stop_Line);
}
 
//ËÑË÷ÈüµÀ±ß½ç
void search_line(const uint8 *image)
{
    uint8 *p = (uint8 *)image[0];             // Í¼ÏñÊý×éÖ¸Õë
    uint8 row_max = SEARCH_IMAGE_H - 1;       // ÐÐ×î´óÖµ
    uint8 row_min = STOPROW;                  // ÐÐ×îÐ¡Öµ
    uint8 col_max = SEARCH_IMAGE_W - CONTRASTOFFSET; // ÁÐ×î´óÖµ
    uint8 col_min = CONTRASTOFFSET;           // ÁÐ×îÐ¡Öµ

    int16 leftstartcol = reference_col;       // ËÑÏß×óÆðÊ¼ÁÐ
    int16 rightstartcol = reference_col;      // ËÑÏßÓÒÆðÊ¼ÁÐ
    int16 leftendcol = 0;                     // ËÑÏß×óÖÕÖ¹ÁÐ
    int16 rightendcol = SEARCH_IMAGE_W - 1;   // ËÑÏßÓÒÖÕÖ¹ÁÐ
   
	  int boundry_left_flag=0;
	  int boundry_right_flag=0;
	  int lost_left_flag=0;
	  int lost_right_flag=0;
	
    uint8 search_time = 0;                    // µ¥µãËÑÏß´ÎÊý
    uint8 temp1 = 0, temp2 = 0;               // ÁÙÊ±±äÁ¿£¬ÓÃÓÚ´æ´¢Í¼ÏñÊý¾Ý
    int temp3 = 0;                            // ÁÙÊ±±äÁ¿£¬ÓÃÓÚ´æ´¢¶Ô±È¶È
    int leftstop = 0, rightstop = 0, stoppoint = 0; // ËÑÏß×ÔËø±äÁ¿
//    int wide;
    int col, row;
    Left_Lost_Time = 0;          //×ó¶ªÏß
    Right_Lost_Time = 0;         //ÓÒ¶ªÏß
    Both_Lost_Time = 0;          //×óÓÒ¶¼¶ªÏß
    Boundry_Start_Left = STOPROW;    // ×óÓÐÐ§Æðµã£¬Ä¬ÈÏ×îÔ¶ÐÐ
    Boundry_Start_Right = STOPROW;   // ÓÒÓÐÐ§Æðµã£¬Ä¬ÈÏ×îÔ¶ÐÐ
	  
    // ³õÊ¼»¯×óÓÒ±ß½çÏßÎªÍ¼Ïñ±ßÔµ
    for(row = row_max; row >= row_min; row--)
    {
        left_edge_line[row] = col_min - CONTRASTOFFSET;
        right_edge_line[row] = col_max + CONTRASTOFFSET;
    }

    // ÖðÐÐÉ¨Ãè£¬ËÑË÷×ó±ß½ç
    for(row = row_max; row >= row_min; row--)
    {
        p = (uint8 *)&image[row * SEARCH_IMAGE_W]; // »ñÈ¡±¾ÐÐÆðµãÎ»ÖÃÖ¸Õë
        if(!leftstop)
        {
            search_time = 2;
            do
            {
                if(search_time == 1)
                {
                    leftstartcol = reference_col;
                    leftendcol = col_min;
                }
                search_time--;

                // ´Ó×óÆðÊ¼ÁÐÏò×óÉ¨Ãè
                for(col = leftstartcol; col >= leftendcol; col-=3)			//////////////////////////////////////////ÄÚ²ã
                {
                    temp1 = *(p + col);                      // »ñÈ¡µ±Ç°µã»Ò¶ÈÖµ
                    temp2 = *(p + col - CONTRASTOFFSET);     // »ñÈ¡¶Ô±Èµã»Ò¶ÈÖµ
                
//									  printf("[×ó±ß½ç] row:%d, col:%d | temp1:%d, temp2:%d\r\n", row, col, temp1, temp2);
                    // Èç¹û²Î¿¼ÁÐÊÇºÚµã£¬Ôò×ÔËø²¢Çå³ýÎÞÐ§×ó±ß½ç
                    if(temp1 < white_min_point && col == leftstartcol && leftstartcol == reference_col)
                    {
                        leftstop = 1;
                        for(stoppoint = row; stoppoint >= 0; stoppoint--)
                        {
                            left_edge_line[stoppoint] = col_min;
                        }
                        search_time = 0;
                        break;
                    }

                    // Èç¹ûµ±Ç°µãÊÇºÚµã£¬Ö±½Ó¼ÇÂ¼Îª×ó±ß½ç
                    if(temp1 < white_min_point)
                    {
                        left_edge_line[row] = (uint8)col;
                        break;
                    }

                    // Èç¹û¶Ô±ÈµãÊÇ°×µã£¬Ìø¹ýµ±Ç°µã
                    if(temp2 > white_max_point)
                    {
                        continue;
                    }

                    // ¼ÆËã¶Ô±È¶È
                    temp3 = (temp1 - temp2) * 200 / (temp1 + temp2);

                    // Èç¹û¶Ô±È¶È´óÓÚãÐÖµ»òÒÑµ½ÁÐ±ßÔµ£¬¼ÇÂ¼×ó±ß½ç
                    if(temp3 > reference_contrast_ratio || col == col_min-2)
                    {
                        left_edge_line[row] = col;
                        // ¸üÐÂÏÂÒ»´ÎËÑÏßµÄ·¶Î§
                        leftstartcol = clamp(col + SEARCHRANGE, col, col_max);
                        leftendcol = clamp(col - SEARCHRANGE, col_min, col);
                        search_time = 0;
                        break;
                    }
                }
            }while(search_time);
        }

        // ÖðÐÐÉ¨Ãè£¬ËÑË÷ÓÒ±ß½ç
        if(!rightstop)
        {
            search_time = 2;
            do
            {
                if(search_time == 1)
                {
                    rightstartcol = reference_col;
                    rightendcol = col_max;
                }
                search_time--;

                // ´ÓÓÒÆðÊ¼ÁÐÏòÓÒÉ¨Ãè
                for(col = rightstartcol; col <= rightendcol; col+=3)
                {
                    temp1 = *(p + col);                      // »ñÈ¡µ±Ç°µã»Ò¶ÈÖµ
                    temp2 = *(p + col + CONTRASTOFFSET);     // »ñÈ¡¶Ô±Èµã»Ò¶ÈÖµ

//									  printf("[ÓÒ±ß½ç] row:%d, col:%d | temp1:%d, temp2:%d\r\n", row, col, temp1, temp2);
                    // Èç¹û²Î¿¼ÁÐÊÇºÚµã£¬Ôò×ÔËø²¢Çå³ýÎÞÐ§ÓÒ±ß½ç
                    if(temp1 < white_min_point && col == reference_col && rightstartcol == reference_col)
                    {
                        rightstop = 1;
                        for(stoppoint = row; stoppoint >= 0; stoppoint--)
                        {
                            right_edge_line[stoppoint] = col_max;
                        }
                        search_time = 0;
                        break;
                    }

                    // Èç¹ûµ±Ç°µãÊÇºÚµã£¬Ö±½Ó¼ÇÂ¼ÎªÓÒ±ß½ç
                    if(temp1 < white_min_point)
										 { 
                     right_edge_line[row] = (uint8)col;
                         break;
                     }
                     // Èç¹û¶Ô±ÈµãÊÇ°×µã£¬Ìø¹ýµ±Ç°µã
                     if(temp2 > white_max_point)
                     {
                         continue;
                     }
                     // ¼ÆËã¶Ô±È¶È
                     temp3 = (temp1 - temp2) * 200 / (temp1 + temp2);
                     // Èç¹û¶Ô±È¶È´óÓÚãÐÖµ»òÒÑµ½ÁÐ±ßÔµ£¬¼ÇÂ¼ÓÒ±ß½ç
                     if(temp3 > reference_contrast_ratio || col == col_max+2)
                     {
                         right_edge_line[row] = col;
                         // ¸üÐÂÏÂÒ»´ÎËÑÏßµÄ·¶Î§
                         rightstartcol = clamp(col - SEARCHRANGE, col_min, col);
                         rightendcol = clamp(col + SEARCHRANGE, col, col_max);
                         search_time = 0;
                         break;
                     }
                 }
             }while(search_time);
         }
     }
		
//		 for(row = row_max; row >= row_min; row--)
//{
//    //Í³¼Æ¶ªÏß
//    if(left_edge_line[row] <= 0)

//        Left_Lost_Time++;

//    if(right_edge_line[row] >= 180)
//        Right_Lost_Time++;

//    if(left_edge_line[row] <= 0 && right_edge_line[row] >= 180)
//        Both_Lost_Time++;
//}
//        for(row = row_max; row >= row_min; row--)
//    {
//        // ×ó¶ªÏß£º»¹ÊÇÄ¬ÈÏÖµ
//        if(left_edge_line[row] <= 0)
//            Left_Lost_Time++;

//        // ÓÒ¶ªÏß
//        if(right_edge_line[row] >= 180)
//            Right_Lost_Time++;

//        // Ë«±ß¶ªÏß
//        if( left_edge_line[row]<= 0 &&
//            right_edge_line[row] >= 180)
//            Both_Lost_Time++;
//    }
for(row = row_max-2; row >= row_min; row--)
{
    //Í³¼Æ¶ªÏß
    if(left_edge_line[row] <= 0)
        Left_Lost_Time++;
//        	printf("L=%d\r\n",row);
    if(right_edge_line[row] >= 180)
        Right_Lost_Time++;
//          printf("R=%d\r\n",row);
    if(left_edge_line[row] <= 0 && right_edge_line[row] >= 180)
        Both_Lost_Time++;
    // ÕÒ×óÆðµã
    if(Boundry_Start_Left == STOPROW && left_edge_line[row] > 0)
    {
        Boundry_Start_Left = row;
    }
    // ÕÒÓÒÆðµã
    if(Boundry_Start_Right == STOPROW && right_edge_line[row] < 180)
    {
        Boundry_Start_Right = row;
    }
}
//	printf("Left_Lost_Time=%d, Right_Lost_Time=%d, Both_Lost_Time=%d\r\n", 
//       Left_Lost_Time, Right_Lost_Time, Both_Lost_Time);
//printf("Boundry_Start_Left=%d, Boundry_Start_Right=%d\r\n", 
//       Boundry_Start_Left, Boundry_Start_Right);
// if(row <= row_max - 2)   // ´Ó row_max-2 ÐÐ¿ªÊ¼Í³¼Æ£¨ÓëÔ­¶ÀÁ¢Ñ­»·Ò»ÖÂ£©
//        {
//            // Í³¼Æ¶ªÏß
//            if(left_edge_line[row] <= 0)
//                Left_Lost_Time++;
//            if(right_edge_line[row] >= 180)
//                Right_Lost_Time++;
//            if(left_edge_line[row] <= 0 && right_edge_line[row] >= 180)
//                Both_Lost_Time++;

//            // ÕÒ×óÓÐÐ§Æðµã£¨´ÓÔ¶µ½½üÓöµ½µÄµÚÒ»¸öÓÐÐ§×ó±ß½ç£©
//            if(Boundry_Start_Left == STOPROW && left_edge_line[row] > 0)
//            {
//                Boundry_Start_Left = row;
//            }
//            // ÕÒÓÒÓÐÐ§Æðµã
//            if(Boundry_Start_Right == STOPROW && right_edge_line[row] < 180)
//            {
//                Boundry_Start_Right = row;
//            }
//        }
//				printf("Left_Lost_Time=%d, Right_Lost_Time=%d, Both_Lost_Time=%d\r\n", 
//       Left_Lost_Time, Right_Lost_Time, Both_Lost_Time);
//printf("Boundry_Start_Left=%d, Boundry_Start_Right=%d\r\n", 
//       Boundry_Start_Left, Boundry_Start_Right);
//    }
    // º¯Êý½áÊø£¬ËùÓÐÍ³¼Æ½á¹ûÒÑ¾­±£´æÔÚÈ«¾Ö±äÁ¿ÖÐ
//}
//		for(row = row_max-2; row >= row_min; row--)
//{
//    // ÕÒ×óÓÐÐ§Æðµã
//    if(left_edge_line[row] >0)
//    {
//        Boundry_Start_Left = row;
//        break;
//    }
//		 // ÕÒÓÒÓÐÐ§Æðµã
//    if(right_edge_line[row]>=180)
//    {
//        Boundry_Start_Right = row;
//			
//        break;
//    }
//		 // Á½¸ö¶¼ÕÒµ½ÁË£¬Ö±½ÓÌáÇ°ÍË³öÑ­»·£¨×îÓÅÐ§ÂÊ£©
//    if(Boundry_Start_Left != 0 && Boundry_Start_Right != 0)
//    {
//        break;
//    }
//		
//}

//for(row = row_max-2; row >= row_min; row--)                   //¸ÄÁËµÄ
//{
//    // ÕÒÓÒÓÐÐ§Æðµã
//    if(right_edge_line[row] <180)
//    {
//        Boundry_Start_Right = row;
//			
//        break;
//    }
//}
// ¼ÆËãÃ¿ÐÐÊµ¼ÊÈüµÀ¿í¶È Road_Wide
//for( i = 0; i < MT9V03X_H; i++)                              //¸ÄÁËµÄ
//{
//    Road_Wide[i] = right_edge_line[i] - left_edge_line[i];

//    // ÏÞ·ù£¬·ÀÖ¹Òì³£Öµ
//    if(Road_Wide[i] < 0) Road_Wide[i] = 0;
//    if(Road_Wide[i] > MT9V03X_W) Road_Wide[i] = MT9V03X_W;
//}
	}
//Îó²îÇóÈ¡
void error_calcul()
{
	  
	  int i;
	  int32 weight_count = 0;
	  int16 left,right,row_error,track_center,img_center;
//    Img_error = 0;
	  int32 Img_error_1=0;
//	  const int16 *weight_table = (Island_State == 4) ? Weight_1 : Weight;
	  Img_error_last=Img_error;
	   // Í¼ÏñÖÐÏß£¨ÓÐ·ûºÅ£¬·¶Î§0~127£©
      img_center = MT9V03X_W / 2;
	  
    for(i = MT9V03X_H - 1; i >= remote_distance[reference_col] - 1; i--)//MT9V03X_H - Search_Stop_Line
    {
        // ÑÏ¸ñ·ÀÖ¹Êý×éÔ½½ç£¨80ÐÐµÄi·¶Î§£º0~79£©
        if(i < 0 || i >= MT9V03X_H) {
            continue;
        }

        // Ç¿ÖÆ×ªÎªÓÐ·ûºÅÀàÐÍ£¬±ÜÃâÎÞ·ûºÅÒç³ö
				
        left = (int16)left_edge_line[i];
        right = (int16)right_edge_line[i];
        
        // ¼ÆËãÊµ¼ÊÖÐÏß
				// ×óµºÓÅÏÈ
if(Left_Island_Flag == 1)
{
    switch(Island_State)
    {
        case 1:
        case 2:
//        case 7:
        case 8:
            track_center = right - 45;
            break;
				
//        case 3:
        case 6:
//            if(h == 1) 
						track_center = left + 45;
            break;
				
        default:
            track_center = (left + right) >> 1;
            break;
    }
}
// ÓÒµºÓÅÏÈ
else if(Right_Island_Flag == 1)
{
    switch(Island_State)
    {
        case 1:
        case 2:
//        case 7:
        case 8:
            track_center = left + 45;
            break;

//        case 3:
        case 6
            :
//            if(h == 13) 
						track_center = right - 45;
            break;

        default:
            track_center = (left + right) >> 1;
            break;
    }
}
// Õý³£Çé¿ö
else
{
    track_center = (left + right) >> 1;
}
        //ÐÞÕýÆ«²î·½Ïò£¬ËõÐ¡ÊýÖµ·¶Î§£¨Êµ¼ÊÖÐÏß - Í¼ÏñÖÐÏß£©
        row_error = track_center - img_center;
        // ¼ÓÈ¨ÀÛ¼Ó£¨×ªÎªfloat£¬±ÜÃâÕûÊýÒç³ö£©
	    if(Island_State==4){
		Img_error_1 += (float)row_error * Weight_1[i];
        // ÀÛ¼ÓÈ¨ÖØ
        weight_count += Weight_1[i];
//				 }else if(start_flag==1){
//					  Img_error_1 += (float)row_error * Weight_2[i];
//        // ÀÛ¼ÓÈ¨ÖØ
//        weight_count += Weight_2[i];
				 }else{
        Img_error_1 += (float)row_error * Weight[i];
        // ÀÛ¼ÓÈ¨ÖØ
        weight_count += Weight[i];
				 }
    }
    	// ¼ÓÈ¨Æ½¾ùÆ«²î
		Img_error = Img_error_1 / weight_count;
		if((Island_State==4||Island_State==3||Island_State==5||Island_State==6)&&Left_Island_Flag==1&&Img_error>-40)
		{
			if(Img_error_last>-40)
			{
				Img_error=-40;
			}else{
				Img_error=Img_error_last;
			}
		}else if((Island_State==4||Island_State==3||Island_State==5||Island_State==6)&&Right_Island_Flag==1&&Img_error<40)
		{
			if(Img_error_last<40)
			{
			Img_error=40;
			}else{
				Img_error=Img_error_last;
			}
		}
//    if((Island_State==4||Island_State==3||Island_State==5||Island_State==6)&&abs(Img_error)<40)
//    if(Img_error_last<40)
//			{
//			Img_error=40;
//           }else{
//				Img_error=Img_error_last;    
//   }
 }
//void UART_Print_Full_Image_Gray(void)
//{
//	  uint16 row;
//	  uint16 col;
//    printf("\r\n========== Õû·ùÍ¼Ïñ»Ò¶ÈÖµ ==========\r\n");
//    for(row = 0; row < MT9V03X_H; row++)
//    {
//        printf("ÐÐ%3d: ", row);
//        for(col = 0; col < MT9V03X_W; col++)
//        {
//            printf("%3d ", mt9v03x_image[row][col]);
//            // Ã¿´òÓ¡10ÁÐ»»ÐÐ£¨±ÜÃâµ¥ÐÐ×Ö·û¹ý³¤£©
//            if((col + 1) % 10 == 0) printf("\r\n      ");
//        }
//        printf("\r\n");
//    }
//    printf("=====================================\r\n");
//}

//void Test_Road_Wide(void)
//{
//    int i;
//    printf("\n==================== ÖðÐÐÈüµÀ¿í¶È²âÊÔ ====================\n");
//    
//    // ÖðÐÐ´òÓ¡ 0 ~ MT9V03X_H-1 È«²¿ÐÐ
//    for(i = 0; i < MT9V03X_H; i++)
//    {
//           printf("ÐÐ[%02d]  Êµ¼Ê¿í¶È=%3d\n",
//               i,
//               Road_Wide[i]);
//    }
//    
//    printf("==========================================================\n\n");
//}


void Image_Pro_1(void){
	//²Î¿¼µã
	get_reference_point((const uint8 *)&mt9v03x_image[0][0]);	//	¿´Í¼Ïñ¶¥²¿¼¸ÐÐ£¬Ëã³ö¡°°×É«ÓÐ¶à°×¡¢ºÚÉ«ÓÐ¶àºÚ¡±
//²Î¿¼ÁÐ
	search_reference_col((const uint8 *)&mt9v03x_image[0][0]);	//´Óµ×²¿ÍùÉÏÉ¨£¬ÕÒ¡°ÄÜ¿´µÃ×îÔ¶µÄÒ»ÁÐ¡±
//ËÑË÷±ß½ç
	search_line((const uint8 *)&mt9v03x_image[0][0]);					//´Ó reference_col ³ö·¢£¬×óÓÒ¸÷É¨³öÒ»ÌõÈüµÀ±ßÏß
//  Test_Road_Wide();
//°Ð×Ó¼ì²â
//	detect_bullseye((const uint8 *)&mt9v03x_image[0][0]);

}
void Image_Pro(void)      
{
	Image_Pro_1();
 //===================== »·µºÈ«Ì×µ÷ÊÔÉÏÎ»»ú´òÓ¡ =====================
//printf("[»·µºµ÷ÊÔ] State=%d | L=%d R=%d | L_Start=%d R_Start=%d\r\n",
//       Island_State,
//       Left_Island_Flag,
//       Right_Island_Flag,
//       Boundry_Start_Left,
//       Boundry_Start_Right);

//printf("[¶ªÏßÍ³¼Æ] L_Lost=%d R_Lost=%d BothLost=%d | ImgErr=%d | SearchLine=%d\r\n",
//       Left_Lost_Time,
//       Right_Lost_Time,
//       Both_Lost_Time,
//       Img_error,
//       Search_Stop_Line);


//printf("--------------------------------------------------------\r\n");

	//°ßÂíÏßÊ¶±ð
//   Zebra_idntify();
//	printf(" ZebraState=%d\n",Zebra_State);
//  //°Ð×Ó¼ì²â
	detect_bullseye((const uint8 *)&mt9v03x_image[0][0]);
//	Ô²»·×´Ì¬»ú
   Island_Detect();
//	//Ö±µÀ¼ì²â
//	 Straight_Detect();
	//Îó²îÇóÈ¡
   error_calcul();				//ÓÃ×óÓÒ±ßÏßËã¡°ÈüµÀÖÐÐÄÔÚÄÄ¡¢³µÆ«ÁË¶àÉÙ¡±

}