/********************************************************************************************************************* 
* 文件名称          Referee.h
* 版本信息          v1.8.0
*
* 注意：
* 	图传链路的波特率为 921600
*
* 修改记录
* 日期              作者                备注
* 2025-03-01        Raze           		v1.7.0
* 2025-04-19        Raze           		v1.8.0
********************************************************************************************************************/
#ifndef __REFEREE_H
#define __REFEREE_H

#include "headfiles.h"

#define    LEN_HEADER    5        //帧头长
#define    LEN_CMDID     2        //命令码长度
#define    LEN_TAIL      2	      //帧尾CRC16


//起始字节,协议固定为0xA5
#define    REFEREE_FRAME_HEADER         (0xA5)

extern int referee_cnt;

typedef enum
{
  FRAME_HEADER         = 0,
  CMD_ID               = 5,
  DATA                 = 7,
} RefereeFrameOffset;

//5字节帧头,偏移位置
typedef enum
{
  SOF          = 0,//起始位
  DATA_LENGTH  = 1,//帧内数据长度,根据这个来获取数据长度
  SEQ          = 3,//包序号
  CRC8         = 4,//CRC8

} FrameHeaderOffset;

/***************命令码ID********************/

/*
命令码 数据段长度 功能说明
0x0001 11         比赛状态数据，1Hz 周期发送                                             服务器→全体机器人
0x0002 1          比赛结果数据，比赛结束触发发送                                         服务器→全体机器人
0x0003 32         机器人血量数据，固定 3Hz 频率发送                                      服务器→全体机器人
0x0101 4          场地事件数据，固定 1Hz 频率发送                                        服务器→己方全体机器人
0x0104 3          裁判警告数据，己方判罚/判负时触发发送，其余时间以 1Hz 频率发送          服务器→被处罚方全体机器人
0x0105 3          飞镖发射相关数据，固定 1Hz 频率发送                                    服务器→己方全体机器人
0x0201 13         机器人性能体系数据，固定 10Hz 频率发送                                 主控模块→对应机器人
0x0202 16         实时底盘功率和枪口热量数据，10Hz 频率发送                              主控模块→对应机器人
0x0203 16         机器人位置数据，固定 1Hz 频率发送                                     主控模块→对应机器人
0x0204 7          机器人增益数据，固定 3Hz 频率发送                                      服务器→对应机器人
0x0206 1          伤害状态数据，伤害发生后发送                                           主控模块→对应机器人
0x0207 7          实时射击数据，弹丸发射后发送                                           主控模块→对应机器人
0x0208 6          允许发弹量，固定 10Hz 频率发送                                         服务器→己方英雄、步兵、哨兵、空中机器人
0x0209 4          机器人 RFID 状态，固定 3Hz 频率发送                                    服务器→己方装有 RFID 模块的机器人
0x020A 6          飞镖选手端指令数据，飞镖闸门上线后固定 3Hz 频率发送                     服务器→己方飞镖机器人
0x020B 40         地面机器人位置数据，固定 1Hz 频率发送                                  服务器→己方哨兵机器人
                  场地围挡在红方补给站附近的交点为坐标原点，沿场地长边向蓝方为 X 轴正方向，沿场地短边向红方停机坪为 Y 轴正方向。
0x020C 1          雷达标记进度数据，固定 1Hz 频率发送                                    服务器→己方雷达机器人
0x020D 6          哨兵自主决策信息同步，固定以 1Hz 频率发送                              服务器→己方哨兵机器人
0x020E 1          雷达自主决策信息同步，固定以 1Hz 频率发送                               服务器→己方雷达机器人
0x0301 <128       机器人交互数据，发送方触发发送，频率上限为30Hz                          
0x0302 30         自定义控制器与机器人交互数据，发送方触发发送，频率上限为 30Hz  (图传)      自定义控制器→选手端图传连接的机器人 
0x0303 15         选手端小地图交互数据，选手端触发发送                                   选手端点击→服务器→发送方选择的己方机器人
0x0304 12         键鼠遥控数据，固定 30Hz 频率发送                              (图传)       客户端→选手端图传连接的机器人
0x0305 24         选手端小地图接收雷达数据，频率上限为5Hz                               雷达→服务器→己方所有选手端
0x0306 8          自定义控制器与选手端交互数据，发送方触发发送，频率上限为 30Hz           自定义控制器→选手端
0x0307 103        选手端小地图接收哨兵数据，频率上限为1Hz                                哨兵/半自动控制机器人→对应操作手选手端
0x0308 34         选手端小地图接收机器人数据，频率上限为 3Hz                             己方机器人→己方选手端
0x0309 30		  自定义控制器接收机器人数据，频率上限为 10Hz（图传）						己方机器人→对应操作手选手端连接的自定义控制器 
*/


