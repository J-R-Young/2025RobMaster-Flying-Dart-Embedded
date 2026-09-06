#ifndef __Motor_h
#define __Motor_h

#include "headfiles.h"
#include "pid.h"

typedef struct
{
  int16_t          lastPosition;    /*上次电机转子编码器值*/

  int32_t 	    sumPosition;     /*电机转子编码器值累计和*/
    
 int32_t       sumPositionLast; /*上一个时刻电机转子编码器值累计和*/

  int16_t 		    position;        /*电机实时转子编码器值*/

  int32_t	positionTarget;  /*位置环的期望*/

  int16_t		    speed;           /*电机实时速度*/

  float 			speed_Filt;		/*电机滤波后的速度*/
	
  float             speedTarget;     /*速度环的期望*/

  uint8_t           temperature;     /*电机温度*/

  int32_t 		    turns;           /*电机从上电开始转过的圈数*/

  int16_t           current;         /*电机电流*/
	
  uint16_t          id;              /*电机id*/

}Motor_HandleTypeDef;

typedef struct
{
	PID_HandleTypeDef 	Lift_Speed;								// 抬升左电机速度环PID
	PID_HandleTypeDef 	Lift_Position;							// 抬升左电机位置环PID
								
	PID_HandleTypeDef   Pull_Spring_Left_Position;         		// 拉簧3508电机
	PID_HandleTypeDef   Pull_Spring_Left_Speed;            		// 拉簧3508电机
	
	PID_HandleTypeDef   Pull_Spring_Right_Position;				// 拉簧3508电机
	PID_HandleTypeDef   Pull_Spring_Right_Speed	;				// 拉簧3508电机
	
	PID_HandleTypeDef   Move_Trigger_Position ;                	// 扳机2006电机PID
	PID_HandleTypeDef   Move_Trigger_Speed;
	
	PID_HandleTypeDef 	Yaw_Speed;								// YAW轴电机速度环PID
	PID_HandleTypeDef 	Yaw_Position;							// YAW轴电机位置环PID

	PID_HandleTypeDef 	Fric_Left_Speed;						// 摩擦轮左电机速度环PID
	PID_HandleTypeDef 	Fric_Right_Speed;						// 摩擦轮右电机速度环PID

	PID_HandleTypeDef   Small_Fric_Speed;                       // 装填小摩擦轮电机速度环PID
	
	PID_HandleTypeDef	Load_Speed;								// 装填电机速度环PID
	PID_HandleTypeDef	Load_Position;							// 装填电机位置环PID
	
}MotorPID_struct;

typedef struct
{
	/*抬升左电机*/
	Motor_HandleTypeDef 	Lift;
	
	/*装填小摩擦轮*/
	Motor_HandleTypeDef 	Small_Fric;	
	
	//蓄力弹簧左电机3508
	Motor_HandleTypeDef 	Pull_Spring_Left;
	
	//蓄力弹簧右电机3508
	Motor_HandleTypeDef 	Pull_Spring_Right;		
	
	/*移动扳机位置的2006电机*/
	Motor_HandleTypeDef 	Move_Trigger;		
	
	/*YAW轴6020电机*/
	Motor_HandleTypeDef 	Yaw;	
	
	/*摩擦轮左电机*/   //3508
	Motor_HandleTypeDef 	Fric_Left;	
	
	/*摩擦轮右电机*/   //3508
	Motor_HandleTypeDef 	Fric_Right;		
	
	//2006装填同步带控制电机
	Motor_HandleTypeDef		Load;		                                                        
    
}MotorData_struct;

extern MotorPID_struct     motor_pid;
extern MotorData_struct    motor_data;

void Motor_HandleInit(void);
void Motor_PID_ParamInit(void);

#endif /* __Motor_h */

