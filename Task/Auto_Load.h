
#ifndef __AUTO_LOAD_h
#define __AUTO_LOAD_h

#include "headfiles.h"

typedef enum
{
    Up        			= 0,     //0x00
    Mid_New          	= 1,
    Mid        			= 2,
	Down			    = 3,
	Right				= 4,
	Left				= 5,
	Running				= 6,
	Waiting				= 7,
	OK                  = 8,
}Position_enum;

extern int Pixel_Offset;   //经过处理过后的像素偏置
extern uint8_t get_err_0_flag ;
void Fire_Ready_Judge(void);
int Compress_the_spring(void); //
int Load_Return(void);
int Pull_Spring_Return(void);
int Lift_Return(void);
void Trigger_Return(void);
void Auto_Load_Control(void);
int Drop_Lift(void);
void Fric_Press_In(void);
void Fric_Press_Out(void);
void Fric_Disable(void);
void Yaw_Vision_Control(void);
#endif /* __CAN_H__ */
