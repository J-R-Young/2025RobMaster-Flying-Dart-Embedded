#include "Dart.h"
#include "headfiles.h"

// 扳机
static PID_InitTypeDef  PID_Move_Trigger_Position_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 0.1,
    .ki         = 0.0,
    .kd         = 0,
    .iOutMax    = 300,
    .outMax     = 4000, //12000
};

static PID_InitTypeDef  PID_Move_Trigger_Speed_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 3,
    .ki         = 0.01,
    .kd         = 7,
    .iOutMax    = 3000,
    .outMax     = 10000, //12000
};

// 抬升
static PID_InitTypeDef  PID_Lift_Position_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 0.5,
    .ki         = 0.0,
    .kd         = 0,
    .iOutMax    = 200,
    .outMax     = 3200, //12000
};

static PID_InitTypeDef  PID_Lift_Speed_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 8.60,
    .ki         = 0.02,
    .kd         = 5,
    .iOutMax    = 3000,
    .outMax     = 10000, //12000
};

// 拉簧左
static PID_InitTypeDef  PID_Pull_Spring_Left_Position_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 0.1,
    .ki         = 0.0,
    .kd         = 0,
    .iOutMax    = 200,
    .outMax     = 7300, //12000
};

static PID_InitTypeDef  PID_Pull_Spring_Left_Speed_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 10.0,
    .ki         = 0.5,
    .kd         = 0,
    .iOutMax    = 9000,
    .outMax     = 16300, //12000
};

// 拉簧右
static PID_InitTypeDef  PID_Pull_Spring_Right_Position_init =
{
   .mode       = PID_MODE_POSITION,          
    .kp         = 0.1,
    .ki         = 0.0,
    .kd         = 0,
    .iOutMax    = 200,
    .outMax     = 7300, //12000
};

static PID_InitTypeDef  PID_Pull_Spring_Right_Speed_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 10.0,
    .ki         = 0.5,
    .kd         = 0,
    .iOutMax    = 9000,
    .outMax     = 12300, //12000
};

// Yaw
static PID_InitTypeDef  PID_Yaw_Position_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 5.0,
    .ki         = 0.0,
    .kd         = 2,
    .iOutMax    = 300,
    .outMax     = 1000, //12000
};

static PID_InitTypeDef  PID_Yaw_Speed_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 62.3,
    .ki         = 0.472,
    .kd         = 2,
    .iOutMax    = 7900,
    .outMax     = 25000,
};

// 左摩擦轮
static PID_InitTypeDef  PID_Fric_Left_Position_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 4.0,
    .ki         = 0.02,
    .kd         = 5,
    .iOutMax    = 3000,
    .outMax     = 10000, //12000
};

static PID_InitTypeDef  PID_Fric_Left_Speed_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 10.0,
    .ki         = 0.0,
    .kd         = 0,
    .iOutMax    = 9000,
    .outMax     = 12000, //12000
};


// 右摩擦轮
static PID_InitTypeDef  PID_Fric_Right_Position_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 10.0,
    .ki         = 0.0,
    .kd         = 0,
    .iOutMax    = 9000,
    .outMax     = 12000, //12000
};

static PID_InitTypeDef  PID_Fric_Right_Speed_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 10.0,
    .ki         = 0.0,
    .kd         = 0,
    .iOutMax    = 9000,
    .outMax     = 12000, //12000
};

// 小摩擦轮
static PID_InitTypeDef  PID_Small_Fric_Position_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 4.0,
    .ki         = 0.02,
    .kd         = 5,
    .iOutMax    = 3000,
    .outMax     = 10000, //12000
};

static PID_InitTypeDef  PID_Small_Fric_Speed_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 4.0,
    .ki         = 0.02,
    .kd         = 5,
    .iOutMax    = 3000,
    .outMax     = 10000, //12000
};

// 装填
static PID_InitTypeDef  PID_Load_Position_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 0.889,
    .ki         = 0.0,
    .kd         = 0.3,
    .iOutMax    = 200,
    .outMax     = 3980, //12000
};

static PID_InitTypeDef  PID_Load_Speed_init =
{
    .mode       = PID_MODE_POSITION,          
    .kp         = 8.7,
    .ki         = 0.0,
    .kd         = 5,
    .iOutMax    = 1500,
    .outMax     = 9780, //12000
};


/* ============ 全局变量声明 ============= */
DartCtrlHandle_TypeDef DartCtrlHandle = {0};

/* ============ 内部函数声明 ================= */
static float Move_TriggerCtrl_PositionCtrl(void);
//static float Move_TriggerCtrl_SpeedCtrl(void);

static float Load_PositionCtrl(void);
static float Load_SpeedCtrl(void);

static float Lift_PositionCtrl(void);
static float Lift_SpeedCtrl(void);

static float Pull_Spring_Left_PositionCtrl(void);
static float Pull_Spring_Left_SpeedCtrl(void);

static float Pull_Spring_Right_PositionCtrl(void);
static float Pull_Spring_Right_SpeedCtrl(void);

static float Yaw_PositionCtrl(void);
static float Yaw_SpeedCtrl(void);

static float Fric_Left_SpeedCtrl(void);

static float Fric_Right_SpeedCtrl(void);

static float Small_Fric_SpeedCtrl(void);

/* ============ 函数定义 ================= */
/**
  * @name   Dart_AutoHome
  * @brief  飞镖电机初始化
  * @call   External
  * @param  NULL
  * @RetVal NULL
  */
void Dart_AutoHome(void)
{
	// 静态初始化
	
	//左右摩擦轮速度环PID参数初始化
    PID_init(&motor_pid.Fric_Left_Speed, &PID_Fric_Left_Speed_init);
    PID_init(&motor_pid.Fric_Right_Speed, &PID_Fric_Right_Speed_init);
	
	//抬升PID参数初始化
	PID_init(&motor_pid.Lift_Position, &PID_Lift_Position_init);
    PID_init(&motor_pid.Lift_Speed, &PID_Lift_Speed_init);
	
	//装填PID参数初始化
    PID_init(&motor_pid.Load_Position, &PID_Load_Position_init);
    PID_init(&motor_pid.Load_Speed, &PID_Load_Speed_init);

	//扳机PID参数初始化
	PID_init(&motor_pid.Move_Trigger_Position, &PID_Move_Trigger_Position_init);
    PID_init(&motor_pid.Move_Trigger_Speed, &PID_Move_Trigger_Speed_init);
	
	//左右拉簧PID参数初始化
    PID_init(&motor_pid.Pull_Spring_Left_Position, &PID_Pull_Spring_Left_Position_init);
    PID_init(&motor_pid.Pull_Spring_Left_Speed, &PID_Pull_Spring_Left_Speed_init);
	
	PID_init(&motor_pid.Pull_Spring_Right_Position, &PID_Pull_Spring_Right_Position_init);
    PID_init(&motor_pid.Pull_Spring_Right_Speed, &PID_Pull_Spring_Right_Speed_init);

	//小摩擦轮速度环PID参数初始化
	PID_init(&motor_pid.Small_Fric_Speed, &PID_Small_Fric_Speed_init);
	
	//Yaw6020 PID参数初始化
	PID_init(&motor_pid.Yaw_Position, &PID_Yaw_Position_init);
	PID_init(&motor_pid.Yaw_Speed, &PID_Yaw_Speed_init);
	
	// 动态初始化

}/* Gimbal_AutoHome() */

/**
  * @name   飞镖控制总函数
  * @brief  在线程中调用的唯一函数
  * @call   
  * @param  
  * @RetVal 
  */
