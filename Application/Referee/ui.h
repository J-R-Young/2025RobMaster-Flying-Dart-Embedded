#ifndef UI_
#define UI_

#include "headfiles.h"

#include "Referee.h"

//自己设置的name索引
#define COVER 21
#define COVER_STATUS 22
#define CHASSIS_STATUS 23
#define GIMBAL_DIRECTION 24
#define CHASSIS_DIRECTION 25
#define PITCH_RULER_Y 26
#define PITCH_RULER_X 27
#define PITCH_NOW 28

#define SUPER_SIDE 29
#define SUPER_VOLT 30
#define SUPER 31
#define SUPER_STATUS 32

#define SHOOT_MODE 33
#define VISION_STATUS 34
#define VISION_MODE 35

#define SHOOT_MODE_ 36
#define VISION_STATUS_ 37
#define VISION_MODE_ 38

#define SPINSWITCH 39//小陀螺
#define PLATE 40//拨盘
#define FRICSTATUS  41//摩擦轮

//#define SUPER_STATUS 36


//线条字体颜色COLOR
//线条粗细可选
#define COLOR_MAIN_RB 0X0
#define COLOR_YELLOW 0X1
#define COLOR_GREEN  0X2
#define COLOR_ORANGE 0X3
#define COLOR_PURPLE 0X4
#define COLOR_PINK 0X5
#define COLOR_CYAN 0X6     //这个是青色
#define COLOR_BLACK 0X7
#define COLOR_WHITE  0X8
//图形类型GRAPHIC
#define LINE 0X0
#define RECTANGLE 0X1
#define CIRCLE 0X2
#define ELLIPSE 0X3   //椭圆
#define ARC 0X4       //圆弧
//数据类型
#define FLOAT_ 0X5
#define INT_   0X6
#define CHAR_  0X7
//图形操作
#define UI_NONE      0X0
#define UI_ADD       0X1
#define UI_CORRECT   0X2
#define UI_DELETE    0X3
//接受方选项
#define CLIENT 0
#define OTHER_ROBOT 1
//统一发送总ID
#define START_ID 0XA5
/*
内容 ID					长度（头结构长度+内容数据段长度）				功能说明
0x0200~0x02FF 						6+n 			    		己方机器人间通信
0x0100 								6+2                         客户端删除图形
0x0101 								6+15                        客户端绘制一个图形
0x0102  						   	6+30                        客户端绘制二个图形
0x0103  							6+75                        客户端绘制五个图形
0x0104 								6+105                       客户端绘制七个图形
0x0110  							6+45                        客户端绘制字符图形*/
//机器人交互数据ID
#define	RobotComData_ID     0x0269,	/* 车间交互，队伍自定义 */
#define	Drawing_Clean_ID	0x0100
#define	Drawing_1_ID        0x0101
#define	Drawing_2_ID        0x0102
#define	Drawing_5_ID     	0x0103
#define	Drawing_7_ID     	0x0104
#define	Drawing_Char_ID	    0x0110
//机器人交互数据长度
#define	Drawing_Clean_LENGTH	8
#define	Drawing_1_LENGTH	   21
#define	Drawing_2_LENGTH	   36
#define	Drawing_5_LENGTH	   81
#define	Drawing_7_LENGTH	  111
#define	Drawing_Char_LENGTH	   51
/*(由于存在多个内容 ID，
但整个 cmd_id 上行频率最大为 10Hz，
请合理安排带宽*/
//机器人 ID：
/*
1，英雄(红)；
2，工程(红)；
3/4/5，步兵(红)；
6，空中(红)；
7，哨兵(红)；
9，雷达站（红）；
101，英雄(蓝)；
102，工程(蓝)；
103/104/105，步兵(蓝)；
106，空中(蓝)；
107，哨兵(蓝)； 
109，雷达站（蓝）。

客户端 ID：
0x0101 为英雄操作手客户端(红)；
0x0102，工程操作手客户端((红)；
0x0103/0x0104/0x0105，步兵操作手客户端(红)；
0x0106，空中操作手客户端((红)；
0x0165，英雄操作手客户端(蓝)；
0x0166，工程操作手客户端(蓝)；
0x0167/0x0168/0x0169，步兵操作手客户端步兵(蓝)；0x016A，空中操作手客户端(蓝)
*/




//交互数据接收信息：0x0301
typedef  struct __attribute__((packed))
{
  uint16_t data_cmd_id;
  uint16_t send_ID;
  uint16_t receiver_ID;
}
ext_student_interactive_header_data_t;

//交互数据 机器人间通信：0x0301
typedef struct __attribute__((packed))
{
uint8_t data/*[]*/;
} 
robot_interactive_data_t;


