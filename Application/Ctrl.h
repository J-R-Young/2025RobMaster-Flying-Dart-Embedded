#ifndef __Ctrl_h
#define __Ctrl_h

#include "headfiles.h"

#define RobotID_CampPin_Red         (0u)
#define RobotID_CampPin_Blue        (100u)

#define LEVER_DOWN_VALUE			1567
#define LEVER_UP_VALUE				0
#define LEVER_MID_VALUE             784

typedef enum
{
    // Red
    RobotID_Red_1_Hero              = RobotID_CampPin_Red + 1,
    RobotID_Red_2_Engineer          = RobotID_CampPin_Red + 2,
    RobotID_Red_3_Infantry          = RobotID_CampPin_Red + 3,
    RobotID_Red_4_Infantry          = RobotID_CampPin_Red + 4,
    RobotID_Red_5_Infantry          = RobotID_CampPin_Red + 5,
    RobotID_Red_6_Fly               = RobotID_CampPin_Red + 6,
    RobotID_Red_7_Sentinel          = RobotID_CampPin_Red + 7,
    RobotID_Red_8_Dart              = RobotID_CampPin_Red + 8,
    RobotID_Red_9_Radar             = RobotID_CampPin_Red + 9,

    // Blue
    RobotID_Blue_1_Hero             = RobotID_CampPin_Blue + 1,
    RobotID_Blue_2_Engineer         = RobotID_CampPin_Blue + 2,
    RobotID_Blue_3_Infantry         = RobotID_CampPin_Blue + 3,
    RobotID_Blue_4_Infantry         = RobotID_CampPin_Blue + 4,
    RobotID_Blue_5_Infantry         = RobotID_CampPin_Blue + 5,
    RobotID_Blue_6_Fly              = RobotID_CampPin_Blue + 6,
    RobotID_Blue_7_Sentinel         = RobotID_CampPin_Blue + 7,
    RobotID_Blue_8_Dart             = RobotID_CampPin_Blue + 8,
    RobotID_Blue_9_Radar            = RobotID_CampPin_Blue + 9,
    
}RobotID_enum;

typedef enum
{
    RobotLevel_1            = 1,
    RobotLevel_2            = 2,
    RobotLevel_3            = 3,
}RobotLevel_enum;

typedef enum    // 控制模式枚举
{
    CtrlWay_Safe        = 0,    	// 安全模式
    CtrlWay_RC          = 1 ,       // 遥控器模式
    CtrlWay_MK          = 2 ,       // 键鼠模式
	CtrlWay_Auto        = 3,		// 自瞄模式
	CtrlWay_Copt		= 4,		// 比赛模式
	CtrlWay_Navg		= 5,		// 导航模式
	CtrlWay_Init		= 6,		// 初始化模式

}CtrlWay_enum;
    
typedef enum    // 射击方式
{
    ShootMode_Empty         = 0,    // 退弹模式
    ShootMode_SingleShot    = 1,    // 单发模式
    ShootMode_ThreeShot     = 3,    // 三连发模式
    ShootMode_Burst         = 5,    // 连发模式
}ShootMode_enum;

typedef enum    // 弹速等级
{
    Fric_Level1             = 1,    // 一级弹速  15m
    Fric_Level2             = 2,    // 二级弹速  18m
    Fric_Level3             = 3,    // 三级弹速  20m
    Fric_Empty              = 4,    // 退弹弹速  尿~
}Fric_Level_enum;

typedef enum
{
    ChassisFollow_ON        = 1,    // 底盘跟随开
    ChassisFollow_OFF       = 0,    // 底盘跟随关
}ChassisFollowSwitch_enum;

typedef enum
{
    Navigation_ON               = 1,    // 视觉开
    Navigation_OFF              = 0,    // 视觉关
}NavigationSwitch_enum;

typedef enum
{
    Spin_ON                 = 1,    // 小陀螺开
    Spin_OFF                = 0,    // 小陀螺关
}SpinSwitch_enum;

typedef enum
{
    SuperPower_ON           = 1,    // 超电开
    SuperPower_OFF          = 0,    // 超电关
}SuperPowerSwitch_enum;

typedef enum
{
    Reach_Position_ON       = 1,    // 到达位置
    Reach_Position_OFF      = 0,    // 没到达位置
}Reach_Position_enum;

typedef enum
{
    // 正常情况下拨盘在这两个状态间切换
    PickStatus_Standby      = 0,        // 待机
    PickStatus_Loading      = 1,        // 正在装填（旋转中）
    
    // 特殊情况
    PickStatus_Timeout      = 2,        // 装填超时
    PickStatus_Stuck        = 3,        // 卡弹
}PickStatus_enum;

typedef enum
{
	Resurrection_Alive      = 0,		// 活着状态
	Resurrection_Judgment	   ,		// 判断到复活瞬间
	Resurrection_SteerReset    ,		// 对舵中
	Resurrection_Spin_Waitig   ,		// 小陀螺等待对正中			
}Resurrection_enum;

typedef struct
{
    /* ==== Car ==== */
    RobotID_enum    			RobotID;
    CtrlWay_enum    			CtrlWay;
    
    uint8_t                     UIreflashFLag;
    uint8_t 					Reset_Flag;				// 程序运行标志位
	
	/* ==== Navigation ====*/
	int16_t						Robot_X;
	int16_t						Robot_Y;
	uint8_t						Robot_ID;
	int16_t						Hero_Position_x;
	int16_t						Hero_Position_y;
	uint8_t						Identify_Flag;
	
	uint8_t						Pressed_Num;			// 云台手按下次数
	/* ==== Status ====*/
	uint16_t					Record_Current_HP;		// 记录血量
	
	uint8_t 					Inital_Flag;			// 判断复活时计时的标志位
	Resurrection_enum			Resurrection_Flag;		// 复活时的状态
	uint8_t						Resurrection_Ready;		// 复活完成
	
	uint8_t 					Death_Number;			// 死亡次数
	/* ==== Hurt ====*/
	uint8_t						ArmorHurt;				// 收到打击
	
    uint8_t 					Launch_Mode;			// 弹频模式
    /* ==== Navigation ==== */
    NavigationSwitch_enum       NavigationSwitch;
    
    ChassisFollowSwitch_enum    ChassisFollowStatus;
    SpinSwitch_enum             SpinSwitch;
    SuperPowerSwitch_enum       SuperPowerSwitch;
	Reach_Position_enum			Reach_Position;
    
    uint8_t                     icmReadyFlag;
    uint8_t 					Patrol_flag;
	uint8_t 					VisionMode;
	
	uint8_t                     Change_Point;
	uint8_t						Follow_Hero_Flag;
    /* ==== Chassis ==== */
    uint8_t                     SteerReset_flag;
    uint8_t                     SteerReset;
    uint8_t                     SteerResetStatus;
    
    /* ==== Launch ==== */
    struct 
    {
        uint8_t             Fric_Enable;
        uint8_t             Fric_Switch;
        
        Fric_Level_enum     Fric_Level;
        
        PickStatus_enum     PickStatus;
        
    }Launch;
    
	uint16_t 				Test_Shoot_Num;
	uint8_t					Copt_Chassis_State;
}CtrlHandle_TypeDef;


extern CtrlHandle_TypeDef hCtrl;
extern int Mark_Zero_CMD ;	
extern int Dart_State ;

void CtrlHandle_Init(CtrlHandle_TypeDef *CtrlHandle);
void CtrlHandle_Update(CtrlHandle_TypeDef *CtrlHandle);
void icmReadyChack(void);

#endif /* __Ctrl_h */