void Dart_Control(void)
{
	if (hCtrl.Reset_Flag == 0)
	{
		DartCtrlHandle.Reset_AutoHome = 1;
	}
	
	//设备在线检测
	DartCtrlHandle.Lift.Online_State = Device_GetState(DeviceID_Lift);
	DartCtrlHandle.Load.Online_State = Device_GetState(DeviceID_Load);
	DartCtrlHandle.Move_Trigger.Online_State = Device_GetState(DeviceID_Move_Trigger);
	DartCtrlHandle.Pull_Spring_Left.Online_State = Device_GetState(DeviceID_Pull_Spring_Left);
	DartCtrlHandle.Pull_Spring_Right.Online_State = Device_GetState(DeviceID_Pull_Spring_Right);
	
	//5月8日加入Yaw轴电机的初始化
	DartCtrlHandle.Yaw.Online_State = Device_GetState(DeviceID_Yaw);  //Yaw轴电机设备在线检测
	
	DartCtrlHandle.Pull_Spring_Left_IO =  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);      //3508链条的开关
	DartCtrlHandle.Pull_Spring_Right_IO =  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);      //3508链条的开关

	/*--------判断部分--------*/
	//给出期望
	if (DartCtrlHandle.Reset_AutoHome == 1)
	{
		// 电机在线判断
		if ( DartCtrlHandle.Lift.Online_State == DeviceState_Online
		  && DartCtrlHandle.Load.Online_State == DeviceState_Online
		  && DartCtrlHandle.Pull_Spring_Left.Online_State == DeviceState_Online
		  && DartCtrlHandle.Pull_Spring_Right.Online_State == DeviceState_Online )
		{
			DartCtrlHandle.Motor_Online = 1;
		}
		
		// 电机校准
		if (DartCtrlHandle.Motor_Online == 1)
		{
			Trigger_AutoHome();
			Load_AutoHome();  
			Lift_AutoHome();
			Pull_Spring_Left_AutoHome();
			Pull_Spring_Right_AutoHome();
			
		}
		
		// 校准完成
		if ( DartCtrlHandle.Lift.AutoHome == AutoHomeStatus_Ready
		  && DartCtrlHandle.Load.AutoHome == AutoHomeStatus_Ready
		  && DartCtrlHandle.Move_Trigger.AutoHome == AutoHomeStatus_Ready
		  && DartCtrlHandle.Pull_Spring_Left.AutoHome == AutoHomeStatus_Ready
		  && DartCtrlHandle.Pull_Spring_Right.AutoHome == AutoHomeStatus_Ready) 
		{
			DartCtrlHandle.Lift.AutoHome = AutoHomeStatus_Default;
			DartCtrlHandle.Load.AutoHome = AutoHomeStatus_Default;
			DartCtrlHandle.Move_Trigger.AutoHome = AutoHomeStatus_Default;
		    DartCtrlHandle.Pull_Spring_Left.AutoHome = AutoHomeStatus_Default;
			DartCtrlHandle.Pull_Spring_Right.AutoHome = AutoHomeStatus_Default;
			
			DartCtrlHandle.Reset_AutoHome = 0;
			DartCtrlHandle.Motor_Online = 0;
			hCtrl.Reset_Flag = 1;
		}
	}
	else
	{
		switch(hCtrl.CtrlWay)
		{
			case CtrlWay_Safe:  /* 安全模式 */
			{
				DartCtrlHandle.Reset_AutoHome = 0;
				DartCtrlHandle.Motor_Online = 0;
				
				Move_Trigger_SetCtrlMode(CtrlMode_safe);
				Load_SetCtrlMode(CtrlMode_safe);
				Lift_SetCtrlMode(CtrlMode_safe);
				Pull_Spring_Left_SetCtrlMode(CtrlMode_safe);
				Pull_Spring_Right_SetCtrlMode(CtrlMode_safe);
				Yaw_SetCtrlMode(CtrlMode_safe);
				Fric_Left_SetCtrlMode(CtrlMode_safe);
				Fric_Right_SetCtrlMode(CtrlMode_safe);
				Small_Firc_SetCtrlMode(CtrlMode_safe);
				
				DartCtrlHandle.Move_Trigger.PidOut = 0;
				DartCtrlHandle.Pull_Spring_Left.PidOut = 0;
				DartCtrlHandle.Pull_Spring_Right.PidOut = 0;
				DartCtrlHandle.Small_Fric.PidOut = 0;
				DartCtrlHandle.Fric_Right.PidOut = 0;
				DartCtrlHandle.Fric_Left.PidOut = 0;
				DartCtrlHandle.Lift.PidOut = 0;
				DartCtrlHandle.Load.PidOut = 0;
				DartCtrlHandle.Yaw.PidOut = 0;
				
				Safe_Mode_Control();
				
				break;
			}
			case CtrlWay_RC:    /* 遥控器模式 */   
			{
				
				RemoteControl_Dart();
				break;
			}
			case CtrlWay_Copt:	/* 比赛模式*/
			{
				Auto_Load_Control();
				break;
			}
			
			
		}
	}
	
	
	/*---------控制部分---------*/
	//根据模式的设置，给出PID输出    
	switch(DartCtrlHandle.Move_Trigger.CtrlMode[0])  				 //根据扳机的控制模式，选择不同的PID输出
    {
        case CtrlMode_position:
        {
            DartCtrlHandle.Move_Trigger.PidOut = Move_TriggerCtrl_PositionCtrl();
            break;
        }
        case CtrlMode_speed:
        {
            DartCtrlHandle.Move_Trigger.PidOut = Move_TriggerCtrl_SpeedCtrl();
            break;
        }
        case CtrlMode_safe:
        {
            DartCtrlHandle.Move_Trigger.PidOut = 0;
            break;
        }
        default:
        {
            DartCtrlHandle.Move_Trigger.PidOut = 0;
            break;
        }
    }
	
	switch(DartCtrlHandle.Fric_Left.CtrlMode[0]) 					 //根据左摩擦轮的控制模式，选择不同的PID输出
    {
        case CtrlMode_speed:
        {
            DartCtrlHandle.Fric_Left.PidOut = Fric_Left_SpeedCtrl();
            break;
        }
        case CtrlMode_safe:
        {
            DartCtrlHandle.Fric_Left.PidOut = 0;
            break;
        }
        default:
        {
            DartCtrlHandle.Fric_Left.PidOut = 0;
            break;
        }
    }
	
	switch(DartCtrlHandle.Fric_Right.CtrlMode[0]) 					 //根据右摩擦轮的控制模式，选择不同的PID输出
    { 
        case CtrlMode_speed:
        {
            DartCtrlHandle.Fric_Right.PidOut = Fric_Right_SpeedCtrl();
            break;
        }
        case CtrlMode_safe:
        {
            DartCtrlHandle.Fric_Right.PidOut = 0;
            break;
        }
        default:
        {
            DartCtrlHandle.Fric_Right.PidOut = 0;
            break;
        }
    }
	
	switch(DartCtrlHandle.Lift.CtrlMode[0]) 					 //根据抬升电机的控制模式，选择不同的PID输出
    {
        case CtrlMode_position:
        {
            DartCtrlHandle.Lift.PidOut = Lift_PositionCtrl();
            break;
        }
        case CtrlMode_speed:
        {
            DartCtrlHandle.Lift.PidOut = Lift_SpeedCtrl();
            break;
        }
        case CtrlMode_safe:
        {
            DartCtrlHandle.Lift.PidOut = 0;
            break;
        }
        default:
        {
            DartCtrlHandle.Lift.PidOut = 0;
            break;
        }
    }
	
	switch(DartCtrlHandle.Load.CtrlMode[0]) 				 	 //根据装填电机的控制模式，选择不同的PID输出
    {
        case CtrlMode_position:
        {
            DartCtrlHandle.Load.PidOut = Load_PositionCtrl();
            break;
        }
        case CtrlMode_speed:
        {
            DartCtrlHandle.Load.PidOut = Load_SpeedCtrl();
            break;
        }
        case CtrlMode_safe:
        {
            DartCtrlHandle.Load.PidOut = 0;
            break;
        }
        default:
        {
            DartCtrlHandle.Load.PidOut = 0;
            break;
        }
    }
	
	switch(DartCtrlHandle.Pull_Spring_Left.CtrlMode[0]) 		 //根据左拉簧电机的控制模式，选择不同的PID输出
    {
        case CtrlMode_position:
        {
            DartCtrlHandle.Pull_Spring_Left.PidOut = Pull_Spring_Left_PositionCtrl();
            break;
        }
        case CtrlMode_speed:
        {
            DartCtrlHandle.Pull_Spring_Left.PidOut = Pull_Spring_Left_SpeedCtrl();
            break;
        }
        case CtrlMode_safe:
        {
            DartCtrlHandle.Pull_Spring_Left.PidOut = 0;
            break;
        }
        default:
        {
            DartCtrlHandle.Pull_Spring_Left.PidOut = 0;
            break;
        }
    }
	
	switch(DartCtrlHandle.Pull_Spring_Right.CtrlMode[0])		 //根据右拉簧电机的控制模式，选择不同的PID输出
    {
        case CtrlMode_position:
        {
            DartCtrlHandle.Pull_Spring_Right.PidOut = Pull_Spring_Right_PositionCtrl();
            break;
        }
        case CtrlMode_speed:
        {
            DartCtrlHandle.Pull_Spring_Right.PidOut = Pull_Spring_Right_SpeedCtrl();
            break;
        }
        case CtrlMode_safe:
        {
            DartCtrlHandle.Pull_Spring_Right.PidOut = 0;
            break;
        }
        default:
        {
            DartCtrlHandle.Pull_Spring_Right.PidOut = 0;
            break;
        }
    }
	
	switch(DartCtrlHandle.Small_Fric.CtrlMode[0])					 //根据小摩擦轮的控制模式，选择不同的PID输出
    {
        case CtrlMode_speed:
        {
            DartCtrlHandle.Small_Fric.PidOut = Small_Fric_SpeedCtrl();
            break;
        }
        case CtrlMode_safe:
        {
            DartCtrlHandle.Small_Fric.PidOut = 0;
            break;
        }
        default:
        {
            DartCtrlHandle.Small_Fric.PidOut = 0;
            break;
        }
    }
	
	switch(DartCtrlHandle.Yaw.CtrlMode[0])							 //根据Yaw轴电机的控制模式，选择不同的PID输出
    {
        case CtrlMode_position:
        {
            DartCtrlHandle.Yaw.PidOut = Yaw_PositionCtrl();
            break;
        }
        case CtrlMode_speed:
        {
            DartCtrlHandle.Yaw.PidOut = Yaw_SpeedCtrl();
            break;
        }
        case CtrlMode_safe:
        {
            DartCtrlHandle.Yaw.PidOut = 0;
            break;
        }
        default:
        {
            DartCtrlHandle.Yaw.PidOut = 0;
            break;
        }
    }
}
/**
  * @name   Yaw_AutoHome
  * @brief  Yaw轴电机的撞限位
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
void Yaw_AutoHome(void)
{
	 static uint8_t LockCount;
    int16_t flag;
    
    DartCtrlHandle.Yaw.Online_State = Device_GetState(DeviceID_Yaw);
//    DartCtrlHandle.ConflictImage = 0;

	   //舵机初始化
    if (DartCtrlHandle.Yaw.AutoHome != AutoHomeStatus_Ready)
    {
        if (DartCtrlHandle.Yaw.AutoHome == AutoHomeStatus_Default)
        {
            PID_cleardata(&motor_pid.Yaw_Speed);

            // 设置校准过程中要使用的PID参数
            motor_pid.Yaw_Speed.mode     = PID_MODE_POSITION;
            motor_pid.Yaw_Speed.kp       = 70;
            motor_pid.Yaw_Speed.ki       = 0.4;
            motor_pid.Yaw_Speed.kd       = 0.5;
            motor_pid.Yaw_Speed.iOutMax  = 8900;// 2000
            motor_pid.Yaw_Speed.outMax   = 12000;

            // 设置找零过程中的速度
            DartCtrlHandle.Yaw.SpdTarget = -400;   //向左撞限位
            // 设为速度控制模式
            Yaw_SetCtrlMode(CtrlMode_speed);
            
            LockCount = 0;
            DartCtrlHandle.Yaw.AutoHome = AutoHomeStatus_Phase_1;
        }
        
        if (DartCtrlHandle.Yaw.AutoHome == AutoHomeStatus_Phase_1)
        {
            if(ABS(motor_data.Yaw.speed) == 0)//motor_pid.Yaw_Speed.outMax)
            {
                LockCount++;
            }
            if(LockCount > 50) 
            {
                // 将当前位置设为图传零点并得出图传的不同位置需要
                motor_data.Yaw.turns = 0;
                motor_data.Yaw.sumPosition = motor_data.Yaw.position;
                DartCtrlHandle.Yaw.PositionZero = motor_data.Yaw.position + 2000;  //待测
				DartCtrlHandle.Yaw.PositionEnd = DartCtrlHandle.Yaw.PositionZero + 360000;  //待测
                //给电机卸力
                PID_cleardata(&motor_pid.Yaw_Speed);
                DartCtrlHandle.Yaw.SpdTarget = 0;
                //转为位置控制
                Yaw_SetCtrlMode(CtrlMode_safe);
                
                DartCtrlHandle.Yaw.AutoHome = AutoHomeStatus_Ready;
				//对扳机使用真正的参数
				PID_init(&motor_pid.Yaw_Speed, &PID_Yaw_Speed_init);
				PID_init(&motor_pid.Yaw_Position, &PID_Yaw_Position_init);
            }
        }
        
        switch(DartCtrlHandle.Yaw.CtrlMode[0])
        {
            case CtrlMode_speed:
            {
                DartCtrlHandle.Yaw.PidOut = Yaw_SpeedCtrl();
                break;
            }
            case CtrlMode_position:
            {
                DartCtrlHandle.Yaw.PidOut = Yaw_PositionCtrl();
                break;
            }
            case CtrlMode_safe:
            {
                DartCtrlHandle.Yaw.PidOut = 0;
                break;
            }
            default:
            {
                DartCtrlHandle.Yaw.PidOut = 0;
                break;
            }
        }
    }

}
/**
  * @name   Trigger_AutoHome
  * @brief  扳机的校准
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
void Trigger_AutoHome(void)
{
    static uint8_t LockCount;
    int16_t flag;
    
    DartCtrlHandle.Move_Trigger.Online_State = Device_GetState(DeviceID_Move_Trigger);

	   //舵机初始化
    if (DartCtrlHandle.Move_Trigger.AutoHome != AutoHomeStatus_Ready)
    {
        if (DartCtrlHandle.Move_Trigger.AutoHome == AutoHomeStatus_Default)
        {
            PID_cleardata(&motor_pid.Move_Trigger_Speed);

            // 设置校准过程中要使用的PID参数
            motor_pid.Move_Trigger_Speed.mode     = PID_MODE_POSITION;
            motor_pid.Move_Trigger_Speed.kp       = 10;
            motor_pid.Move_Trigger_Speed.ki       = 0.01;
            motor_pid.Move_Trigger_Speed.kd       = 5;
            motor_pid.Move_Trigger_Speed.iOutMax  = 1000;// 2000
            motor_pid.Move_Trigger_Speed.outMax   = 4000;

            // 设置找零过程中的速度
            DartCtrlHandle.Move_Trigger.SpdTarget = -4200;
            // 设为速度控制模式
            Move_Trigger_SetCtrlMode(CtrlMode_speed);
            
            LockCount = 0;
            DartCtrlHandle.Move_Trigger.AutoHome = AutoHomeStatus_Phase_1;
        }
        
        if (DartCtrlHandle.Move_Trigger.AutoHome == AutoHomeStatus_Phase_1)
        {
            if(ABS(motor_data.Move_Trigger.speed) == 0)
            {
                LockCount++;
            }
            if(LockCount > 50) 
            {
                // 将当前位置设为图传零点并得出图传的不同位置需要
                motor_data.Move_Trigger.turns = 0;
                motor_data.Move_Trigger.sumPosition = motor_data.Move_Trigger.position;
                DartCtrlHandle.Move_Trigger.PositionZero = motor_data.Move_Trigger.position + 29000;
				DartCtrlHandle.Move_Trigger.PositionEnd = DartCtrlHandle.Move_Trigger.PositionZero + 13719026;
                //给电机卸力
                PID_cleardata(&motor_pid.Move_Trigger_Speed);
                DartCtrlHandle.Move_Trigger.SpdTarget = 0;
                //转为位置控制
                Move_Trigger_SetCtrlMode(CtrlMode_safe);
                
                DartCtrlHandle.Move_Trigger.AutoHome = AutoHomeStatus_Ready;
				//对扳机使用真正的参数
				PID_init(&motor_pid.Move_Trigger_Speed, &PID_Move_Trigger_Speed_init);
				PID_init(&motor_pid.Move_Trigger_Position, &PID_Move_Trigger_Position_init);
            }
        }
        
        switch(DartCtrlHandle.Move_Trigger.CtrlMode[0])
        {
            case CtrlMode_speed:
            {
                DartCtrlHandle.Move_Trigger.PidOut = Move_TriggerCtrl_SpeedCtrl();
                break;
            }
            case CtrlMode_position:
            {
                DartCtrlHandle.Move_Trigger.PidOut = Move_TriggerCtrl_PositionCtrl();
                break;
            }
            case CtrlMode_safe:
            {
                DartCtrlHandle.Move_Trigger.PidOut = 0;
                break;
            }
            default:
            {
                DartCtrlHandle.Move_Trigger.PidOut = 0;
                break;
            }
        }
    }

}

/**
  * @name   Load_AutoHome
  * @brief  装填电机的校准
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
void Load_AutoHome(void)
{
    static uint8_t LockCount;
    int16_t flag;
    
    DartCtrlHandle.Load.Online_State = Device_GetState(DeviceID_Load);
    
    if(DartCtrlHandle.Load.AutoHome != AutoHomeStatus_Ready)
    {
        if(DartCtrlHandle.Load.AutoHome == AutoHomeStatus_Default)
        {
            PID_cleardata(&motor_pid.Load_Speed);

            // 设置校准过程中要使用的PID参数
            motor_pid.Load_Speed.mode     = PID_MODE_POSITION;
            motor_pid.Load_Speed.kp       = 10;
            motor_pid.Load_Speed.ki       = 0.01;
            motor_pid.Load_Speed.kd       = 5;
            motor_pid.Load_Speed.iOutMax  = 1000;
            motor_pid.Load_Speed.outMax   = 5000;// 7000

            // 设置找零过程中的速度
            DartCtrlHandle.Load.SpdTarget = 1000;
            // 设为速度控制模式
            Load_SetCtrlMode(CtrlMode_speed);
            
            LockCount = 0;
            DartCtrlHandle.Load.AutoHome = AutoHomeStatus_Phase_1;
        }
        
        if(DartCtrlHandle.Load.AutoHome == AutoHomeStatus_Phase_1)
        {
            if(ABS(motor_pid.Load_Speed.out) >= motor_pid.Load_Speed.outMax)  //PID不合适可能导致电机转速过低，且就算撞到限位，也判断没有
            {
                LockCount++;
            }

            if(LockCount > 50)
            {
                // 将当前位置设为图传零点并得出图传的不同位置需要
                motor_data.Load.turns = 0;
                motor_data.Load.sumPosition = motor_data.Load.position;
                DartCtrlHandle.Load.PositionZero = motor_data.Load.position - 2500;
				DartCtrlHandle.Load_Position_Mid = DartCtrlHandle.Load.PositionZero - 840321;
				DartCtrlHandle.Load_Position_Mid_New = DartCtrlHandle.Load.PositionZero - 1601934;
				DartCtrlHandle.Load.PositionEnd = DartCtrlHandle.Load.PositionZero - 2700574;
				
                //给电机卸力
                PID_cleardata(&motor_pid.Load_Speed);
				
                DartCtrlHandle.Load.SpdTarget = 0;
				
				Load_SetCtrlMode(CtrlMode_safe);
				
                //转为位置控制

				DartCtrlHandle.Load.AutoHome = AutoHomeStatus_Ready;
                                
				//对扳机使用真正的参数
				PID_init(&motor_pid.Load_Speed, &PID_Load_Speed_init);
				PID_init(&motor_pid.Load_Position, &PID_Load_Position_init);
            }
        }
        
        switch(DartCtrlHandle.Load.CtrlMode[0])
        {
            case CtrlMode_speed:
            {
                DartCtrlHandle.Load.PidOut = Load_SpeedCtrl();
                break;
            }
            case CtrlMode_position:
            {
                DartCtrlHandle.Load.PidOut = Load_PositionCtrl();
                break;
            }
            case CtrlMode_safe:
            {
                DartCtrlHandle.Load.PidOut = 0;
                break;
            }
            default:
            {
                DartCtrlHandle.Load.PidOut = 0;
                break;
            }
        }
    }

}

/**
  * @name   Lift_AutoHome
  * @brief  抬升电机的校准
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
void Lift_AutoHome(void)   
{
    static uint8_t LockCount;
	static uint16_t ShootNum;
	static uint8_t Progress = 0;
    int16_t flag;
    
    DartCtrlHandle.Lift.Online_State = Device_GetState(DeviceID_Lift);
	DartCtrlHandle.Trigger_Detect = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);  //读飞镖上膛的扳机上的微动开关
    

    if (DartCtrlHandle.Lift.AutoHome != AutoHomeStatus_Ready)
    {
        if(DartCtrlHandle.Lift.AutoHome == AutoHomeStatus_Default)
        {
            PID_cleardata(&motor_pid.Lift_Speed);

            // 设置校准过程中要使用的PID参数
            motor_pid.Lift_Speed.mode     = PID_MODE_POSITION;
            motor_pid.Lift_Speed.kp       = 8;
            motor_pid.Lift_Speed.ki       = 0.1;
            motor_pid.Lift_Speed.kd       = 4;
            motor_pid.Lift_Speed.iOutMax  = 3000 ;
            motor_pid.Lift_Speed.outMax   = 9600;// 7000

            // 设置找零过程中的速度
            DartCtrlHandle.Lift.SpdTarget = 1200;
            // 设为速度控制模式
            Lift_SetCtrlMode(CtrlMode_speed);
            
            LockCount = 0;
            DartCtrlHandle.Lift.AutoHome = AutoHomeStatus_Phase_1;
        }
        
        if(DartCtrlHandle.Lift.AutoHome == AutoHomeStatus_Phase_1)
        {
            if(ABS(motor_pid.Lift_Speed.out) >= motor_pid.Lift_Speed.outMax)
            {
                LockCount++;
            }

            if(LockCount > 50 && Progress == 0)
            {
              
                // 将当前位置设为图传零点并得出图传的不同位置需要
                motor_data.Lift.turns = 0;
                motor_data.Lift.sumPosition = motor_data.Lift.position;
                DartCtrlHandle.Lift.PositionZero = motor_data.Lift.position - 9900;
                DartCtrlHandle.Lift.PositionEnd = DartCtrlHandle.Lift.PositionZero - 330000;
				
                //给电机卸力
                PID_cleardata(&motor_pid.Lift_Speed);
                DartCtrlHandle.Lift.SpdTarget = 0;
				
				Progress = 1;

            }
			
			if (Progress == 1)
			{
				PID_init(&motor_pid.Lift_Speed, &PID_Lift_Speed_init);
				PID_init(&motor_pid.Lift_Position, &PID_Lift_Position_init);
				
				Lift_SetCtrlMode(CtrlMode_position);
				DartCtrlHandle.Lift.EncTarget = DartCtrlHandle.Lift.PositionZero;
				
				if (ABS(motor_data.Lift.sumPosition - DartCtrlHandle.Lift.EncTarget) < 2000 && DartCtrlHandle.Pull_Spring_Left_IO == 0 && DartCtrlHandle.Pull_Spring_Right_IO == 0)
				{
					PWM_SetDuty(PWM_B4, Release_PWM);
					ShootNum++;
					
					if (ShootNum > 500)
					{
						DartCtrlHandle.Lift.AutoHome = AutoHomeStatus_Ready;
						Lift_SetCtrlMode(CtrlMode_safe);
						
						DartCtrlHandle.Lift.PidOut = 0;
						 //对扳机使用真正的参数
						ShootNum = 0;
					}
				}
			}
        }
        
        switch(DartCtrlHandle.Lift.CtrlMode[0])
        {
            case CtrlMode_speed:
            {
                DartCtrlHandle.Lift.PidOut = Lift_SpeedCtrl();
                break;
            }
            case CtrlMode_position:
            {
                DartCtrlHandle.Lift.PidOut = Lift_PositionCtrl();
                break;
            }
            case CtrlMode_safe:
            {
                DartCtrlHandle.Lift.PidOut = 0;
                break;
            }
            default:
            {
                DartCtrlHandle.Lift.PidOut = 0;
                break;
            }
        }
    }

}

/**
  * @name   Pull_Spring_Left_AutoHome
  * @brief  拉簧左电机的校准
  * @call   Internal
  * @param  NUL
  * @RetVal NULL
  */