//命令码ID,用来判断接收的是什么数据
typedef enum
{
	ID_game_state       		  = 0x0001,//比赛状态数据
	ID_game_result 				  = 0x0002,//比赛结果数据
	ID_game_robot_HP      		  = 0x0003,//机器人血量数据

	ID_event_data  				  = 0x0101,//场地事件数据
	ID_supply_projectile_action   = 0x0102,//场地补给站动作标识数据
	ID_referee_warning			  = 0x0104,//裁判警告数据
	ID_dart_info          		  = 0x0105,//己方飞镖发射剩余时间

	ID_game_robot_performance     = 0x0201,//机器人性能体系数据
	ID_power_heat_data    		  = 0x0202,//实时功率热量数据
	ID_game_robot_pos_self   	  = 0x0203,//本机器人位置数据
	ID_buff_musk				  = 0x0204,//机器人增益数据
	ID_robot_hurt				  = 0x0206,//伤害状态数据
	ID_shoot_data			      = 0x0207,//实时射击数据
	ID_bullet_remaining			  = 0x0208,//允许发弹量，空中机器人以及哨兵机器人发送，1Hz 周期发送
	ID_RFID_status                = 0x0209,//机器人 RFID 状态
	ID_dart_command               = 0x020A,//飞镖选手端指令数据
	ID_game_robot_pos             = 0x020B,//己方地面机器人位置数据
	ID_radar_mark_level           = 0x020C,//雷达标记进度数据
	ID_sentry_info                = 0x020D,//哨兵自主决策信息同步
	ID_radar_info                 = 0x020E,//雷达自主决策信息同步

	ID_student_ui_                = 0x0301,//机器人间交互数据（发给选手端即是UI）
	ID_self_controller_robot      = 0x0302,//自定义控制器与机器人交互数据接口
	ID_map_data_screen            = 0x0303,//选手端小地图交互数据
	ID_key_mouse                  = 0x0304,//键鼠遥控数据
	ID_map_receive_radar          = 0x0305,//选手端小地图接收雷达数据
	ID_self_controller_screen     = 0x0306,//自定义控制器与选手端交互数据接口
	ID_map_receive_sentry         = 0x0307,//选手端小地图接收哨兵数据
	ID_custom_info				  = 0x0308,//选手端小地图接收机器人数据
	ID_robot_custom				  = 0x0309//自定义控制器接收机器人数据
} CmdID;


//命令码数据段长,根据官方协议来定义长度
typedef enum//不全，按需求补
{
	LEN_game_state       			=  11,	//0x0001
	LEN_game_result      			=   1,	//0x0002
	LEN_game_robot_HP		        =  32,	//0x0003

	LEN_event_data  				=   4,	//0x0101
	LEN_referee_warning         	=   3,	//0x0104
	LEN_dart_remaining_time			=   3,	//0x0105

	LEN_game_robot_state    		=  13,	//0x0201
	LEN_power_heat_data   			=  16,	//0x0202
	LEN_game_robot_pos_self    		=  16,	//0x0203
	LEN_buff_musk        			=   7,	//0x0204
	LEN_robot_hurt        			=   1,	//0x0206
	LEN_shoot_data       			=   7,	//0x0207
	LEN_bullet_remaining			=   6,  //0x0208
	LEN_RFID_status              	=   4,  //0x0209
	LEN_dart_command             	=   6,  //0x020A
	LEN_game_robot_pos           	=  40,  //0x020B
	LEN_radar_mark_level         	=   1,  //0x020C
	LEN_sentry_info               	=   6,  //0x020D    
	LEN_radar_info               	=   1,  //0x020E
	LEN_robot_interaction_data   	=  10,	//0x0301
	LEN_self_controller_robot     	=  30,  //0x0302
	LEN_map_data_screen         	=  15,  //0x0303
	LEN_key_mouse                 	=  12,  //0x0304
	LEN_map_receive_radar        	=  24,  //0x0305
	LEN_self_controller_screen      =   8,  //0x0306
	LEN_map_receive_sentry         	= 103,  //0x0307
	LEN_custom_info                 =  34,  //0x0308
	LEN_robot_custom                =  30,  //0x0309
	
} RefereeDataLength;

