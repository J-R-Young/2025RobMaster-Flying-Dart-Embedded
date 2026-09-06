/********************************************************************************************************************* 
* 文件名称          rc_SBUS.h
* 版本信息          v1.1.0

* 修改记录
* 日期              作者                备注
* 2025-03-23        Raze            second version
* 新增对FS-i6x的sbus解析和调用，可在rc_SBUS.h中修改RC_FRAME_LENGTH的值进行遥控器的一键更换，不影响图传链路键鼠
********************************************************************************************************************/

#ifndef __rc_SBUS_h
#define __rc_SBUS_h


#include "headfiles.h"

/* ----------------------- RC Channel Definition ---------------------------- */
#define RC_CH_VALUE_MIN             ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET          ((uint16_t)1024)
#define RC_CH_VALUE_MAX             ((uint16_t)1684)

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
#define RC_FRAME_LENGTH             (25u) //FS-i6x是25u，首尾必定0x0f和0x00，DT7是18u！
#define SBUS_RX_BUF_NUM             (36u)

/* ----------------------- RC Switch Definition ----------------------------- */
#define SW_L                        s[1]
#define SW_R                        s[0]
#define SW_L_Last                   s_last[1]
#define SW_R_Last                   s_last[0]
#define RC_SW_UP                    ((RC_FRAME_LENGTH == 25u) ? ((uint16_t) 0) : ((uint16_t)1))
#define RC_SW_MID                   ((RC_FRAME_LENGTH == 25u) ? ((uint16_t)784) : ((uint16_t)3))
#define RC_SW_DOWN                  ((RC_FRAME_LENGTH == 25u) ? ((uint16_t)1567) : ((uint16_t)2))


#define SwA ch[4] 											 //up:0 				down:	1567
#define SwB ch[5] 											 //up:0   				down:	1567
#define SwC ch[6] 											 //up:0   center:784    down:	1567
#define SwD ch[7] 											 //up:0   				down:	1567
#define VrA ch[8] 											 //left:0 			   	right:	1567
#define VrB ch[9] 											 //left:0 	 		 	right:	1567
/* ----------------------- PC Mouse Definition ------------------------------ */
#define MOUSE_PRESSED               ((uint16_t)1)
#define MOUSE_RAISED                ((uint16_t)0)

/* ----------------------- PC Key Definition -------------------------------- */
#define KEY_PRESSED_OFFSET_W        ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S        ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A        ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D        ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q        ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E        ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT    ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL     ((uint16_t)0x01<<7)

/* ----------------------- Data Struct ------------------------------------- */
typedef __packed struct
{
    __packed struct
    {
        int16_t     ch[10];
		int16_t     ch_last[10];
        uint8_t     s[2];
        uint8_t     s_last[2];
    }rc;
    
    __packed struct
    {
        int16_t x;
        int16_t x_last;
        int16_t y;
        int16_t y_last;
        int16_t z;
        int16_t z_last;
        uint8_t press_l;
        uint8_t press_r;
    }mouse;
    
    __packed struct
    {
        uint16_t v;
    }key;

}RC_ctrl_t;

/* ----------------------- Internal Data ----------------------------------- */
extern RC_ctrl_t rc_ctrl;

void remote_control_init(void);
const RC_ctrl_t *get_remote_control_point(void);
void SBUS_callback(void);

#endif /* __rc_SBUS_h */