void Pull_Spring_Left_AutoHome(void)   
{
    static uint8_t LockCount;
	static uint8_t Record_IO_State;
    int16_t flag;

    DartCtrlHandle.Pull_Spring_Left.Online_State = Device_GetState(DeviceID_Pull_Spring_Left);  
	Record_IO_State	= DartCtrlHandle.Pull_Spring_Left_IO;
	DartCtrlHandle.Pull_Spring_Left_IO =  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);      //3508链条的开关
	
	
    if (DartCtrlHandle.Pull_Spring_Left.AutoHome != AutoHomeStatus_Ready)
    {
        if(DartCtrlHandle.Pull_Spring_Left.AutoHome == AutoHomeStatus_Default)
        {
            PID_cleardata(&motor_pid.Pull_Spring_Left_Speed);

            // 设置校准过程中要使用的PID参数
            motor_pid.Pull_Spring_Left_Speed.mode     = PID_MODE_POSITION;
            motor_pid.Pull_Spring_Left_Speed.kp       = 10;
            motor_pid.Pull_Spring_Left_Speed.ki       = 0.05;
            motor_pid.Pull_Spring_Left_Speed.kd       = 0;
            motor_pid.Pull_Spring_Left_Speed.iOutMax  = 2000 ;
            motor_pid.Pull_Spring_Left_Speed.outMax   = 4000;// 7000

            // 设置找零过程中的速度
            DartCtrlHandle.Pull_Spring_Left.SpdTarget = -500;
            // 设为速度控制模式
            Pull_Spring_Left_SetCtrlMode(CtrlMode_speed);
            
            LockCount = 0;
            DartCtrlHandle.Pull_Spring_Left.AutoHome = AutoHomeStatus_Phase_1;
        }
        
        if(DartCtrlHandle.Pull_Spring_Left.AutoHome == AutoHomeStatus_Phase_1)
        {
			if ((DartCtrlHandle.Pull_Spring_Left_IO == 0 && Record_IO_State == 1) || (DartCtrlHandle.Pull_Spring_Left_IO == 0 && Record_IO_State == 0))
			{
				// 将当前位置设为图传零点并得出图传的不同位置需要
				motor_data.Pull_Spring_Left.turns = 0;
				motor_data.Pull_Spring_Left.sumPosition = motor_data.Pull_Spring_Left.position;
				DartCtrlHandle.Pull_Spring_Left.PositionZero = motor_data.Pull_Spring_Left.position;
				DartCtrlHandle.Pull_Spring_Left.PositionEnd = DartCtrlHandle.Pull_Spring_Left.PositionZero + 2000000;
				

				//给电机卸力
				PID_cleardata(&motor_pid.Pull_Spring_Left_Speed);
				DartCtrlHandle.Pull_Spring_Left.SpdTarget = 0;

				//转为位置控制
				Pull_Spring_Left_SetCtrlMode(CtrlMode_safe);
						
				DartCtrlHandle.Pull_Spring_Left.AutoHome = AutoHomeStatus_Ready;

				//对扳机使用真正的参数
				PID_init(&motor_pid.Pull_Spring_Left_Speed, &PID_Pull_Spring_Left_Speed_init);
				PID_init(&motor_pid.Pull_Spring_Left_Position, &PID_Pull_Spring_Left_Position_init);
			}
        }
        
        switch(DartCtrlHandle.Pull_Spring_Left.CtrlMode[0])
        {
            case CtrlMode_speed:
            {
                DartCtrlHandle.Pull_Spring_Left.PidOut = Pull_Spring_Left_SpeedCtrl();
                break;
            }
            case CtrlMode_position:
            {
                DartCtrlHandle.Pull_Spring_Left.PidOut = Pull_Spring_Left_PositionCtrl();
                break;
            }
            case CtrlMode_safe:
            {
                DartCtrlHandle.Pull_Spring_Left.PidOut = 0;
                break;
            }
            default:
            {
                DartCtrlHandle.Pull_Spring_Left.PidOut = 0;
                break;
            }
        }
    }

}

