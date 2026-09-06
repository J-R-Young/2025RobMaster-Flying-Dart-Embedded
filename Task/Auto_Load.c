#include "Auto_Load.h"

/**
  * @name   Auto_Load_Ctrl
  * @brief  自动装填控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */            
void Auto_Load_Control(void)
{
	static uint8_t Slipway_Alraedy = 0;
	DartCtrlHandle.Trigger_Last = DartCtrlHandle.Trigger_Detect;				// 扳机
	
	Yaw_Vision_Control();		//视觉自瞄识别,同时给金姐发信息
	
	Trigger_Return();			// 发射完成之后，扳机根据键显的值开始移动到目标位置 
	
	if (DartCtrlHandle.ShootTarget == 3)			// 随机移动靶
	{
		DartCtrlHandle.ShootStatus = Shoot_discontinuous;
	}
	else if (rc_ctrl.rc.ch[5] > 1000)
	{
		DartCtrlHandle.ShootStatus = Shoot_Continuous_Three;
	}
	else if (rc_ctrl.rc.ch[5] < 500)
	{
		DartCtrlHandle.ShootStatus = Shoot_Continuous_Two;	
	}
	
	if (DartCtrlHandle.Fire_Detect == 0)
	{
		Fire_Ready_Judge();
	}
	else if ((DartCtrlHandle.Copt_Shoot_Num == 2 && DartCtrlHandle.ShootStatus > 0) 
		|| (DartCtrlHandle.ShootStatus == Shoot_discontinuous && DartCtrlHandle.Chambered_Count == 4))
	{
		Move_Trigger_SetCtrlMode(CtrlMode_position);
		DartCtrlHandle.Move_Trigger.EncTarget = motor_data.Move_Trigger.sumPosition;
		Load_SetCtrlMode(CtrlMode_position);
		DartCtrlHandle.Load.EncTarget = motor_data.Load.sumPosition;
		Lift_SetCtrlMode(CtrlMode_position);
		DartCtrlHandle.Lift.EncTarget = motor_data.Lift.sumPosition;
		Pull_Spring_Left_SetCtrlMode(CtrlMode_position);
		DartCtrlHandle.Pull_Spring_Left.EncTarget = motor_data.Pull_Spring_Left.sumPosition;
		Pull_Spring_Right_SetCtrlMode(CtrlMode_position);
		DartCtrlHandle.Pull_Spring_Right.EncTarget = motor_data.Pull_Spring_Right.sumPosition;
		
		Yaw_SetCtrlMode(CtrlMode_safe);
		Fric_Left_SetCtrlMode(CtrlMode_safe);
		Fric_Right_SetCtrlMode(CtrlMode_safe);
		Small_Firc_SetCtrlMode(CtrlMode_safe);
	}			
	else
	{
		switch (DartCtrlHandle.Auto_Load_Run_Flag)
		{
			case Fire_Ready:   										// 发射
			{
				if (DartCtrlHandle.Lift_Position == Up && DartCtrlHandle.Load_Finish != OK)
				{
					DartCtrlHandle.Load_Position = Load_Return();     //装填送镖
				}
				
				Fric_Press_Out();
				
				Servo_Control();									// 舵机控制发射	
			
				if (DartCtrlHandle.Trigger_Detect == 0 && DartCtrlHandle.Fire_State == OK)  			// 扳机舵机松开
				{
					DartCtrlHandle.Fire_After_Time++;    			// 膛里的微动开关					

				}
				                                                //把这个移出去
				if (DartCtrlHandle.Fire_After_Time > 300)  		// 时间延时，完成发射，滑台才可以下压蓄力，抬升才可以下降
				{
					DartCtrlHandle.Chambered_Count++;				// 发射镖数+1
					
						
					DartCtrlHandle.Auto_Chambered_Count++;			// 自动发射镖数+1
					
					if (DartCtrlHandle.ShootStatus == Shoot_Continuous_Two) //如果是两联发
					{
						if (DartCtrlHandle.Auto_Chambered_Count == 2)
						{
							DartCtrlHandle.Copt_Shoot_Num++;
							DartCtrlHandle.Auto_Chambered_Count = 0;
							DartCtrlHandle.Shoot_Num = 0;
						}
					}
					else if (DartCtrlHandle.ShootStatus == Shoot_Continuous_Three)  //如果是三连发
					{
						if (DartCtrlHandle.Auto_Chambered_Count == 3)
						{
							DartCtrlHandle.Copt_Shoot_Num++;
							DartCtrlHandle.Auto_Chambered_Count = 0;
							DartCtrlHandle.Shoot_Num = 0;
						}
						else if (DartCtrlHandle.Auto_Chambered_Count == 1 && DartCtrlHandle.Copt_Shoot_Num == 1)
						{
							DartCtrlHandle.Copt_Shoot_Num++;
							DartCtrlHandle.Auto_Chambered_Count = 0;
							DartCtrlHandle.Shoot_Num = 0;
						}
					}
					else if (DartCtrlHandle.ShootStatus == Shoot_discontinuous)  //如果是单发
					{
						if (DartCtrlHandle.Auto_Chambered_Count == 1)  //打一发就清零
						{
							DartCtrlHandle.Copt_Shoot_Num++;
							DartCtrlHandle.Auto_Chambered_Count = 0;
							DartCtrlHandle.Shoot_Num = 0;
						}
					}
					
					DartCtrlHandle.Fire_State = 0;
					DartCtrlHandle.Fire_After_Time = 0;             // 计时清0
					DartCtrlHandle.Auto_Load_Run_Flag = Fire_Charge;
					
				}
				break;
			}
			case Fire_Charge:  													//发射蓄力
			{
				Servo_Control();

				if (DartCtrlHandle.Servo_Time < 250 && DartCtrlHandle.Servo_Time > 0)    //等的时间从500改到300
				{
					// 每4发镖第一发不需要降抬升
					if ((DartCtrlHandle.Chambered_Count % 4 != 0) && (DartCtrlHandle.Load_Finish == OK))
					{
						DartCtrlHandle.Lift_Position = Drop_Lift();               //降下抬升               
					}
					
					if (DartCtrlHandle.Lift_Position == Down)
					{
						Fric_Press_In();												// 摩擦轮开始挤压镖体进入膛 
					}
					
					DartCtrlHandle.Pull_Spring_Left.EncTarget = motor_data.Pull_Spring_Left.sumPosition;
					DartCtrlHandle.Pull_Spring_Right.EncTarget = motor_data.Pull_Spring_Right.sumPosition;
					Pull_Spring_Left_SetCtrlMode(CtrlMode_position);
					Pull_Spring_Right_SetCtrlMode(CtrlMode_position);
					
					DartCtrlHandle.Servo_Time++;
				}
				else if (Slipway_Alraedy == 0)
				{
					if ((DartCtrlHandle.Chambered_Count % 4 != 0) && (DartCtrlHandle.Load_Finish == OK))
					{
						DartCtrlHandle.Lift_Position = Drop_Lift();               //降下抬升               
					}
					DartCtrlHandle.Pull_Spring_Position = Compress_the_spring();  // 拉簧下压
				}
				
				if(DartCtrlHandle.Lift_Position == Down && DartCtrlHandle.Load_Switch == 0)
				{
					DartCtrlHandle.Load_Finish = 0;
				}

				DartCtrlHandle.Load_Position = Load_Return(); 

				
				if (DartCtrlHandle.Servo_Time == 250) 			 //滑台和抬升到了低位  
				{
					DartCtrlHandle.Servo_Time = 0;
					
					Slipway_Alraedy = 0;
					
					 if((DartCtrlHandle.Lift_Position == Down && DartCtrlHandle.Chambered_Count % 4 != 0) || (DartCtrlHandle.Lift_Position == Up))
					 {
						DartCtrlHandle.Auto_Load_Run_Flag = Sliding_table_In_Place;  //运行状态到了压入滑台
					 }
				 }
				break;
			}
			case Sliding_table_In_Place:  										// 滑台到位
			{
				Servo_Control();
				
				if(DartCtrlHandle.Lift_Position == Down && DartCtrlHandle.Load_Switch == 0)
				{
					DartCtrlHandle.Load_Finish = 0;
				}
				
				DartCtrlHandle.Pull_Spring_Position = Pull_Spring_Return();    		// 拉簧归位
				
				if (DartCtrlHandle.Load_Switch == 0 && DartCtrlHandle.Press_In_After_Time < 70)
				{
					DartCtrlHandle.Press_In_After_Time++;							// 挤压镖体进入膛后开始计数
				}
				
				if (DartCtrlHandle.Press_In_After_Time == 70) //等到镖体进入膛内后     //由200 改到100
				{
					DartCtrlHandle.Press_In_After_Time = 600;
				}
				
				if (DartCtrlHandle.Press_In_After_Time == 600)
				{
					DartCtrlHandle.Lift_Position = Lift_Return();   			//抬升要回到上位
				}
				
				if (DartCtrlHandle.Lift_Position == Up && DartCtrlHandle.Load_Switch == 0)
				{
					DartCtrlHandle.Load_Position = Load_Return(); 
				}
				
				if ( DartCtrlHandle.Pull_Spring_Position == Up && DartCtrlHandle.Lift_Position == Up)  //抬升上归位，拉簧归位
				{
					DartCtrlHandle.Press_In_After_Time = 0;
					  			//装填送镖     
					DartCtrlHandle.Auto_Load_Run_Flag = Fire_Ready; 			//运行标志位到了准备发射
				}
				break;
			}
		}
	}
}