///* 自定义帧头 */
//typedef struct __attribute__((packed))
//{
//  uint8_t  SOF;
//  uint16_t DataLength;
//  uint8_t  Seq;
//  uint8_t  CRC8;
//  //uint16_t Cmd_ID;
//}xFrameHeader;

/* ID: 0x0001  Byte:  11    比赛状态数据 */
typedef struct __attribute__((packed))
{
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
	uint64_t SyncTimeStamp;
}
game_status_t;


/* ID: 0x0002  Byte:  1    比赛结果数据 */
typedef struct __attribute__((packed))
{
	uint8_t winner;
}
game_result_t;


/* ID: 0x0003  Byte:  32     机器人血量数据 */
typedef struct __attribute__((packed))
{
	uint16_t red_1_robot_HP;
	uint16_t red_2_robot_HP;
	uint16_t red_3_robot_HP;
	uint16_t red_4_robot_HP;
	uint16_t red_5_robot_HP;
	uint16_t red_7_robot_HP;
	uint16_t red_outpost_HP;
	uint16_t red_base_HP;
	uint16_t blue_1_robot_HP;
	uint16_t blue_2_robot_HP;
	uint16_t blue_3_robot_HP;
	uint16_t blue_4_robot_HP;
	uint16_t blue_5_robot_HP;
	uint16_t blue_7_robot_HP;
	uint16_t blue_outpost_HP;
	uint16_t blue_base_HP;
}
game_robot_HP_t;


/* ID: 0x0101  Byte:  4    场地事件数据 */
typedef struct __attribute__((packed))
{
	uint32_t event_data;
}
event_data_t;


/* ID: 0x0102  Byte:  4    场地补给站动作标识数据 */
typedef struct __attribute__((packed))
{
	uint8_t reserved;//保留，即未使用-----v1.5为补给站ID
	uint8_t supply_robot_id;
	uint8_t supply_projectile_step;
	uint8_t supply_projectile_num;
} 
ext_supply_projectile_action_t;


/* ID: 0X0104  Byte:  3    裁判警告信息 */
typedef struct __attribute__((packed))
{
	uint8_t level;
	uint8_t offending_robot_id;
	uint8_t count;
}
referee_warning_t;


/* ID: 0X0105  Byte:  3    飞镖发射口倒计时 */
typedef struct __attribute__((packed))
{
	uint8_t dart_remaining_time;
	uint16_t dart_info;
}
dart_info_t;;


/* ID: 0X0201  Byte:   13    机器人状态数据 */
typedef struct __attribute__((packed))
{
	uint8_t robot_id;
	uint8_t robot_level;
	uint16_t current_HP; 
	uint16_t maximum_HP;
	uint16_t shooter_barrel_cooling_value;           //机器人枪口热量每秒冷却值
	uint16_t shooter_barrel_heat_limit;              //机器人枪口热量上限
	uint16_t chassis_power_limit;                    //机器人底盘功率上限
	uint8_t power_management_gimbal_output : 1;
	uint8_t power_management_chassis_output : 1; 
	uint8_t power_management_shooter_output : 1;
}
robot_status_t;


/* ID: 0x0202  Byte:   16    实时功率热量数据 */
typedef struct __attribute__((packed))
{
	uint16_t chassis_voltage;
	uint16_t chassis_current;
	float    chassis_power;   //瞬时功率
	uint16_t buffer_energy;//60焦耳缓冲能量
	uint16_t shooter_17mm_1_barrel_heat;
	uint16_t shooter_17mm_2_barrel_heat;
	uint16_t shooter_42mm_barrel_heat;
}
power_heat_data_t;


/* ID: 0x0203  Byte:   16    机器人位置数据 */
typedef struct __attribute__((packed))
{
	float x;
	float y;
	float angle;
}
robot_pos_t;


/* ID: 0x0204  Byte:  6    机器人增益数据 */
typedef struct __attribute__((packed))
{
	uint8_t recovery_buff;
	uint8_t cooling_buff;
	uint8_t defence_buff;
	uint8_t vulnerability_buff;
	uint16_t attack_buff;
}
buff_t;


/* ID: 0x0205  Byte:  2    空中机器人能量状态数据 */
typedef struct __attribute__((packed))
{
	uint8_t airforce_status;
	uint8_t time_remain;
}
air_support_data_t;