/**
  * @name   Pull_Spring_Right_AutoHome
  * @brief  拉簧右电机的校准
  * @call   Internal
  * @param  NUL
  * @RetVal NULL
  */
void Pull_Spring_Right_AutoHome(void)  
{
    static uint8_t LockCount;
	static uint8_t Record_IO_State;
    int16_t flag;

    DartCtrlHandle.Pull_Spring_Right.Online_State = Device_GetState(DeviceID_Pull_Spring_Right);
	Record_IO_State	= DartCtrlHandle.Pull_Spring_Right_IO;
	DartCtrlHandle.Pull_Spring_Right_IO =  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);      //3508链条的开关

    if (DartCtrlHandle.Pull_Spring_Right.AutoHome != AutoHomeStatus_Ready)
    {
        if(DartCtrlHandle.Pull_Spring_Right.AutoHome == AutoHomeStatus_Default)
        {
            PID_cleardata(&motor_pid.Pull_Spring_Right_Speed);

            // 设置校准过程中要使用的PID参数
            motor_pid.Pull_Spring_Right_Speed.mode     = PID_MODE_POSITION;
            motor_pid.Pull_Spring_Right_Speed.kp       = 10;
            motor_pid.Pull_Spring_Right_Speed.ki       = 0.0500;
            motor_pid.Pull_Spring_Right_Speed.kd       = 0;
            motor_pid.Pull_Spring_Right_Speed.iOutMax  = 2000 ;
            motor_pid.Pull_Spring_Right_Speed.outMax   = 6000;// 7000

            // 设置找零过程中的速度
            DartCtrlHandle.Pull_Spring_Right.SpdTarget = 500;
            // 设为速度控制模式
            Pull_Spring_Right_SetCtrlMode(CtrlMode_speed);
            
            LockCount = 0;
            DartCtrlHandle.Pull_Spring_Right.AutoHome = AutoHomeStatus_Phase_1;
        }
        
        if(DartCtrlHandle.Pull_Spring_Right.AutoHome == AutoHomeStatus_Phase_1)
        {
			if ((DartCtrlHandle.Pull_Spring_Right_IO == 0 && Record_IO_State == 1) || (DartCtrlHandle.Pull_Spring_Right_IO == 0 && Record_IO_State == 0))
			{
                // 将当前位置设为图传零点并得出图传的不同位置需要
                motor_data.Pull_Spring_Right.turns = 0;
                motor_data.Pull_Spring_Right.sumPosition = motor_data.Pull_Spring_Right.position;
                DartCtrlHandle.Pull_Spring_Right.PositionZero = motor_data.Pull_Spring_Right.position;
				DartCtrlHandle.Pull_Spring_Right.PositionEnd = DartCtrlHandle.Pull_Spring_Right.PositionZero - 2000000;
				
                //给电机卸力
                PID_cleardata(&motor_pid.Pull_Spring_Right_Speed);
                DartCtrlHandle.Pull_Spring_Right.SpdTarget = 0;
                //转为位置控制
                Pull_Spring_Right_SetCtrlMode(CtrlMode_safe);
                
                DartCtrlHandle.Pull_Spring_Right.AutoHome = AutoHomeStatus_Ready;
				 //对扳机使用真正的参数
				PID_init(&motor_pid.Pull_Spring_Right_Speed, &PID_Pull_Spring_Right_Speed_init);
				PID_init(&motor_pid.Pull_Spring_Right_Position, &PID_Pull_Spring_Right_Position_init);
            }
        }
        
        switch(DartCtrlHandle.Pull_Spring_Right.CtrlMode[0])
        {
            case CtrlMode_speed:
            {
                DartCtrlHandle.Pull_Spring_Right.PidOut = Pull_Spring_Right_SpeedCtrl();
                break;
            }
            case CtrlMode_position:
            {
                DartCtrlHandle.Pull_Spring_Right.PidOut = Pull_Spring_Right_PositionCtrl();
                break;
            }
            case CtrlMode_safe:
            {
                DartCtrlHandle.Pull_Spring_Right.PidOut = 0;
                break;
            }
            default:
            {
                DartCtrlHandle.Pull_Spring_Right.PidOut = 0;
                break;
            }
        }
    }

}

