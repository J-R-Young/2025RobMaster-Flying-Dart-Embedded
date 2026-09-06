/*
	把这个当库使用，不用改
*/

#include "pid.h"

float PID_calc(PID_HandleTypeDef *pid, float exp_data, float real_data)
{
    if(pid == NULL)
    {
        return 0.0f;    
    }
    
    if(pid->mode == PID_MODE_POSITION)
    {
        pid->set = exp_data;

        pid->err3[2] = pid->err3[1];
        pid->err3[1] = pid->err3[0];
        pid->err3[0] = exp_data - real_data;//误差计算

        pid->pOut = pid->kp * pid->err3[0];
        pid->iOut+= (pid->ki * pid->err3[0]);
        pid->dOut = pid->kd * (pid->err3[0] - pid->err3[1]);
        
        if(pid->iOutMax != 0)//使用积分限幅
        {
            pid->iOut = pid_Data_Limit(pid->iOut, -(pid->iOutMax), pid->iOutMax);
        }
        
        pid->out = pid->pOut + pid->iOut + pid->dOut;
        
        if(pid->outMax != 0)//使用输出限幅
        {
            pid->out = pid_Data_Limit(pid->out, -(pid->outMax), pid->outMax);
        }
    }
    else if(pid->mode == PID_MODE_ADD)
    {
        pid->err3[2] = pid->err3[1];
        pid->err3[1] = pid->err3[0];
        pid->err3[0] = exp_data - real_data;//误差计算

        pid->pOut = pid->kp * (pid->err3[0] - pid->err3[1]);
        pid->iOut = pid->ki * pid->err3[0];
        pid->dOut = pid->kd * ((pid->err3[0]-pid->err3[1])-(pid->err3[1]-pid->err3[2]));

        pid->out += pid->pOut + pid->iOut + pid->dOut ;

        if(pid->outMax != 0)//使用输出限幅
        {
            pid->out = pid_Data_Limit(pid->out, -(pid->outMax), pid->outMax);
        }
    }

    return pid->out;
}

float PID_calc_err(PID_HandleTypeDef *pid, float err)
{
    if(pid == NULL)
    {
        return 0.0f;    
    }
    if(pid->mode == PID_MODE_POSITION)
    {
        pid->err3[2] = pid->err3[1];
        pid->err3[1] = pid->err3[0];
        pid->err3[0] = err;

        pid->pOut = pid->kp * pid->err3[0];
        pid->iOut+= (pid->ki * pid->err3[0]);
        pid->dOut = pid->kd * (pid->err3[0] - pid->err3[1]);
        
        if(pid->iOutMax != 0)//使用积分限幅
        {
            pid->iOut = pid_Data_Limit(pid->iOut, -(pid->iOutMax), pid->iOutMax);
        }
        
        pid->out = pid->pOut + pid->iOut + pid->dOut;
        
        if(pid->outMax != 0)//使用输出限幅
        {
            pid->out = pid_Data_Limit(pid->out, -(pid->outMax), pid->outMax);
        }
    }
    else if(pid->mode == PID_MODE_ADD)
    {
        pid->err3[2] = pid->err3[1];
        pid->err3[1] = pid->err3[0];
        pid->err3[0] = err;

        pid->pOut = pid->kp * (pid->err3[0] - pid->err3[1]);
        pid->iOut = pid->ki * pid->err3[0];
        pid->dOut = pid->kd * ((pid->err3[0]-pid->err3[1])-(pid->err3[1]-pid->err3[2]));

        pid->out += pid->pOut + pid->iOut + pid->dOut ;

        if(pid->outMax != 0)//使用输出限幅
        {
            pid->out = pid_Data_Limit(pid->out, -(pid->outMax), pid->outMax);
        }
    }
    return pid->out;
}

void PID_init(PID_HandleTypeDef *pid, PID_InitTypeDef *PID)
{
    if (pid == NULL || PID == NULL)
    {
        return;
    }
    
    pid->mode 			        = PID->mode;//增量式还是位置式啥的
    pid->kp 			        = PID->kp;
    pid->ki 			        = PID->ki;
    pid->kd				        = PID->kd;
    pid->iOutMax		        = PID->iOutMax;//积分限幅
    pid->outMax			        = PID->outMax;//输出限幅
}

void PID_cleardata(PID_HandleTypeDef *pid)
{
    pid->err3[0] = 0;
    pid->err3[1] = 0;
    pid->err3[2] = 0;
    pid->pOut = 0;
    pid->iOut = 0;
    pid->dOut = 0;
    pid->out = 0;
}

void PID_loadparam(PID_HandleTypeDef *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void PID_loadtarget(PID_HandleTypeDef *pid, float target)
{
    pid->set = target;
}
