
//#ifndef __ChassisTask_h
//#define __ChassisTask_h


#ifndef __Dart_h
#define __Dart_h

#include "headfiles.h"

#define Pull_Spring_Dif	   23500

#define Lock_PWM   1070
#define Release_PWM 800

typedef struct 
{
	int Adjust_Flag;  		// 扳机上的2006校准运行的标志位
	int CtrlMode[2];            // 扳机控制方式    [0]当前   [1]上一次
	float   PidOut;             		//电机PID的输出
}MotorHandle_TypeDef;

typedef enum
{
    Fire_Charge            = 0,      //发射蓄力         
    Sliding_table_In_Place = 1,      //滑台到位 
	Wait_Fire   	   	   = 2,      //等待发射
    Fire_Ready             = 3,      //发射
	Finish_Fire            = 4,      //完成发射
}Auto_Load_enum;


typedef enum					// 控制模式的结构体
{
    CtrlMode_safe   = 0,        // 控制模式-安全      电机关闭输出
    CtrlMode_speed,             // 控制模式-速度      单速度环控制
    CtrlMode_position,          // 控制模式-位置      位置速度串级控制
    CtrlMode_omega,             // 控制模式-角速度    单角度环控制
    CtrlMode_angle,             // 控制模式-角度      角度角速度串级控制
}CtrlMode_enum;

typedef enum
{
    Target_Tower          			= 0,      //0x00
    Target_Base_Fixed          		,
    Target_Base_Fixed_Random        ,
	Target_Base_Movable_Random		,
}ShootTarget_enum;

typedef enum
{
	Shoot_discontinuous				= 0,
    Shoot_Continuous_Two         	,      //0x00
    Shoot_Continuous_Three         	,
}ShootStatus_enum;

typedef enum
{
    AutoHomeStatus_Default          = 0,      //0x00
    AutoHomeStatus_Phase_1          ,
    AutoHomeStatus_Phase_2          ,
    AutoHomeStatus_Phase_3          ,
  
    AutoHomeStatus_TimeOut          = 0xDD,   //0x80 + 0x40 + 0x10 + 0x0D
        
    AutoHomeStatus_Ready            = 0xFF,
}AutoHomeStatus_enum;

typedef struct 
{
	uint8_t AutoHome;        				// 初始化状态
	uint8_t Online_State;					// 在线状态标志
	float   SpdTarget;             			// 期望速度
    float   EncTarget;            			// 期望位置
	uint8_t CtrlMode[2];          			// 控制方式    [0]当前   [1]上一次
	int32_t PositionZero;    				// 扳机电机行程零点
	int32_t PidOut;							// 最终输出
	int32_t PositionEnd;
	
}MotorStateHandle_TypeDef;

typedef struct 
{
	uint8_t 					Reset_AutoHome;			// 校准标志
	uint8_t						Motor_Online;			// 电机校准在线标志
	uint8_t						Pull_Spring_Left_IO;	// 左链条接近开关标志
	uint8_t						Pull_Spring_Right_IO;	// 右链条接近开关标志
	int32_t						Load_Position_Mid;      // 同步带的位置变量
	int32_t						Load_Position_Mid_New; // 同步带的位置变量  //End
																		   //Mid_New
																		   //Mid
																		   //Zero
	
	uint8_t                     Trigger_Detect;         // 扳机上的微动开关的标志位
	
	uint8_t						Load_Switch;			// 装填上的微动开关，判断镖体是否已经装上装填
	
	int32_t 					Pull_Spring_Enc_Target_Up; // 拉簧电机的期望位置
	int32_t 					Pull_Spring_Enc_Target_Down; // 拉簧电机的期望位置
	
	uint8_t						Race_Mode;				// 比赛模式，由键显输入
	
	uint8_t						Chambered_Count;        // 发射镖数
	
	uint8_t						Auto_Chambered_Count;        // 发射镖数
	
	uint8_t						Fire_Detect;    		// 发射就位标志位
	
	uint16_t					Fire_After_Time;		// 发射之后的计时，时间到了之后，抬升机构在下降，防止抬升机构阻挡飞镖的飞行		
	
	uint16_t					Press_In_After_Time;    // 镖体压入后的时间
	
	uint16_t					Load_Num;				// 摩擦轮将飞镖压进抬升机构后的计时变量，是为了保证飞镖装进了抬升机构中
	
	uint8_t						Shoot_Num;				// 单次发射计数
	
	uint8_t						Lift_Position;          // 抬升机构的位置标志
	
	uint8_t						Pull_Spring_Position;   // 拉簧的位置标志
	
	uint8_t                     Load_Position;          //装填同步带位置的标志
	
	Auto_Load_enum				Auto_Load_Run_Flag;     //自动装填的运行标志位
	
	uint8_t						Load_Finish;			//同步带装填成功后的标志位
	
	int16_t 					Record_Right_Num;		// 记录右摇杆值，记录跳变
	
	int16_t						Servo_Time;				//扳机扣上需要延时
	
	int8_t                      Trigger_Last;			//微动开关上一次的值
	
	int8_t						Red;                    //我方是红方
	
	int8_t                      Blue;					//我方是蓝方
	
	int8_t                      Fire_State;				//根据舵机的控制，改变开火状态
	
	float						screw_2006_num;     	 // 扳机上丝杆的圈数
	
	float						Screw_Target_Num[4];	 //键显输入的期望的丝杆的圈数
	
	int16_t						Move_Trigger_Target_Num; //由键显输入的期望的丝杆的圈数得出的2006的转的圈数
	
	ShootTarget_enum			ShootTarget;             // 由云台手选择的目标
	
	int16_t						Offset[4];				 // 由键显输入的像素的偏置
	
	ShootStatus_enum			ShootStatus;
	
	uint8_t 					Copt_Shoot_Num;
	
	uint16_t					Yaw_Time_Count; 		// 6020电机的编码器会微小跳变，会造成I项的累加，特殊计时处理
	
	uint8_t 					Yaw_Position;			// Yaw轴电机的位置控制
	
	uint16_t					Time;					// 发射时间计时，超过15秒之后，就不允许发射
	
	uint8_t						Time_CMD;				// 发射计时使能
	
	uint8_t						Time_Out;				// 0:开始计时 1：计时中 2：时间到
	
	uint8_t						Run_Count;				// 运行次数，键显显示
	
	uint16_t					RC_Fric_Count_Num;
	
	uint8_t 					Vision_Last_State;
	
	uint8_t						Lost_Vision_CMD;
	
	int 						Vision_Time;
	
	uint8_t						never_seen_green;
	
	MotorStateHandle_TypeDef	Yaw;
	MotorStateHandle_TypeDef	Load;
	MotorStateHandle_TypeDef	Lift;
	MotorStateHandle_TypeDef	Fric_Left;
	MotorStateHandle_TypeDef	Fric_Right;
	MotorStateHandle_TypeDef	Small_Fric;
	MotorStateHandle_TypeDef	Move_Trigger;
	MotorStateHandle_TypeDef	Pull_Spring_Left;
	MotorStateHandle_TypeDef	Pull_Spring_Right;
	
}DartCtrlHandle_TypeDef;