/* ============ 函数定义 ================= */
/**
  * @name   遥控器控制飞镖

  * @brief   电机根据遥控的信号值开始移动，考虑到因为是遥控模式，
            各个模块的位置，主要由人来控制，所以只采取速度控制
  * @call   
  * @param  
  * @RetVal 
*/
//上 右是正  拨杆
//福斯：0：右摇杆左右  1：右摇杆上下  2：左摇杆上下  3：左摇杆左右 
//拨杆：上0 ，下1567 4；左一拨杆  5；左二拨杆  6：右一拨杆 7：右二拨杆  
//8：左旋钮 0-1567  9右旋扭 0-1567  
//左一上 左二上为遥控模式
int16_t Record_Right_Num_1 = 0;
int16_t Record_Right_Num_2 = 0;
void RemoteControl_Dart(void)   		//rc_ctrl.rc.ch[7]
{	
	static uint8_t Load_Num = 0;
	static uint8_t Pull_Spring_Small_Flag = 0;

	Record_Right_Num_2 = Record_Right_Num_1;
	DartCtrlHandle.Load_Switch = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5);

	if (DartCtrlHandle.Race_Mode == 3)
	{
		if (get_err_0_flag == 1 )
		{
			DartCtrlHandle.Yaw.SpdTarget = 0;   //正偏差镖架向右，即速度期望是正数时，镖架向右运动
		}
	}
	// 右中上[拉簧]
	if (rc_ctrl.rc.ch[6] < 50)  //右一拨杆在上
	{
		//左摇杆微动
		if (ABS(rc_ctrl.rc.ch[2]) > 10 && DartCtrlHandle.Pull_Spring_Left.EncTarget >= DartCtrlHandle.Pull_Spring_Left.PositionZero && DartCtrlHandle.Pull_Spring_Left.EncTarget <= DartCtrlHandle.Pull_Spring_Left.PositionEnd)
		{
			DartCtrlHandle.Pull_Spring_Left.EncTarget = motor_data.Pull_Spring_Left.sumPosition - rc_ctrl.rc.ch[2] * 1;
			Pull_Spring_Left_SetCtrlMode(CtrlMode_position);
			Pull_Spring_Small_Flag = 1;
		}
		// 拉簧电机  右摇杆大动
		else if (ABS(rc_ctrl.rc.ch[1]) > 10 && Pull_Spring_Small_Flag == 0
			&& DartCtrlHandle.Pull_Spring_Left.EncTarget >= DartCtrlHandle.Pull_Spring_Left.PositionZero && DartCtrlHandle.Pull_Spring_Left.EncTarget <= DartCtrlHandle.Pull_Spring_Left.PositionEnd)
		{
			DartCtrlHandle.Pull_Spring_Enc_Target_Up += rc_ctrl.rc.ch[1] * 2.5;
			DartCtrlHandle.Pull_Spring_Enc_Target_Up = Data_Limit(DartCtrlHandle.Pull_Spring_Enc_Target_Up, DartCtrlHandle.Pull_Spring_Right.PositionEnd, 0);
							
			DartCtrlHandle.Pull_Spring_Left.EncTarget = DartCtrlHandle.Pull_Spring_Left.PositionZero - DartCtrlHandle.Pull_Spring_Enc_Target_Up;
			Pull_Spring_Left_SetCtrlMode(CtrlMode_position);
			DartCtrlHandle.Pull_Spring_Right.EncTarget = DartCtrlHandle.Pull_Spring_Right.PositionZero + DartCtrlHandle.Pull_Spring_Enc_Target_Up;// - Pull_Spring_Dif;
			Pull_Spring_Right_SetCtrlMode(CtrlMode_position);
		}
		else if ((ABS(rc_ctrl.rc.ch[1]) > 10 && Pull_Spring_Small_Flag == 1)
			&& DartCtrlHandle.Pull_Spring_Left.EncTarget >= DartCtrlHandle.Pull_Spring_Left.PositionZero && DartCtrlHandle.Pull_Spring_Left.EncTarget <= DartCtrlHandle.Pull_Spring_Left.PositionEnd)
		{
			
			DartCtrlHandle.Pull_Spring_Left.EncTarget = motor_data.Pull_Spring_Left.sumPosition - rc_ctrl.rc.ch[1] * 40;
			Pull_Spring_Left_SetCtrlMode(CtrlMode_position);
			DartCtrlHandle.Pull_Spring_Right.EncTarget = motor_data.Pull_Spring_Right.sumPosition + rc_ctrl.rc.ch[1] * 40;
			Pull_Spring_Right_SetCtrlMode(CtrlMode_position);
		}
		
		
		DartCtrlHandle.Pull_Spring_Left.EncTarget = Data_Limit(DartCtrlHandle.Pull_Spring_Left.EncTarget, DartCtrlHandle.Pull_Spring_Left.PositionZero, DartCtrlHandle.Pull_Spring_Left.PositionEnd);
		DartCtrlHandle.Pull_Spring_Right.EncTarget = Data_Limit(DartCtrlHandle.Pull_Spring_Right.EncTarget, DartCtrlHandle.Pull_Spring_Right.PositionEnd, DartCtrlHandle.Pull_Spring_Right.PositionZero);		
		
	}
	else if (rc_ctrl.rc.ch[6] > 500 && rc_ctrl.rc.ch[6] < 1000)		//右拨杆在中间		// 装填
	{
		// 抬升
		if (rc_ctrl.rc.ch[2] < -500)    
		{
			DartCtrlHandle.Lift.EncTarget = DartCtrlHandle.Lift.PositionEnd;
			Lift_SetCtrlMode(CtrlMode_position);	
		}
		else if (rc_ctrl.rc.ch[2] > 500)
		{
			DartCtrlHandle.Lift.EncTarget = DartCtrlHandle.Lift.PositionZero;
			Lift_SetCtrlMode(CtrlMode_position);	
		}
		
		// 装填   右摇杆上下
		if (rc_ctrl.rc.ch[1] > 500 && DartCtrlHandle.Record_Right_Num <= 500)
		{
			Record_Right_Num_1++;
		}
		else if (rc_ctrl.rc.ch[1] < -500 && DartCtrlHandle.Record_Right_Num >= -500)
		{
			Record_Right_Num_1--;
		}
		
		
		if (Record_Right_Num_1 - Record_Right_Num_2 > 0)   //向上推了一下
		{
			if (ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load.PositionZero) < 10000)
			{
				DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load_Position_Mid;
				DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
				Load_SetCtrlMode(CtrlMode_position);
			}
			else if (ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load_Position_Mid) < 10000)
			{
				DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load_Position_Mid_New;
				DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
				Load_SetCtrlMode(CtrlMode_position);
			}
			else if (ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load_Position_Mid_New) < 10000)
			{
				DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load.PositionEnd;
				DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
				Load_SetCtrlMode(CtrlMode_position);
			}
			else if (ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load.PositionEnd) < 10000)
			{
				DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load.PositionEnd;
				DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
				Load_SetCtrlMode(CtrlMode_position);
			}
		}
		else if (Record_Right_Num_1 - Record_Right_Num_2 < 0)  //向下退了一下
		{
			if (ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load.PositionZero) < 10000)
			{
				DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load.PositionZero;
				DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
				Load_SetCtrlMode(CtrlMode_position);
			}
			else if ( ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load_Position_Mid) < 10000)
			{
				DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load.PositionZero;
				DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
				Load_SetCtrlMode(CtrlMode_position);
			}
			else if ( ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load_Position_Mid_New) < 10000)
			{
				DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load_Position_Mid;
				DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
				Load_SetCtrlMode(CtrlMode_position);
			}
			else if (ABS(motor_data.Load.sumPosition - DartCtrlHandle.Load.PositionEnd) < 10000)
			{
				DartCtrlHandle.Load.EncTarget = DartCtrlHandle.Load_Position_Mid_New;
				DartCtrlHandle.Load.EncTarget = Data_Limit(DartCtrlHandle.Load.EncTarget, DartCtrlHandle.Load.PositionEnd, DartCtrlHandle.Load.PositionZero);
				Load_SetCtrlMode(CtrlMode_position);
			}
		}	
	}
	else if (rc_ctrl.rc.ch[6] > 1000)	// 右一拨杆在下		// 微调
	{
		// Yaw   右遥感左右微调
		
		if (ABS(rc_ctrl.rc.ch[0] )< 10)    
		{
			DartCtrlHandle.Yaw.SpdTarget = 0;
			Yaw_SetCtrlMode(CtrlMode_speed);
			PID_cleardata(&motor_pid.Yaw_Speed);
//			clearerr
		}
		else
		{
			DartCtrlHandle.Yaw.SpdTarget = /*motor_data.Yaw.sumPosition + */0.1 * rc_ctrl.rc.ch[0] ;
			Yaw_SetCtrlMode(CtrlMode_speed);
		}
		
		if (ABS(rc_ctrl.rc.ch[2]) < 10)
		{
			DartCtrlHandle.Move_Trigger.EncTarget = motor_data.Move_Trigger.sumPosition;
			DartCtrlHandle.Move_Trigger.EncTarget = Data_Limit(DartCtrlHandle.Move_Trigger.EncTarget, DartCtrlHandle.Move_Trigger.PositionZero, DartCtrlHandle.Move_Trigger.PositionEnd);
			Move_Trigger_SetCtrlMode(CtrlMode_position);		
		}
		else
		{
			DartCtrlHandle.Move_Trigger.EncTarget = motor_data.Move_Trigger.sumPosition - 20 * rc_ctrl.rc.ch[2];
			DartCtrlHandle.Move_Trigger.EncTarget = Data_Limit(DartCtrlHandle.Move_Trigger.EncTarget, DartCtrlHandle.Move_Trigger.PositionZero, DartCtrlHandle.Move_Trigger.PositionEnd);
			Move_Trigger_SetCtrlMode(CtrlMode_position);	
		}
		
		DartCtrlHandle.screw_2006_num = motor_data.Move_Trigger.turns / 36.0;
	}
	