/* ID: 0x0206  Byte:  1    伤害状态数据 */
typedef struct __attribute__((packed))
{
	uint8_t armor_id : 4;
	uint8_t HP_deduction_reason : 4;
}
hurt_data_t;


/* ID: 0x0207  Byte:  7    实时射击数据 */
typedef struct __attribute__((packed))
{
	uint8_t bullet_type;
	uint8_t shooter_number;
	uint8_t launching_frequency;
	float initial_speed;
}
shoot_data_t;


/* ID: 0x0208  Byte:  6  子弹剩余发射数 */
typedef struct __attribute__((packed))
{
	uint16_t projectile_allowance_17mm;
	uint16_t projectile_allowance_42mm;
	uint16_t remaining_gold_coin;
}
projectile_allowance_t;


/*机器人 RFID 状态：0x0209。发送频率：1Hz，发送范围：单一机器人*/
typedef struct __attribute__((packed))
{
	uint32_t rfid_status;
}
rfid_status_t;


/*飞镖机器人客户端指令数据：0x020A。发送频率：10Hz，发送范围：单一机器人*/
typedef struct __attribute__((packed))
{
	uint8_t dart_launch_opening_status;
	uint8_t reserved;
	uint16_t target_change_time;
	uint16_t latest_launch_cmd_time;
}
dart_client_cmd_t;


/*地面机器人位置数据：0x020B。发送频率：1Hz，发送范围：单一机器人*/
typedef struct __attribute__((packed))
{
	float hero_x;
	float hero_y;
	float engineer_x;
	float engineer_y;
	float standard_3_x;
	float standard_3_y;
	float standard_4_x;
	float standard_4_y;
	float standard_5_x;
	float standard_5_y;
}
ground_robot_position_t;


/*雷达标记进度数据：0x020C。发送频率：1Hz，发送范围：单一机器人*/
typedef struct __attribute__((packed))
{
	uint8_t mark_hero_progress;
	uint8_t mark_engineer_progress;
	uint8_t mark_standard_3_progress;
	uint8_t mark_standard_4_progress;
	uint8_t mark_standard_5_progress;
	uint8_t mark_sentry_progress;
}
radar_mark_data_t;

/*哨兵自主决策信息同步：0x020D。发送频率：固定1Hz，发送范围：己方哨兵机器人*/
typedef struct __attribute__((packed))
{
 uint32_t sentry_info;
} 
sentry_info_t;

/*雷达自主决策信息同步：0x020E。发送频率：固定1Hz，发送范围：己方雷达机器人*/
typedef struct __attribute__((packed))
{
	uint8_t radar_info;
} 
radar_info_t;

/*多车通信：0x0301。发送频率：最高30Hz，发送范围：所有机器人*/
typedef struct __attribute__((packed))
{ 
	uint16_t data_cmd_id; 
	uint16_t sender_id; 
	uint16_t receiver_id; 
	float Radar_Yaw;
 
}
robot_interaction_data_t; 

/*自定义遥控器数据：0x0302。发送频率：最大30Hz，发送范围：单一机器人*/
typedef struct __attribute__((packed))
{
	 uint8_t data[30];
}
custom_robot_data_t;


/*选手端小地图交互数据：0x0303。发送频率：选手端触发发送(最大1/3Hz)发送范围：单一机器人*/
typedef struct __attribute__((packed))
{
	 float target_position_x;
	 float target_position_y;
	 uint8_t cmd_keyboard;
	 uint8_t target_robot_id;
	 uint8_t cmd_source;
}
map_command_t;


/*键鼠遥控数据：0x0304。发送频率：30Hz，发送范围：单一机器人*/
typedef struct __attribute__((packed))
{
	 int16_t mouse_x;
	 int16_t mouse_y;
	 int16_t mouse_z;
	 int8_t left_button_down;
	 int8_t right_button_down;
	 uint16_t keyboard_value;
	 uint16_t reserved;
}
remote_control_t;

/*雷达发送坐标数据：0x0305。发送频率：最高5Hz，发送范围：己方所有选手端*/
typedef struct __attribute__((packed))
{ 
	uint16_t hero_position_x; 
	uint16_t hero_position_y; 
	uint16_t engineer_position_x; 
	uint16_t engineer_position_y; 
	uint16_t infantry_3_position_x; 
	uint16_t infantry_3_position_y; 
	uint16_t infantry_4_position_x; 
	uint16_t infantry_4_position_y; 
	uint16_t infantry_5_position_x; 
	uint16_t infantry_5_position_y; 
	uint16_t sentry_position_x; 
	uint16_t sentry_position_y; 
} 
map_robot_data_t;