/**
  * @name   Fric_Press_In
  * @brief  摩擦轮开始将镖体压进
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */  
void Fric_Press_In(void)
{
	DartCtrlHandle.Fric_Left.SpdTarget = -6300;
	DartCtrlHandle.Fric_Right.SpdTarget = 6300;
	
	if (ABS(motor_data.Fric_Left.speed) > 6300)
	{
		DartCtrlHandle.Small_Fric.SpdTarget = -2000;
	}
	Small_Firc_SetCtrlMode(CtrlMode_speed);
	Fric_Left_SetCtrlMode(CtrlMode_speed);
	Fric_Right_SetCtrlMode(CtrlMode_speed);
}


/**
  * @name   Fric_Press_Out
  * @brief  摩擦轮开始将镖体压出
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */ 
void Fric_Press_Out(void)
{
	if (DartCtrlHandle.Load_Switch == 1 && DartCtrlHandle.Load_Finish == OK)
	{
		DartCtrlHandle.Fric_Left.SpdTarget  = 0;
		DartCtrlHandle.Fric_Right.SpdTarget = 0;
		DartCtrlHandle.Small_Fric.SpdTarget = 0;
		Small_Firc_SetCtrlMode(CtrlMode_speed);
		Fric_Left_SetCtrlMode(CtrlMode_speed);
		Fric_Right_SetCtrlMode(CtrlMode_speed);
	}
	else
	{
		DartCtrlHandle.Fric_Left.SpdTarget  =  1200;
		DartCtrlHandle.Fric_Right.SpdTarget = -1200;
		DartCtrlHandle.Small_Fric.SpdTarget = 0;
		Small_Firc_SetCtrlMode(CtrlMode_speed);
		Fric_Left_SetCtrlMode(CtrlMode_speed);
		Fric_Right_SetCtrlMode(CtrlMode_speed);
		DartCtrlHandle.Load_Num++;
	}
	
}

