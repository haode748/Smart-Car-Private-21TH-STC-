#include "pid.h"
float p_=0;
float d_=0;
//??????¦Ë
uint8 flage=0;
RS_Incremental_PID Pid_vl;
RS_Incremental_PID Pid_vr;
RS_Incremental_PID Pid_turn;
RS_Incremental_PID Pid_wheel_turn;
//???
RS_Incremental_PID Pidzd_vl;
RS_Incremental_PID Pidzd_vr;
RS_Incremental_PID Pidzd_turn;
float kp2= 0.0002;
float kd2= 0.01;

//????
int32 car_dir,vl_ex,vr_ex , car_wheel_dir, pwm_r;
void PID_Start_init(void)
{
//???????
Pid_vl.Kp = 2.7;
Pid_vl.Ki =3.0;
Pid_vl.Kd = 0.0;
Pid_vl.Target = 70;
    
////??????????
//Pidzd_vl.Kp = 1.0;
//Pidzd_vl.Ki =0.25;
//Pidzd_vl.Kd = 0.0;
//Pidzd_vl.Target = 90;
//???????
Pid_vr.Kp = 2.7;
Pid_vr.Ki = 3.0;
Pid_vr.Kd = 0;
Pid_vr.Target = 70;

//    //??????????
//    Pidzd_vr.Kp = 1.0;
//Pidzd_vr.Ki = 0.35;
//Pidzd_vr.Kd = 0.032;
//Pidzd_vr.Target = 90;

//???(???)
Pid_turn.Kp = -0.38;
Pid_turn.Kd = -1.5;
Pid_turn.Target = 0;



//???????
Pid_wheel_turn.Kp = 1.3 ;
Pid_wheel_turn.Kd = 0.3;
Pid_wheel_turn.Target = 0;

}

//#include "pid.h"
//float p_=0;
//float d_=0;
////??????¦Ë
//uint8 flage=0;
//RS_Incremental_PID Pid_vl;
//RS_Incremental_PID Pid_vr;
//RS_Incremental_PID Pid_turn;
//RS_Incremental_PID Pid_wheel_turn;
////???
//RS_Incremental_PID Pidzd_vl;
//RS_Incremental_PID Pidzd_vr;
//RS_Incremental_PID Pidzd_turn;
//float kp2= 0.00018;
//float kd2=0.013;
////float kd2=0.0;
////????
//int32 car_dir,vl_ex,vr_ex , car_wheel_dir, pwm_r;
//void PID_Start_init(void)
//{
////???????
//Pid_vl.Kp = 5;
//Pid_vl.Ki =1.6;
//Pid_vl.Kd = 0.0;
//Pid_vl.Target = 155;
//    
//////??????????
////Pidzd_vl.Kp = 1.0;
////Pidzd_vl.Ki =0.25;
////Pidzd_vl.Kd = 0.0;
////Pidzd_vl.Target = 90;
////???????
//Pid_vr.Kp = 5;
//Pid_vr.Ki = 1.6;
//Pid_vr.Kd = 0.0;
//Pid_vr.Target = 155;

////    //??????????
////    Pidzd_vr.Kp = 1.0;
////Pidzd_vr.Ki = 0.35;
////Pidzd_vr.Kd = 0.032;
////Pidzd_vr.Target = 90;

////???
//Pid_turn.Kp = -0.33;
//Pid_turn.Kd = -3;
//Pid_turn.Target = 0;



////???????
//Pid_wheel_turn.Kp = 1.7;
//Pid_wheel_turn.Kd = 0.2;
//Pid_wheel_turn.Target = 0;

//}
//?????PID
int32 IncrementalPID_Calculate(RS_Incremental_PID *pid , int32 wheel_ex )
{ 
  pid->Err = wheel_ex - (int32)pid->Measure;					// ??? = ??? - ???
  
	//????
	
  pid->p_out = pid->Kp * (pid->Err - pid->Last_Err);
  pid->i_out = pid->Ki * pid->Err;
  pid->d_out = pid->Kd * (pid->Err - 2*pid->Last_Err + pid->Previous_Err);
  
  pid->Output += pid->p_out + pid->i_out + pid->d_out; 		// ?? ???
  
  pid->Previous_Err = pid->Last_Err;
  pid->Last_Err =  pid->Err;
	
	pid->Output = Clip(pid->Output,-7000 , 7000); // ???????????????????????????
	return pid->Output;
}



////?????PID
//int32 IncrementalPID_Calculate(RS_Incremental_PID *pid , int32 wheel_ex )
//{ 
//		
//	// ??????????
//pid->Err = wheel_ex - pid->Measure;

//// ???????????????????
//if (abs(pid->Err) > INTEGRAL_SEP_THRESHOLD) {
//    pid->i_out = 0;
//} else {
//    pid->i_out = pid->Ki * pid->Err;
//    // ??????? i_out ????????????? ??200
//    pid->i_out = Clip(pid->i_out, -200, 200);
//}

//pid->p_out = pid->Kp * (pid->Err - pid->Last_Err);
//pid->d_out = pid->Kd * (pid->Err - 2*pid->Last_Err + pid->Previous_Err);

//pid->Output += pid->p_out + pid->i_out + pid->d_out;
//pid->Output = Clip(pid->Output, -7000, 7000);
//	return pid->Output;
//}



//¦Ë???PD
int32 Postion_PID_Cal(RS_Incremental_PID *pid , int32 error)
{
  pid->Err = error;
  
  pid->p_out = pid->Kp * pid->Err;
  pid->d_out = pid->Kd * (pid->Err - pid->Last_Err) ;
  
  pid->Output = pid->p_out + pid->d_out; 
  
  pid->Last_Err =  pid->Err;
	
	pid->Output = Clip(pid->Output,-1000 , 1000); // ???
//	pid->Output=-pid->Output;
	return pid->Output;
}

//¦Ë???PD(????????)
int32 Postion_PI2D_Cal(RS_Incremental_PID *pid , int32 error ,float groyz ,float kd2)
{
  pid->Err = error;
	pid->p_out = pid->Kp * pid->Err;
	pid->d_out = pid->Kd * (pid->Err - pid->Last_Err) + groyz*kd2;

	pid->Output = pid->p_out + pid->d_out;
	
	pid->Last_Err =  pid->Err;
	
	pid->Output = Clip(pid->Output,-7000 , 7000); // ???
	
	return pid->Output;
}



//??????
int32 Absolute(int32 value)
{
  if(value < 0) value = -value;
	return value;
}

//???PD????
int32 Postion_P2ID_Cal(RS_Incremental_PID *pid , int32 error ,float groyz , float kp2, float kd2)
{
//	if(error > 5 || error < -5) {kd2 = 0;}
//	else{kd2  = 0.022;}
  pid->Err = error;
	pid->p_out = pid->Kp * pid->Err + pid->Err*pid->Err*kp2;
	pid->d_out = pid->Kd * (pid->Err - pid->Last_Err) + groyz*kd2;

	pid->Output = pid->p_out + pid->d_out;
	
	pid->Last_Err =  pid->Err;
	
	pid->Output = Clip(pid->Output,-5000 , 5000); // ???
	   p_=pid->p_out;
	d_=pid->d_out;

	return pid->Output;
}