/*自定义控制器模拟键鼠操作：0x0306。发送频率：最高5Hz，发送范围：己方所有选手端*/
typedef struct __attribute__((packed))
{ 
	uint16_t key_value; 
	uint16_t x_position:12; 
	uint16_t mouse_left:4; 
	uint16_t y_position:12; 
	uint16_t mouse_right:4; 
	uint16_t reserved; 
}
custom_client_data_t; 

/*哨兵或半自动发送路径坐标数据：0x0307。发送频率：最高5Hz，发送范围：己方所有选手端*/
typedef struct __attribute__((packed))
{ 
	uint8_t intention; 
	uint16_t start_position_x; 
	uint16_t start_position_y; 
	int8_t delta_x[49]; 
	int8_t delta_y[49]; 
	uint16_t sender_id; 
}
map_data_t;

/*机器人向己方任意选手端发送自定义的消息：0x0308。发送频率：最高5Hz，发送范围：己方所有选手端*/
typedef struct __attribute__((packed))
{ 
	uint16_t sender_id; 
	uint16_t receiver_id; 
	uint8_t user_data[30]; 
}
custom_info_t;

/*机器人向自定义控制器发送数据：0x0309。发送频率：最高5Hz，发送范围：己方所有选手端*/
typedef struct __attribute__((packed))
{ 
	uint8_t data; 
}
robot_custom_data_t; 

extern game_status_t       				        GameState;								//0x0001
extern game_result_t                  		    GameResult;								//0x0002
extern game_robot_HP_t		       				GameRobotHP;							//0x0003

extern event_data_t        			         	EventData;								//0x0101
extern ext_supply_projectile_action_t	        SupplyProjectileAction;		            //0x0102
extern referee_warning_t      					RefereeWarning;						    //0x0104
extern dart_info_t   			                Dart_info;					            //0x0105

extern robot_status_t		         	  	    GameRobotState;				     		//0x0201
extern power_heat_data_t		      			PowerHeatData;					     	//0x0202
extern robot_pos_t		             			GameRobotPos;							//0x0203
extern buff_t				        			BuffMusk;								//0x0204
extern air_support_data_t		    			AerialRobotEnergy;				        //0x0205
extern hurt_data_t			         			RobotHurt;								//0x0206
extern shoot_data_t					        	ShootData;								//0x0207
extern projectile_allowance_t					ProjectileAllowance;				    //0x0208
extern rfid_status_t                            RFIDStatus;                             //0x0209
extern dart_client_cmd_t                        Dart_Client_Cmd;                        //0x020A
extern ground_robot_position_t                  GroundRobotPos;                         //0x020B
extern radar_mark_data_t                        RadarMarkData;                          //0x020C
extern sentry_info_t                            Sentry_info;                            //0x020D
extern radar_info_t                             Radar_info;                             //0X020E
extern robot_interaction_data_t                 Robot_Interaction_Data;                 //0X0301
extern custom_robot_data_t                      CustomRobotData;                        //0X0302
extern map_command_t                            MapCommand;                             //0X0303
extern remote_control_t                         RemoteControlData;                      //0x0304
extern map_robot_data_t							Map_Robot_Data;							//0x0305
extern custom_client_data_t 					Custom_Client_Data;						//0x0306
extern map_data_t								Map_Data;								//0x0307
extern custom_info_t							Custom_Info;							//0x0308
extern robot_custom_data_t 						Robot_Custom_Data;						//0x0309

//extern xFrameHeader              FrameHeader;		//发送帧头信息



int referee_data_solve(uint8_t *ReadFromUsart);
void determine_ID(void);
uint8_t get_robot_id(void);
void get_chassis_power_and_buffer(float *power, float *buffer,uint16_t *powmax);
void get_shoot_heat0_limit_and_heat0(uint16_t *heat0_limit, uint16_t *heat0);
void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1);
void get_shoot_data(uint8_t *bullet_freq,	float *bullet_speed, uint8_t *bullet_speedlimit);

// CRC8
//unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned intdwLength,unsigned char ucCRC8);
//unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
//void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);

// CRC16
//uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
//uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
//void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);
#endif