/**
  * @name   Fric_Disable
  * @brief  抬升变化摩擦轮失能
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */ 
void Fric_Disable(void)
{

	DartCtrlHandle.Fric_Left.SpdTarget  = 0;
	DartCtrlHandle.Fric_Right.SpdTarget = 0;
	DartCtrlHandle.Small_Fric.SpdTarget = 0;
	Small_Firc_SetCtrlMode(CtrlMode_speed);
	Fric_Left_SetCtrlMode(CtrlMode_speed);
	Fric_Right_SetCtrlMode(CtrlMode_speed);
	
}

/**
  * @name   Drop_Lift
  * @brief  抬升机构下降
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
int Drop_Lift(void)
{
	if (ABS(motor_data.Lift.sumPosition - DartCtrlHandle.Lift.PositionEnd) > 1000)
	{
		Fric_Disable();
		DartCtrlHandle.Lift.EncTarget = DartCtrlHandle.Lift.PositionEnd;
		Lift_SetCtrlMode(CtrlMode_position);	
	
		return Running ;
	}
	else
	{
		return Down ;
	}

}


/**
  * @name   Compress_the_spring
  * @brief  拉簧将滑台压进
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
int Compress_the_spring(void)
{
	DartCtrlHandle.Trigger_Detect = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);  //读飞镖上膛的扳机上的微动开关
	
	// 微动接触
	if (DartCtrlHandle.Trigger_Detect == 1)
	{
		DartCtrlHandle.Servo_Time++;
	}
	
		// 拉簧电机
	// 完成标志是滑台撞到微动   Trigger_Detect等于0 还没有扣上，需要扣上，带延时
	if(DartCtrlHandle.Trigger_Detect == 0)
	{
		if (ABS(motor_data.Pull_Spring_Left.sumPosition - DartCtrlHandle.Pull_Spring_Left.PositionZero) < 950000)
		{
			DartCtrlHandle.Pull_Spring_Enc_Target_Up -= 800;
		}
		else
		{
			DartCtrlHandle.Pull_Spring_Enc_Target_Up -= 600;
		}
		
		motor_pid.Pull_Spring_Left_Position.outMax   = 7300;
		
		motor_pid.Pull_Spring_Right_Position.outMax   = 7300;
		
		DartCtrlHandle.Pull_Spring_Left.EncTarget = DartCtrlHandle.Pull_Spring_Left.PositionZero - DartCtrlHandle.Pull_Spring_Enc_Target_Up;
		Pull_Spring_Left_SetCtrlMode(CtrlMode_position);
		
		DartCtrlHandle.Pull_Spring_Right.EncTarget = DartCtrlHandle.Pull_Spring_Right.PositionZero + DartCtrlHandle.Pull_Spring_Enc_Target_Up - Pull_Spring_Dif;
		Pull_Spring_Right_SetCtrlMode(CtrlMode_position);
		
		DartCtrlHandle.Pull_Spring_Left.EncTarget = Data_Limit(DartCtrlHandle.Pull_Spring_Left.EncTarget, DartCtrlHandle.Pull_Spring_Left.PositionZero, DartCtrlHandle.Pull_Spring_Left.PositionEnd);
		DartCtrlHandle.Pull_Spring_Right.EncTarget = Data_Limit(DartCtrlHandle.Pull_Spring_Right.EncTarget, DartCtrlHandle.Pull_Spring_Right.PositionEnd, DartCtrlHandle.Pull_Spring_Right.PositionZero);
	
		return Running ;
	}
	else if (DartCtrlHandle.Trigger_Detect == 1)  //滑台压到微动开关并置位，标志着滑台已经压入扳机
	{
		DartCtrlHandle.Pull_Spring_Enc_Target_Up = 0;
		return Down ;
	}
	
}

/**
  * @name   Fire_Ready_Judge
  * @brief  开火准备判断
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
void Fire_Ready_Judge(void)
{
	static uint8_t Slipway_Alraedy = 0;
	
	DartCtrlHandle.Chambered_Count = 0;
	
	Trigger_Return();								// 发射完成之后，扳机根据键显的值开始移动到目标位置
	
	if (DartCtrlHandle.Fire_Detect == 0)
	{
		// 进入比赛模式之后，需要检测1：是否已经有一发飞镖已经上膛，2：是否第二发飞镖已经在装填里，3：装填电机是否在最下面
		
		Servo_Control();									// 舵机控制发射
		DartCtrlHandle.Load_Position = Load_Return();       
		
		if (DartCtrlHandle.Servo_Time >= 200 || Slipway_Alraedy == 1)   			//判断滑台是否已经扣上
		{
			Slipway_Alraedy = 1;
			if (DartCtrlHandle.Lift_Position == Down)
			{
				Fric_Press_In();
				if (DartCtrlHandle.Load_Switch == 0 && DartCtrlHandle.Press_In_After_Time < 300)
				{
					DartCtrlHandle.Press_In_After_Time++;							// 挤压镖体进入膛后开始计数
				}

				if (DartCtrlHandle.Press_In_After_Time == 100) //等到镖体进入膛内后     //由200 改到100
				{
					DartCtrlHandle.Press_In_After_Time = 600;
				}
			}

			if (DartCtrlHandle.Press_In_After_Time == 600)
			{
				DartCtrlHandle.Lift_Position = Lift_Return();   			//抬升要回到上位
			}
			DartCtrlHandle.Pull_Spring_Position = Pull_Spring_Return();  // 拉簧电机是否归位判断，若没有归位，则控制拉簧归位	
		}
		else if (DartCtrlHandle.Servo_Time < 200 && DartCtrlHandle.Servo_Time > 0)
		{
			DartCtrlHandle.Pull_Spring_Left.EncTarget = motor_data.Pull_Spring_Left.sumPosition;
			DartCtrlHandle.Pull_Spring_Right.EncTarget = motor_data.Pull_Spring_Right.sumPosition;
			Pull_Spring_Left_SetCtrlMode(CtrlMode_position);
			Pull_Spring_Right_SetCtrlMode(CtrlMode_position);
			DartCtrlHandle.Lift_Position = Drop_Lift();           		  // 抬升电机在下方
			DartCtrlHandle.Servo_Time++;
		}
		else if (Slipway_Alraedy == 0)
		{
			DartCtrlHandle.Lift_Position = Drop_Lift();           		  // 抬升电机在下方
			
			if (ABS(DartCtrlHandle.screw_2006_num - DartCtrlHandle.Screw_Target_Num[0]) < 2)
			{
				DartCtrlHandle.Pull_Spring_Position = Compress_the_spring();  // 拉簧下压
			}
		}
		
		
				
	// 各个电机已就,装填中有一发，剩余三发，共四发
		
		if (DartCtrlHandle.Pull_Spring_Position == Up && DartCtrlHandle.Lift_Position == Up && DartCtrlHandle.Load_Position == Mid && DartCtrlHandle.Load_Switch == 1 && DartCtrlHandle.Load_Finish == OK)     //剩余两发
		{	
			DartCtrlHandle.Servo_Time = 0;
			DartCtrlHandle.Press_In_After_Time = 0;
			Slipway_Alraedy = 0;
			DartCtrlHandle.Fire_Detect = 1;              //开火就位，且作为标志位，下一次不进入判断
			DartCtrlHandle.Auto_Load_Run_Flag = Fire_Ready;
			DartCtrlHandle.Load_Num = 0;

		}
	}
}
/**
  * @name   Load_Return
  * @brief  装填电机归位控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
int Load_Return(void)
{
	DartCtrlHandle.Load_Switch = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5);
	
	if (DartCtrlHandle.Fire_Detect == 0)
	{
		if (DartCtrlHandle.Lift_Position == Up && DartCtrlHandle.Press_In_After_Time == 600)
		{
			DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load_Position_Mid;
			DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
			Load_SetCtrlMode(CtrlMode_position);
			Fric_Press_Out();
		}
		
	}
	else if ( DartCtrlHandle.Lift_Position == Up)	//DartCtrlHandle.Chambered_Count % 4 != 0 &&		// 有问题
	{
		if (DartCtrlHandle.Load_Switch == 0 && ABS(motor_data.Load.sumPosition - motor_data.Load.positionTarget) < 2000 
			&& DartCtrlHandle.Load.EncTarget == DartCtrlHandle.Load.PositionZero && DartCtrlHandle.Press_In_After_Time == 600)
		{
			DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load_Position_Mid;
			DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
			Load_SetCtrlMode(CtrlMode_position);
		}
		else if (DartCtrlHandle.Load_Switch == 0 && ABS(motor_data.Load.sumPosition - motor_data.Load.positionTarget) < 2000 
			&& DartCtrlHandle.Load.EncTarget == DartCtrlHandle.Load_Position_Mid && DartCtrlHandle.Press_In_After_Time == 600)
		{
			DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load_Position_Mid_New;
			DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
			Load_SetCtrlMode(CtrlMode_position);
		}
		else if (DartCtrlHandle.Load_Switch == 0 && ABS(motor_data.Load.sumPosition - motor_data.Load.positionTarget) < 2000 
			&& DartCtrlHandle.Load.EncTarget == DartCtrlHandle.Load_Position_Mid_New && DartCtrlHandle.Press_In_After_Time == 600)
		{
			DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load.PositionEnd;
			DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
			Load_SetCtrlMode(CtrlMode_position);
		}
		else if (DartCtrlHandle.Load_Switch == 0 && ABS(motor_data.Load.sumPosition - motor_data.Load.positionTarget) < 2000 
			&& DartCtrlHandle.Load.EncTarget == DartCtrlHandle.Load.PositionEnd && DartCtrlHandle.Press_In_After_Time == 600)
		{
			DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load.PositionZero;
			DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
			Load_SetCtrlMode(CtrlMode_position);
		}	
	}

	
	if(DartCtrlHandle.Load.EncTarget == DartCtrlHandle.Load.PositionZero 	//装填到达了低位期望
		&& ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load.PositionZero) < 2000
		&& DartCtrlHandle.Load_Switch == 1 && DartCtrlHandle.Load_Num > 100)
	{
		DartCtrlHandle.Load_Finish = OK;
		DartCtrlHandle.Load_Num = 0;
		return Down;
	}
	
	if(DartCtrlHandle.Load.EncTarget == DartCtrlHandle.Load_Position_Mid 	//装填到达了中位期望
		&& ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load_Position_Mid) < 2000
		&& DartCtrlHandle.Load_Switch == 1 && DartCtrlHandle.Load_Num > 100)
	{
		DartCtrlHandle.Load_Num = 0;
		DartCtrlHandle.Load_Finish = OK;
		return Mid;
	}
	
	if(DartCtrlHandle.Load.EncTarget == DartCtrlHandle.Load_Position_Mid_New 	//装填到达了中位期望
		&& ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load_Position_Mid_New) < 2000
		&& DartCtrlHandle.Load_Switch == 1 && DartCtrlHandle.Load_Num > 100)
	{
		DartCtrlHandle.Load_Num = 0;
		DartCtrlHandle.Load_Finish = OK;
		return Mid_New;
	}
	
	if(DartCtrlHandle.Load.EncTarget == DartCtrlHandle.Load.PositionEnd   	//装填到达了高位期望
		&& ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load.PositionEnd) < 2000
		&& DartCtrlHandle.Load_Switch == 1 && DartCtrlHandle.Load_Num > 100)
	{
		DartCtrlHandle.Load_Num = 0;
		DartCtrlHandle.Load_Finish = OK;
		return Up;
	}
}


/**
  * @name   Pull_Spring_Return
  * @brief  拉簧电机归位控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
int Pull_Spring_Return(void)
{
	uint8_t Left_Spring_Position= 0;
	uint8_t Right_Spring_Position= 0;
	
	if (ABS(motor_data.Pull_Spring_Left.sumPosition - DartCtrlHandle.Pull_Spring_Left.PositionZero) > 2000)
	{
		DartCtrlHandle.Pull_Spring_Left.EncTarget = DartCtrlHandle.Pull_Spring_Left.PositionZero;
		
		motor_pid.Pull_Spring_Left_Position.outMax   = 4000;
		
		Pull_Spring_Left_SetCtrlMode(CtrlMode_position);
	}
	else
	{
		Left_Spring_Position = Up;
	}
	
	
	if (ABS(motor_data.Pull_Spring_Right.sumPosition - DartCtrlHandle.Pull_Spring_Right.PositionZero) > 2000)
	{
		DartCtrlHandle.Pull_Spring_Right.EncTarget = DartCtrlHandle.Pull_Spring_Right.PositionZero;
		
		motor_pid.Pull_Spring_Right_Position.outMax   = 4000;
		
		Pull_Spring_Right_SetCtrlMode(CtrlMode_position);
	}
	else
	{
		Right_Spring_Position = Up;
	}
	
	if (Left_Spring_Position == Up && Right_Spring_Position == Up)
	{
		DartCtrlHandle.Servo_Time = 0;
		return Up;
	}
	
	

}

/**
  * @name   Lift_Return
  * @brief  抬升电机上升判断及控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
int Lift_Return(void)
{
	
	if (ABS(motor_data.Lift.sumPosition - DartCtrlHandle.Lift.PositionZero) > 1000)
	{
		Fric_Disable();
		DartCtrlHandle.Lift.EncTarget = DartCtrlHandle.Lift.PositionZero;
		Lift_SetCtrlMode(CtrlMode_position);	
		return Running;
	}
	else
	{
		return Up;
	}
}

/**
  * @name   Trigger_Return
  * @brief  扳机电机上升判断及控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
void Trigger_Return(void)
{
	//控制扳机根据键显输入的值开始移动
	DartCtrlHandle.Move_Trigger_Target_Num = DartCtrlHandle.Screw_Target_Num[DartCtrlHandle.Chambered_Count] * 36;

	DartCtrlHandle.Move_Trigger.EncTarget = DartCtrlHandle.Move_Trigger.PositionZero + DartCtrlHandle.Move_Trigger_Target_Num * 8192 ;

	Move_Trigger_SetCtrlMode(CtrlMode_position);	
	
	DartCtrlHandle.screw_2006_num = motor_data.Move_Trigger.turns / 36.0;      //键显参数显示
	
}

/**
  * @name   Trigger_Return
  * @brief  Yaw轴电机自瞄控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
int Pixel_Offset = 0;   //经过处理过后的像素偏置
uint8_t get_err_0_flag = 0;
int Yaw_position_last = 0;
void Yaw_Vision_Control(void)
{
	DartCtrlHandle.ShootTarget = (Dart_info.dart_info >> 6) & 0X03;   //
	
	Usart_SendByte(&huart1,DartCtrlHandle.ShootTarget); //和金姐通信  1：固定靶 2：随机固定靶  3：随机移动靶
	
	if ( (DartCtrlHandle.Offset[DartCtrlHandle.Chambered_Count] / 1000) == 1)  //根据键显输入的偏置，进行处理   如果像输入负数的话，就在千位输入1
	{
		Pixel_Offset = -DartCtrlHandle.Offset[DartCtrlHandle.Chambered_Count] % 1000;
	}
	else
	{
		Pixel_Offset = DartCtrlHandle.Offset[DartCtrlHandle.Chambered_Count];
	}
	
	if (VisionData.IdentifyFlag == 1)   // VisionData.IdentifyFlag == 1 标志着瞄到绿灯
	{
		DartCtrlHandle.never_seen_green = 2;    //
		
		DartCtrlHandle.Lost_Vision_CMD = 0;   
			
		DartCtrlHandle.Vision_Time = 8000;
		
		if (get_err_0_flag == 0)
		{
			if (ABS(VisionData.x_pixel_err)<=1)
			{
				DartCtrlHandle.Yaw.SpdTarget = 0;   //正偏差镖架向右，即速度期望是正数时，镖架向右运动
				
				Yaw_SetCtrlMode(CtrlMode_speed);
			}
			else
			{
				DartCtrlHandle.Yaw.SpdTarget = 10.0 * (VisionData.x_pixel_err);// - Pixel_Offset);   //正偏差镖架向右，即速度期望是正数时，镖架向右运动

				Yaw_SetCtrlMode(CtrlMode_speed);
			}
		}


		// 6020编码器的值可能会存在跳变，导致会有一个I项的累加，导致长时间累加过后，可能I项会越来越大，或导致6020小范围的抖动
		//为了消除这个抖动，加了此程序，当在一秒钟内，电机的编码器没有跳变，那就将电机的输出值清空
		if (motor_data.Yaw.position == motor_data.Yaw.lastPosition && DartCtrlHandle.Yaw.SpdTarget == 0)
		{
			DartCtrlHandle.Yaw_Time_Count++;
			if (DartCtrlHandle.Yaw_Time_Count >= 1000)
			{
				PID_cleardata(&motor_pid.Yaw_Speed);
				DartCtrlHandle.Yaw_Time_Count = 0;
			}
		}
		else
		{
			DartCtrlHandle.Yaw_Time_Count = 0;
		}
	}
	else if (rc_ctrl.rc.ch[7] != rc_ctrl.rc.ch_last[7] && VisionData.IdentifyFlag == 0)
	{
		DartCtrlHandle.never_seen_green = 1;
	}
	else if (DartCtrlHandle.Shoot_Num == 10 && VisionData.IdentifyFlag == 0 && DartCtrlHandle.Time <= 7500) 
	{
		
		if (VisionData.IdentifyFlag == 0 && DartCtrlHandle.Vision_Last_State == 1)   //上一次还可以看到绿灯，但是下一次就看不到
		{
			DartCtrlHandle.never_seen_green = 0;
			
			DartCtrlHandle.Lost_Vision_CMD = 1;    //等于一，意味着丢失了绿灯视野
			
			DartCtrlHandle.Vision_Time = 8000;		//1.6秒的等待时间
		}
		if (DartCtrlHandle.Lost_Vision_CMD == 1)
		{
			if (DartCtrlHandle.Vision_Time <= 8000 && DartCtrlHandle.Vision_Time >= 0)
			{
				DartCtrlHandle.Vision_Time--;    //1.6秒以内，镖架不动
				DartCtrlHandle.Yaw.SpdTarget = 0 ;    
				Yaw_SetCtrlMode(CtrlMode_speed);
				if (DartCtrlHandle.Vision_Time <= 0)
				{
					DartCtrlHandle.Lost_Vision_CMD = 2;
				}
			}
		}	
		 else if (DartCtrlHandle.Lost_Vision_CMD == 2 || DartCtrlHandle.never_seen_green == 1)
		{
			DartCtrlHandle.Yaw.SpdTarget = 200 ;    //如果没看到绿灯的话，镖架就一直向右走

			Yaw_SetCtrlMode(CtrlMode_speed);
		}

	}
	else if (DartCtrlHandle.Shoot_Num == 0 && VisionData.IdentifyFlag == 0)    //舱门关闭状态，未识别到绿灯
	{
		DartCtrlHandle.Yaw.SpdTarget = 0;   

		Yaw_SetCtrlMode(CtrlMode_speed);
	}
}

/**
  * @name   Yaw_Return
  * @brief  Yaw轴电机控制镖架去到最右边
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
Position_enum Yaw_Return(void)
{
	if (ABS(DartCtrlHandle.Yaw.PositionEnd - motor_data.Yaw.sumPosition) <= 5000)  //判断是否到达最右边
	{
		return Right;
	}
	else
	{
		DartCtrlHandle.Yaw.EncTarget = DartCtrlHandle.Yaw.PositionEnd;  //先去到最右边
		Lift_SetCtrlMode(CtrlMode_position);							//用位置控制
		return Waiting;
	}

}