extern DartCtrlHandle_TypeDef DartCtrlHandle;
/* =========== 宏定义 ================ */

/* =========== 全局变量声明 ========== */

/* =========== 函数声明 ============== */
#define Move_Trigger_SetCtrlMode(mode)    {   \
                                            DartCtrlHandle.Move_Trigger.CtrlMode[1] = DartCtrlHandle.Move_Trigger.CtrlMode[0];  \
                                            DartCtrlHandle.Move_Trigger.CtrlMode[0] = mode;   \
                                        }  

#define Load_SetCtrlMode(mode)    {   \
											DartCtrlHandle.Load.CtrlMode[1] = DartCtrlHandle.Load.CtrlMode[0];  \
											DartCtrlHandle.Load.CtrlMode[0] = mode;   \
										}  

#define Lift_SetCtrlMode(mode)    {   \
									DartCtrlHandle.Lift.CtrlMode[1] = DartCtrlHandle.Lift.CtrlMode[0];  \
									DartCtrlHandle.Lift.CtrlMode[0] = mode;   \
									} 


#define Pull_Spring_Left_SetCtrlMode(mode)    {   \
												DartCtrlHandle.Pull_Spring_Left.CtrlMode[1] = DartCtrlHandle.Pull_Spring_Left.CtrlMode[0];  \
												DartCtrlHandle.Pull_Spring_Left.CtrlMode[0] = mode;   \
												} 

#define Pull_Spring_Right_SetCtrlMode(mode)    {   \
												DartCtrlHandle.Pull_Spring_Right.CtrlMode[1] = DartCtrlHandle.Pull_Spring_Right.CtrlMode[0];  \
												DartCtrlHandle.Pull_Spring_Right.CtrlMode[0] = mode;   \
												} 

#define Yaw_SetCtrlMode(mode)    {   \
									DartCtrlHandle.Yaw.CtrlMode[1] = DartCtrlHandle.Yaw.CtrlMode[0];  \
									DartCtrlHandle.Yaw.CtrlMode[0] = mode;   \
									} 

#define Fric_Left_SetCtrlMode(mode)    {   \
										DartCtrlHandle.Fric_Left.CtrlMode[1] = DartCtrlHandle.Fric_Left.CtrlMode[0];  \
										DartCtrlHandle.Fric_Left.CtrlMode[0] = mode;   \
										} 

#define Fric_Right_SetCtrlMode(mode)    {   \
												DartCtrlHandle.Fric_Right.CtrlMode[1] = DartCtrlHandle.Fric_Right.CtrlMode[0];  \
												DartCtrlHandle.Fric_Right.CtrlMode[0] = mode;   \
												} 

#define Small_Firc_SetCtrlMode(mode)    {   \
												DartCtrlHandle.Small_Fric.CtrlMode[1] = DartCtrlHandle.Small_Fric.CtrlMode[0];  \
												DartCtrlHandle.Small_Fric.CtrlMode[0] = mode;   \
												} 
void Servo_Remote_Control(void);
void Servo_Control(void);			
void Yaw_AutoHome(void);												
void Trigger_AutoHome(void);	
void Load_AutoHome(void);
void Lift_AutoHome(void);					
void Pull_Spring_Left_AutoHome(void);
void Pull_Spring_Right_AutoHome(void);
												
										
void Dart_AutoHome(void);										
										
void Dart_Control(void);
void Move_Trigger_Init (void);
void Pull_Spring_Left_Init(void);
void Pull_Spring_Right_Init(void);
void Safe_Mode_Control(void);
												
float Move_TriggerCtrl_SpeedCtrl(void);


void RemoteControl_Dart(void) ;
#endif /* __ChassisTask_h */