//客户端删除图形 机器人间通信：0x0301
typedef struct __attribute__((packed))
{
uint8_t operate_tpye; 
uint8_t layer; 
} 
ext_client_custom_graphic_delete_t;
// 图形数据
typedef  struct __attribute__((packed))
{
  uint8_t graphic_name[3];
  uint32_t operate_tpye:3;
  uint32_t graphic_tpye:3;
  uint32_t layer:4;
  uint32_t color:4;
  uint32_t start_angle:9;
  uint32_t end_angle:9;
  uint32_t width:10;
  uint32_t start_x:11;
  uint32_t start_y:11;
  uint32_t radius:10;
  uint32_t end_x:11;
  uint32_t end_y:11;
}
graphic_data_struct_t;

/* 自定义帧头 */
typedef struct __attribute__((packed))
{
  uint8_t  SOF;
  uint16_t DataLength;
  uint8_t  Seq;
  uint8_t  CRC8;
  //uint16_t Cmd_ID;
}xFrameHeader;

typedef  struct __attribute__((packed))
{
  xFrameHeader   							txFrameHeader;//帧头
  uint16_t		 						    Cmd_ID;//命令码
  ext_student_interactive_header_data_t     dataFrameHeader;//数据段头结构
  graphic_data_struct_t                     grapic_data_struct;
  uint8_t                                   data[30];
  uint16_t		 						    FrameTail;//帧尾
}
ext_client_custom_character_t;

typedef  struct __attribute__((packed))
{
  xFrameHeader   							txFrameHeader;//帧头
  uint16_t		 						Cmd_ID;//命令码
  ext_student_interactive_header_data_t   dataFrameHeader;//数据段头结构
  graphic_data_struct_t  					grapic_data_struct;//数据段
  uint16_t		 						FrameTail;//帧尾
}
ext_client_custom_graphic_single_t;

////小地图下发信息标识：0x0303。发送频率：触发时发送
//typedef struct __attribute__((packed))
//{
//	float target_position_x;
//	float target_position_y;
//	float target_position_z;
//	uint8_t commd_keyboard;
//	uint16_t target_robot_ID;
//} 
//ext_robot_command_t;
	
//小地图接收信息标识：0x0305。最大接收频率：10Hz
typedef struct __attribute__((packed))
{
uint16_t target_robot_ID;
float target_position_x;
float target_position_y;
} ext_client_map_command_t;

//图传遥控信息标识：0x0304。发送频率：30Hz
typedef struct __attribute__((packed))
{
int16_t mouse_x;
int16_t mouse_y;
int16_t mouse_z;
int8_t left_button_down;
int8_t right_button_down;
uint16_t keyboard_value;
uint16_t reserved;
} ext_client_command_t;

//绘制图形字符函数
uint16_t referee_get_receiver_ID(uint16_t send_ID);
void referee_draw_char(uint8_t robot_id,char *string,uint8_t string_dex,uint8_t control_way,uint8_t color,uint16_t x,uint16_t y);
void referee_draw_float(uint8_t robot_id,int32_t data,uint8_t string_dex,uint8_t control_way,uint8_t color,uint16_t x,uint16_t y);
void referee_draw_int(uint8_t robot_id,int32_t data,uint8_t string_dex,uint8_t control_way,uint8_t color,uint16_t x,uint16_t y);
void referee_draw_line(uint8_t robot_id,uint8_t dex,uint8_t control_way,uint8_t color,uint16_t x,uint16_t y,uint16_t x_end,uint16_t y_end,uint8_t width);
void referee_draw_rectangle(uint8_t robot_id,uint8_t dex,uint8_t control_way,uint8_t color,uint16_t x,uint16_t y,uint16_t x_end,uint16_t y_end);
void referee_draw_circle(uint8_t robot_id,uint8_t dex,uint8_t control_way,uint8_t color,uint16_t x_centre,uint16_t y_centre,uint16_t radius,uint8_t width);
void referee_draw_ellipse(uint8_t robot_id,uint8_t dex,uint8_t control_way,uint8_t color,uint16_t x_centre,uint16_t y_centre,uint16_t x_radius,uint16_t y_radius);
void referee_draw_arc(uint8_t robot_id,uint8_t dex,uint8_t control_way,uint8_t color,int16_t start_angle,int16_t end_angle,uint16_t x_centre,uint16_t y_centre,uint16_t x_radius,uint16_t y_radius);

//ui封装指令
void ui_init(void);
void ui_test(void);//测试
void ui_show(void);
void ui_renew(void);




#endif
