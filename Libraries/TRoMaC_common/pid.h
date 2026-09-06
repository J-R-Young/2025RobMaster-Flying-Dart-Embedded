#ifndef __PID_H
#define __PID_H

#include "stdint.h"
#include "stdlib.h"

#define pid_Data_Limit(Data, min, max)      (((Data) < (min)) ? (min) : (((Data) > (max) ? (max) : (Data))))
#define pid_ABS(x)                          ((x) < 0 ? (-(x)) : (x))        

typedef unsigned char uint8_t;

typedef enum 
{
    PID_MODE_POSITION   = 0,
    PID_MODE_ADD,	
}PID_MODE_TypeDef;


typedef struct
{
    PID_MODE_TypeDef    mode;
    float               kp;
    float               ki;
    float               kd;
    float 	            iOutMax;
    float 	            outMax;
}PID_InitTypeDef;


typedef struct
{
	PID_MODE_TypeDef    mode;
    
	float               set;
	float 	            err3[3];

	float 	            kp;
	float 	            ki;
	float 	            kd;
	
	float 	            pOut;
	float 	            iOut;
	float 	            dOut;
	
	float 	            out;            
        
	float 	            iOutMax;
	float 	            outMax;
	
}PID_HandleTypeDef;

float PID_calc(PID_HandleTypeDef *pid, float exp_data, float real_data);
float PID_calc_err(PID_HandleTypeDef *pid, float err);
void  PID_init(PID_HandleTypeDef *pid, PID_InitTypeDef *PID);
void  PID_cleardata(PID_HandleTypeDef *pid);
void  PID_loadparam(PID_HandleTypeDef *pid, float kp, float ki, float kd);
void  PID_loadtarget(PID_HandleTypeDef *pid, float target);

#endif /* __PID_H */