// 摩擦轮控制
//福斯：0：右摇杆左右  1：右摇杆上下  2：左摇杆上下  3：左摇杆左右 
//拨杆：上0 ，下1567 4；左一拨杆  5；左二拨杆  6：右一拨杆 7：右二拨杆  
//8：左旋钮 0-1567  9右旋扭 0-1567 
	if (rc_ctrl.rc.ch[8] > 200 || DartCtrlHandle.RC_Fric_Count_Num < 100
		|| (ABS(DartCtrlHandle.Load.EncTarget - motor_data.Load.sumPosition) > 3000 && DartCtrlHandle.Load_Switch == 0 && ABS(motor_data.Lift.sumPosition - DartCtrlHandle.Lift.PositionZero) < 3000))
	{
		if (DartCtrlHandle.RC_Fric_Count_Num >= 100)
		{
			DartCtrlHandle.RC_Fric_Count_Num = 0;
		}
		DartCtrlHandle.RC_Fric_Count_Num++;
		DartCtrlHandle.Fric_Left.SpdTarget = 1200;
		DartCtrlHandle.Fric_Right.SpdTarget = -1200;
		DartCtrlHandle.Small_Fric.SpdTarget = 0;
		Small_Firc_SetCtrlMode(CtrlMode_speed);
		Fric_Left_SetCtrlMode(CtrlMode_speed);
		Fric_Right_SetCtrlMode(CtrlMode_speed);
	}
	else if (rc_ctrl.rc.ch[9] > 200)
	{
		DartCtrlHandle.Fric_Left.SpdTarget = -6300;
		DartCtrlHandle.Fric_Right.SpdTarget = 6300;
		
		if (ABS(motor_data.Fric_Left.speed) > 6000)
		{
			DartCtrlHandle.Small_Fric.SpdTarget = -2000;
		}
		Small_Firc_SetCtrlMode(CtrlMode_speed);
		Fric_Left_SetCtrlMode(CtrlMode_speed);
		Fric_Right_SetCtrlMode(CtrlMode_speed);
	}
	else
	{
		DartCtrlHandle.Fric_Left.SpdTarget = 0;
		DartCtrlHandle.Fric_Right.SpdTarget = 0;
		DartCtrlHandle.Small_Fric.SpdTarget = 0;
		Small_Firc_SetCtrlMode(CtrlMode_speed);
		Fric_Left_SetCtrlMode(CtrlMode_speed);
		Fric_Right_SetCtrlMode(CtrlMode_speed);
	}
	
	
	
	// 舵机控制    //遥控器模式下的舵机控制
	Servo_Remote_Control();
	
	if (DartCtrlHandle.Race_Mode == 3)
	{
		Yaw_Vision_Control();		//视觉自瞄识别,同时给金姐发信息
	}
}	

