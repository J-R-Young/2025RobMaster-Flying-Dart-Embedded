#include "Motor.h"

/*电机结构体*/
MotorPID_struct     motor_pid;
MotorData_struct    motor_data;

//DartMotor_RouteTypeDef		Motor_RouteSummary;			//整车电机行程坐标结构体汇总
//Motor_ResetSummaryTypeDef           MotorResetSummary;//电机

void Motor_HandleInit(void)
{
	motor_data.Yaw.position      = 0XFFFF;
    motor_data.Lift.position     = 0XFFFF;
    motor_data.Load.position     = 0XFFFF;
    motor_data.Move_Trigger.position    = 0XFFFF;
    motor_data.Pull_Spring_Left.position      = 0XFFFF;
	motor_data.Pull_Spring_Right.position      = 0XFFFF;
}
