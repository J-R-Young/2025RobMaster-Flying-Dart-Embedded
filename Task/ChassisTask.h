/*************************************************
Copyright (C), 2016-2023, TYUT JBD TRoMaC
File name: 
Author: 
Version:               
Date: 
Description:  
Others:      
Function List:
History:
<author>    <time>          <version>       <desc>
**************************************************/
#ifndef __ChassisTask_h
#define __ChassisTask_h

#include "headfiles.h"

typedef struct
{
	uint8_t control_way;//控制的方式，0是急停模式，1是遥控器模式，2是键盘模式#
	uint8_t chassis_follow;//0是底盘跟随，1是底盘不跟随#
	uint8_t last_status;//上一次是什么模式#
	uint8_t now_status;//当前是什么模式
	uint8_t frictiongear_status;//是否开启摩擦轮#
	uint8_t tuidan_mode;//是否退弹#
	uint8_t poker_status;//是否开启拨盘#
	uint8_t fire_mode;//射击模式,0是连发模式，1是3连发，2是单发模式#
	uint8_t Q_mode;//25Hz的高射频模式
	uint8_t spin_status;//是否开启小陀螺，0关闭，1打开#
	uint8_t spin_aim_status;//小陀螺按什么方式转动，0普通，1是正弦小陀螺，2是有打小陀螺功能的小陀螺
    
	uint8_t Navigation_stsus;//是否开启视觉，0关闭，1打开#
//	uint8_t Navigation_mode;//视觉模式选择，1自瞄，2小符，3大符#
	
	uint8_t	super;	        //是否使用超电模组
	uint8_t	super_status;   //是否开启超电，0是电池供电，1是超电供电。
	uint8_t	super_aim_status;   //是否开启超电，0是普通供电，1是用于飞坡

	uint16_t shot_bullet_num;//已经发射的子弹数量#
	uint16_t shot_bullet_num_aim;//期望发射子弹数量#
	
	uint8_t climb;			//是否爬坡，0是不爬坡，1是处于爬坡状态
	uint8_t  cover_status; 			//弹舱盖子的状态,0关闭，1开启#
	uint8_t  ui_renew;     //ui的状态
      uint8_t  ui_renew_last;
}status_data_t;	//车子状态

/* =========== 宏定义 ================ */

/* =========== 全局变量声明 ========== */

extern status_data_t status_data;

/* =========== 函数声明 ============== */

#endif /* __ChassisTask_h */