/**
  * @name   Servo_Remote_Control
  * @brief  舵机遥控模式下的控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
void Servo_Remote_Control(void)
{
	DartCtrlHandle.Trigger_Last = DartCtrlHandle.Trigger_Detect;
	
	DartCtrlHandle.Trigger_Detect = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
	
	// 检测摁住一瞬间
	if (DartCtrlHandle.Trigger_Detect == 1 && DartCtrlHandle.Trigger_Last == 0)
	{
		PWM_SetDuty(PWM_B4, Lock_PWM);      // 舵机锁住
		
		DartCtrlHandle.Fire_State = Waiting;  //等待发射
	}
	// 状态相同
	else if (DartCtrlHandle.Trigger_Detect == DartCtrlHandle.Trigger_Last)
	{
		if (rc_ctrl.rc.ch[7] != rc_ctrl.rc.ch_last[7])
		{
			if ((ABS(motor_data.Lift.sumPosition - DartCtrlHandle.Lift.PositionZero) < 2000 
				&& ABS(motor_data.Pull_Spring_Left.sumPosition - DartCtrlHandle.Pull_Spring_Left.PositionZero) < 24000
				&& ABS(motor_data.Pull_Spring_Right.sumPosition - DartCtrlHandle.Pull_Spring_Right.PositionZero) < 24000)  //抬升机构在底下
			|| (DartCtrlHandle.Race_Mode == 2 && ABS(motor_data.Lift.sumPosition - DartCtrlHandle.Lift.PositionZero) < 2000))  //抬升机构在底下
			{
				// 舵机松开
				PWM_SetDuty(PWM_B4, Release_PWM);
				DartCtrlHandle.Fire_State = OK; //发射欧克
			}
			else
			{
				PWM_SetDuty(PWM_B4, Lock_PWM);       //舵机锁
			}
						
		}
	}
	
}

/**
  * @name   Servo_Control
  * @brief  舵机控制  自动装填过程中的舵机控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
void Servo_Control(void)
{
	DartCtrlHandle.Trigger_Last = DartCtrlHandle.Trigger_Detect;
	
	DartCtrlHandle.Trigger_Detect = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
	
	if (DartCtrlHandle.Shoot_Num == 10)//当云台手拨了拨杆之后，开始使能计时
	{
		DartCtrlHandle.Time_Out = 0;
		DartCtrlHandle.Time_CMD = 1;
	}
	
	if (DartCtrlHandle.Time_CMD == 1)  //当云台手拨了拨杆之后，开始使能计时
	{
		DartCtrlHandle.Time++;
		DartCtrlHandle.Time_Out = 1;
	}
	
	if (DartCtrlHandle.Time >= 7500)    // 500 等于1秒  15秒就是7500
	{
		DartCtrlHandle.Time = 0;
		DartCtrlHandle.Shoot_Num = 0;   // 等于0 之后，就不允许发射了
		DartCtrlHandle.Time_CMD = 0;
		
		DartCtrlHandle.Time_Out = 2;
	}
	
	// 检测摁住一瞬间
	if (DartCtrlHandle.Trigger_Detect == 1 && DartCtrlHandle.Trigger_Last == 0)
	{
		PWM_SetDuty(PWM_B4, Lock_PWM);      // 舵机锁住
		
		DartCtrlHandle.Fire_State = Waiting;  //等待发射
	}
	// 状态相同
	else if (DartCtrlHandle.Trigger_Detect == DartCtrlHandle.Trigger_Last)  //如果前后两次的微动开关的值是一样的
	{
		if ((DartCtrlHandle.ShootStatus == Shoot_Continuous_Two) && DartCtrlHandle.Fire_State == Waiting) //如果是两联发，且发射状态是等待发射
		{
			if (rc_ctrl.rc.ch[7] != rc_ctrl.rc.ch_last[7])  // 拨一下右二拨杆，读到跳变，就标志着使能了两联发
			{
				DartCtrlHandle.Shoot_Num = 10;
			}
			
			if (DartCtrlHandle.Shoot_Num == 10 && DartCtrlHandle.Auto_Chambered_Count < 2)
			{
				if ((ABS(motor_data.Lift.sumPosition - DartCtrlHandle.Lift.PositionZero) < 2000) 
					&& (ABS(motor_data.Pull_Spring_Left.sumPosition - DartCtrlHandle.Pull_Spring_Left.PositionZero) < 24000)
					&& (ABS(motor_data.Pull_Spring_Right.sumPosition - DartCtrlHandle.Pull_Spring_Right.PositionZero) < 24000)
				&& get_err_0_flag == 2
					&& VisionData.IdentifyFlag == 1)  //抬升机构在底下
				{
					// 舵机松开
					PWM_SetDuty(PWM_B4, Release_PWM);
					DartCtrlHandle.Fire_State = OK; //发射欧克
					get_err_0_flag = 0;
				}
				else
				{
					PWM_SetDuty(PWM_B4, Lock_PWM);       //舵机锁
				}
			}
		}
		else if ((DartCtrlHandle.ShootTarget == Shoot_Continuous_Three) && DartCtrlHandle.Fire_State == Waiting)
		{
			if (rc_ctrl.rc.ch[7] != rc_ctrl.rc.ch_last[7])
			{
				DartCtrlHandle.Shoot_Num = 10;
			}
			
			if (DartCtrlHandle.Shoot_Num == 10 && DartCtrlHandle.Auto_Chambered_Count < 3)
			{
				if ((ABS(motor_data.Lift.sumPosition - DartCtrlHandle.Lift.PositionZero) < 2000) 
					&& (ABS(motor_data.Pull_Spring_Left.sumPosition - DartCtrlHandle.Pull_Spring_Left.PositionZero) < 24000)
					&& (ABS(motor_data.Pull_Spring_Right.sumPosition - DartCtrlHandle.Pull_Spring_Right.PositionZero) < 24000)
				//					&&  ABS(VisionData.x_pixel_err - Pixel_Offset) <= 1
				&& get_err_0_flag == 2
					&& VisionData.IdentifyFlag == 1)  //抬升机构在底下
				{
					// 舵机松开
					PWM_SetDuty(PWM_B4, Release_PWM);
					DartCtrlHandle.Fire_State = OK; //发射欧克
					get_err_0_flag = 0;
				}
				else
				{
					PWM_SetDuty(PWM_B4, Lock_PWM);       //舵机锁
				}
			}
		}
		else if (rc_ctrl.rc.ch[7] != rc_ctrl.rc.ch_last[7] && DartCtrlHandle.Fire_State == Waiting)  //单发
		{
			DartCtrlHandle.Shoot_Num = 10;
			if (((ABS(motor_data.Lift.sumPosition - DartCtrlHandle.Lift.PositionZero) < 2000) 
				&& (ABS(motor_data.Pull_Spring_Left.sumPosition - DartCtrlHandle.Pull_Spring_Left.PositionZero) < 24000)
				&& (ABS(motor_data.Pull_Spring_Right.sumPosition - DartCtrlHandle.Pull_Spring_Right.PositionZero) < 24000)
		//					&&  ABS(VisionData.x_pixel_err - Pixel_Offset) <= 1
				&& get_err_0_flag == 2
				&& VisionData.IdentifyFlag == 1)  //抬升机构在底下
			|| (DartCtrlHandle.Race_Mode == 2 && ABS(motor_data.Lift.sumPosition - DartCtrlHandle.Lift.PositionZero) < 2000))  //抬升机构在底下
			{
				// 舵机松开
				PWM_SetDuty(PWM_B4, Release_PWM);
				DartCtrlHandle.Fire_State = OK; //发射欧克
				get_err_0_flag = 0;
			}
			else
			{
				PWM_SetDuty(PWM_B4, Lock_PWM);       //舵机锁
			}
						
		}
	}	
		
}
		



/**
  * @name   Safe_Mode_Control
  * @brief  
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
void Safe_Mode_Control(void)
{
	// 摩擦轮
	PID_cleardata(&motor_pid.Fric_Left_Speed);
	PID_cleardata(&motor_pid.Fric_Right_Speed);
	PID_cleardata(&motor_pid.Small_Fric_Speed);

	// 装填
	PID_cleardata(&motor_pid.Load_Position);
	PID_cleardata(&motor_pid.Load_Speed);

	// 扳机
	PID_cleardata(&motor_pid.Move_Trigger_Position);
	PID_cleardata(&motor_pid.Move_Trigger_Speed);

	// 拉簧
	PID_cleardata(&motor_pid.Pull_Spring_Left_Position);
	PID_cleardata(&motor_pid.Pull_Spring_Right_Position);
	PID_cleardata(&motor_pid.Pull_Spring_Left_Speed);
	PID_cleardata(&motor_pid.Pull_Spring_Right_Speed);
	
	// Yaw
	PID_cleardata(&motor_pid.Yaw_Position);
	PID_cleardata(&motor_pid.Yaw_Speed);
	
	// 装填
	PID_cleardata(&motor_pid.Lift_Position);
	PID_cleardata(&motor_pid.Lift_Speed);
	
}

/**
  * @name   Move_TriggerCtrl_PositionCtrl
  * @brief  扳机的位置控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Move_TriggerCtrl_PositionCtrl(void)
{
    static uint8_t LoopCount = 0; 
    if(LoopCount++ % 2 == 0)
    {
        motor_data.Move_Trigger.positionTarget = DartCtrlHandle.Move_Trigger.EncTarget;
        PID_calc(&motor_pid.Move_Trigger_Position, motor_data.Move_Trigger.positionTarget, motor_data.Move_Trigger.sumPosition);
        
        DartCtrlHandle.Move_Trigger.SpdTarget = motor_pid.Move_Trigger_Position.out;
        motor_data.Move_Trigger.speedTarget = motor_pid.Move_Trigger_Position.out;
    }
    
    PID_calc(&motor_pid.Move_Trigger_Speed, motor_data.Move_Trigger.speedTarget, motor_data.Move_Trigger.speed);
    
    return motor_pid.Move_Trigger_Speed.out;
}


/**
  * @name   Move_TriggerCtrl_SpeedCtrl
  * @brief  扳机的速度控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
float Move_TriggerCtrl_SpeedCtrl(void)
{
    motor_data.Move_Trigger.speedTarget = DartCtrlHandle.Move_Trigger.SpdTarget;
    PID_calc(&motor_pid.Move_Trigger_Speed, motor_data.Move_Trigger.speedTarget, motor_data.Move_Trigger.speed);
    
    return motor_pid.Move_Trigger_Speed.out;    
}

/**
  * @name   Move_TriggerCtrl_PositionCtrl
  * @brief  装填电机的位置控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Load_PositionCtrl(void)
{
    static uint8_t LoopCount = 0; 
    if(LoopCount++ % 2 == 0)
    {
        motor_data.Load.positionTarget = DartCtrlHandle.Load.EncTarget;
        PID_calc(&motor_pid.Load_Position, motor_data.Load.positionTarget, motor_data.Load.sumPosition);
        
        DartCtrlHandle.Load.SpdTarget = motor_pid.Load_Position.out;
        motor_data.Load.speedTarget = motor_pid.Load_Position.out;
    }
    
    PID_calc(&motor_pid.Load_Speed, motor_data.Load.speedTarget, motor_data.Load.speed);
    
    return motor_pid.Load_Speed.out;
}

/**
  * @name   Load_SpeedCtrl
  * @brief  装填的速度控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Load_SpeedCtrl(void)
{
    motor_data.Load.speedTarget = DartCtrlHandle.Load.SpdTarget;
    PID_calc(&motor_pid.Load_Speed, motor_data.Load.speedTarget, motor_data.Load.speed);
    
    return motor_pid.Load_Speed.out;    
}

/**
  * @name   Move_TriggerCtrl_PositionCtrl
  * @brief  抬升电机的位置控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Lift_PositionCtrl(void)
{
    static uint8_t LoopCount = 0; 
    if(LoopCount++ % 2 == 0)
    {
        motor_data.Lift.positionTarget = DartCtrlHandle.Lift.EncTarget;
        PID_calc(&motor_pid. Lift_Position, motor_data.Lift.positionTarget, motor_data.Lift.sumPosition);
        
        DartCtrlHandle.Lift.SpdTarget = motor_pid.Lift_Position.out;
        motor_data.Lift.speedTarget = motor_pid.Lift_Position.out;
    }
    
    PID_calc(&motor_pid.Lift_Speed, motor_data.Lift.speedTarget, motor_data.Lift.speed);
    
    return motor_pid.Lift_Speed.out;
}

/**
  * @name  Lift_SpeedCtrl
  * @brief  抬升的速度控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Lift_SpeedCtrl(void)
{
    motor_data.Lift.speedTarget = DartCtrlHandle.Lift.SpdTarget;
    PID_calc(&motor_pid.Lift_Speed, motor_data.Lift.speedTarget, motor_data.Lift.speed);
    
    return motor_pid.Lift_Speed.out;    
}

/**
  * @name   Move_TriggerCtrl_PositionCtrl
  * @brief  拉簧左电机的位置控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Pull_Spring_Left_PositionCtrl(void)
{
    static uint8_t LoopCount = 0; 
    if(LoopCount++ % 2 == 0)
    {
        motor_data.Pull_Spring_Left.positionTarget = DartCtrlHandle.Pull_Spring_Left.EncTarget;
        PID_calc(&motor_pid.Pull_Spring_Left_Position, motor_data.Pull_Spring_Left.positionTarget, motor_data.Pull_Spring_Left.sumPosition);
        
        DartCtrlHandle.Pull_Spring_Left.SpdTarget = motor_pid.Pull_Spring_Left_Position.out;
        motor_data.Pull_Spring_Left.speedTarget = motor_pid.Pull_Spring_Left_Position.out;
    }
    
    PID_calc(&motor_pid.Pull_Spring_Left_Speed, motor_data.Pull_Spring_Left.speedTarget, motor_data.Pull_Spring_Left.speed);
    
    return motor_pid.Pull_Spring_Left_Speed.out;
}

/**
  * @name  Lift_SpeedCtrl
  * @brief  拉簧左电机速度控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Pull_Spring_Left_SpeedCtrl(void)
{
    motor_data.Pull_Spring_Left.speedTarget = DartCtrlHandle.Pull_Spring_Left.SpdTarget;
    PID_calc(&motor_pid.Pull_Spring_Left_Speed, motor_data.Pull_Spring_Left.speedTarget, motor_data.Pull_Spring_Left.speed);
    
    return motor_pid.Pull_Spring_Left_Speed.out;    
}


/**
  * @name   Move_TriggerCtrl_PositionCtrl
  * @brief  拉簧右电机的位置控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Pull_Spring_Right_PositionCtrl(void)
{
    static uint8_t LoopCount = 0; 
    if(LoopCount++ % 2 == 0)
    {
        motor_data.Pull_Spring_Right.positionTarget = DartCtrlHandle.Pull_Spring_Right.EncTarget;
        PID_calc(&motor_pid.Pull_Spring_Right_Position, motor_data.Pull_Spring_Right.positionTarget, motor_data.Pull_Spring_Right.sumPosition);
        
        DartCtrlHandle.Pull_Spring_Right.SpdTarget = motor_pid.Pull_Spring_Right_Position.out;
        motor_data.Pull_Spring_Right.speedTarget = motor_pid.Pull_Spring_Right_Position.out;
    }
    
    PID_calc(&motor_pid.Pull_Spring_Right_Speed, motor_data.Pull_Spring_Right.speedTarget, motor_data.Pull_Spring_Right.speed);
    
    return motor_pid.Pull_Spring_Right_Speed.out;
}

/**
  * @name  Lift_SpeedCtrl
  * @brief  拉簧右电机速度控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Pull_Spring_Right_SpeedCtrl(void)
{
    motor_data.Pull_Spring_Right.speedTarget = DartCtrlHandle.Pull_Spring_Right.SpdTarget;
    PID_calc(&motor_pid.Pull_Spring_Right_Speed, motor_data.Pull_Spring_Right.speedTarget, motor_data.Pull_Spring_Right.speed);
    
    return motor_pid.Pull_Spring_Right_Speed.out;    
}


/**
  * @name   Move_TriggerCtrl_PositionCtrl
  * @brief  Yaw6020电机的位置控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Yaw_PositionCtrl(void)
{
    static uint8_t LoopCount = 0; 
    if(LoopCount++ % 2 == 0)
    {
        motor_data.Yaw.positionTarget = DartCtrlHandle.Yaw.EncTarget;
        PID_calc(&motor_pid.Yaw_Position, motor_data.Pull_Spring_Right.positionTarget, motor_data.Yaw.sumPosition);
        
        DartCtrlHandle.Yaw.SpdTarget = motor_pid.Yaw_Position.out;
        motor_data.Yaw.speedTarget = motor_pid.Yaw_Position.out;
    }
    
    PID_calc(&motor_pid.Yaw_Speed, motor_data.Yaw.speedTarget, motor_data.Yaw.speed);
    
    return motor_pid.Yaw_Speed.out;
}

/**
  * @name  Lift_SpeedCtrl
  * @brief  Yaw电机速度控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Yaw_SpeedCtrl(void)
{
    motor_data.Yaw.speedTarget = DartCtrlHandle.Yaw.SpdTarget;
    PID_calc(&motor_pid.Yaw_Speed, motor_data.Yaw.speedTarget, motor_data.Yaw.speed);
    
    return motor_pid.Yaw_Speed.out;    
}

/**
  * @name  Fric_Left_SpeedCtrl
  * @brief  左摩擦轮速度控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Fric_Left_SpeedCtrl(void)
{
    motor_data.Fric_Left.speedTarget = DartCtrlHandle.Fric_Left.SpdTarget;
    PID_calc(&motor_pid.Fric_Left_Speed, motor_data.Fric_Left.speedTarget, motor_data.Fric_Left.speed);
    
    return motor_pid.Fric_Left_Speed.out;    
}


/**
  * @name  Fric_Right_SpeedCtrl
  * @brief  Yaw电机速度控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Fric_Right_SpeedCtrl(void)
{
    motor_data.Fric_Right.speedTarget = DartCtrlHandle.Fric_Right.SpdTarget;
    PID_calc(&motor_pid.Fric_Right_Speed, motor_data.Fric_Right.speedTarget, motor_data.Fric_Right.speed);
    
    return motor_pid.Fric_Right_Speed.out;    
}


/**
  * @name  Lift_SpeedCtrl
  * @brief  Yaw电机速度控制
  * @call   Internal
  * @param  NULL
  * @RetVal NULL
  */
static float Small_Fric_SpeedCtrl(void)
{
    motor_data.Small_Fric.speedTarget = DartCtrlHandle.Small_Fric.SpdTarget;
    PID_calc(&motor_pid.Small_Fric_Speed, motor_data.Small_Fric.speedTarget, motor_data.Small_Fric.speed);
    
    return motor_pid.Small_Fric_Speed.out;    
}

