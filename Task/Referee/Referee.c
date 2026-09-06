/********************************************************************************************************************* 
* 文件名称          Referee.c
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
#include "Referee.h"
#include "crc.h"


/*****************系统数据定义**********************/
game_status_t       				    GameState;								//0x0001
game_result_t                  		    GameResult;								//0x0002
game_robot_HP_t		       				GameRobotHP;							//0x0003

event_data_t        			       	EventData;								//0x0101
referee_warning_t      					RefereeWarning;						    //0x0104
dart_info_t   				            Dart_info;					            //0x0105

robot_status_t		         	  	    GameRobotState;				     		//0x0201
power_heat_data_t		      			PowerHeatData;					     	//0x0202
robot_pos_t		             			GameRobotPos;							//0x0203
buff_t				        			BuffMusk;								//0x0204
hurt_data_t			         			RobotHurt;								//0x0206
shoot_data_t					    	ShootData;								//0x0207
projectile_allowance_t					ProjectileAllowance;				    //0x0208
rfid_status_t                           RFIDStatus;                             //0x0209
dart_client_cmd_t                       Dart_Client_Cmd;                        //0x020A
ground_robot_position_t                 GroundRobotPos;                         //0x020B
radar_mark_data_t                       RadarMarkData;                          //0x020C
sentry_info_t                           Sentry_info;                            //0x020D
radar_info_t                            Radar_info;                             //0X020E

robot_interaction_data_t				Robot_Interaction_Data;					//0X0301
custom_robot_data_t                     CustomRobotData;                        //0X0302
map_command_t                           MapCommand;                             //0X0303
remote_control_t                        RemoteControlData;                      //0x0304

map_robot_data_t						Map_Robot_Data;							//0x0305
custom_client_data_t 					Custom_Client_Data;						//0x0306
map_data_t								Map_Data;								//0x0307
custom_info_t							Custom_Info;							//0x0308
robot_custom_data_t 					Robot_Custom_Data;						//0x0309

xFrameHeader                            FrameHeader;		//发送帧头信息
/****************************************************/


int referee_data_solve(uint8_t *ReadFromUsart)
{
    uint16_t referee_length;//统计一帧数据长度
	static float Record_Position_x = 0;
	static float Record_Position_y = 0;

    int CmdID = 0;//数据命令码解析

    if (ReadFromUsart == NULL)
    {
        return -1;
    }

    memcpy(&FrameHeader,ReadFromUsart,LEN_HEADER);

    if(ReadFromUsart[0] == REFEREE_FRAME_HEADER)
    {
        if(Verify_CRC8_Check_Sum(ReadFromUsart, LEN_HEADER) == 1)
        {
            referee_length = ReadFromUsart[1] + LEN_HEADER + LEN_CMDID + LEN_TAIL;

            if(Verify_CRC16_Check_Sum(ReadFromUsart,referee_length) == 1)
            {
                // 获取帧ID
                CmdID = (ReadFromUsart[6] << 8 | ReadFromUsart[5]);
                
                //解析数据命令码,将数据拷贝到相应结构体中(注意拷贝数据的长度)
				//由于官方给的协议有Bug，实际数据段长度与协议中的不符，按照自己测的长度或者读裁判发的长度
                switch(CmdID)
                {
                    case ID_game_state:                     //0x0001 
                        memcpy(&GameState, (ReadFromUsart + DATA), sizeof(GameState));
                        break;

                    case ID_game_result:          		    //0x0002 
                        memcpy(&GameResult, (ReadFromUsart + DATA), sizeof(GameResult));
                        break;

                    case ID_game_robot_HP:                  //0x0003 
						hCtrl.Record_Current_HP = GameRobotState.current_HP;						
						memcpy(&GameRobotHP, (ReadFromUsart + DATA), sizeof(GameRobotHP));
                        break;

                    case ID_event_data:    				    //0x0101 ProjectileAllowance
                        memcpy(&EventData, (ReadFromUsart + DATA), sizeof(EventData));
                        break;
                    
                    case ID_referee_warning:                //0x0104 
                        memcpy(&RefereeWarning, (ReadFromUsart + DATA), sizeof(RefereeWarning));
                        break;

                    case ID_dart_info:                      //0x0105 
                        memcpy(&Dart_info, (ReadFromUsart + DATA), sizeof(Dart_info));
                        break;

                    case ID_game_robot_performance:         //0x0201 
                        memcpy(&GameRobotState, (ReadFromUsart + DATA), sizeof(GameRobotState));
                        break;

                    case ID_power_heat_data:      		    //0x0202 
                        memcpy(&PowerHeatData, (ReadFromUsart + DATA), sizeof(PowerHeatData));
                        break;

                    case ID_game_robot_pos_self:      		//0x0203
                        memcpy(&GameRobotPos, (ReadFromUsart + DATA), sizeof(GameRobotPos));
                        break;

                    case ID_buff_musk:      			    //0x0204
                        memcpy(&BuffMusk, (ReadFromUsart + DATA), sizeof(BuffMusk));
                        break;
					
                    case ID_robot_hurt:      			    //0x0206
                        memcpy(&RobotHurt, (ReadFromUsart + DATA), sizeof(RobotHurt));
                        break;

                    case ID_shoot_data:					    //0x0207
                        memcpy(&ShootData, (ReadFromUsart + DATA), sizeof(ShootData));
						hCtrl.Test_Shoot_Num++;
                        break;

                    case ID_bullet_remaining:      		    //0x0208
                        memcpy(&ProjectileAllowance, (ReadFromUsart + DATA), sizeof(ProjectileAllowance));
                        break;
                    case ID_RFID_status:                    //0x0209
                        memcpy(&RFIDStatus, (ReadFromUsart + DATA), sizeof(RFIDStatus));
                        break;
                    case ID_dart_command:                   //0x020A
                        memcpy(&Dart_Client_Cmd, (ReadFromUsart + DATA), sizeof(Dart_Client_Cmd));
                        break;
                    case ID_game_robot_pos:                 //0x020B
                        memcpy(&GroundRobotPos, (ReadFromUsart + DATA), sizeof(GroundRobotPos));
                        break;
                    case ID_radar_mark_level:               //0x020C
                        memcpy(&RadarMarkData, (ReadFromUsart + DATA), sizeof(RadarMarkData));
                        break;
                    case ID_sentry_info:                    //0x020D
                        memcpy(&Sentry_info, (ReadFromUsart + DATA), sizeof(Sentry_info));
                        break;
                    case ID_radar_info:                     //0x020E
                        memcpy(&Radar_info, (ReadFromUsart + DATA), sizeof(Radar_info));
                        break;
					case ID_student_ui_:
						memcpy(&Robot_Interaction_Data, (ReadFromUsart + DATA), sizeof(Robot_Interaction_Data));
						break;
                    case ID_self_controller_robot:          //0x0302
                        memcpy(&CustomRobotData, (ReadFromUsart + DATA), sizeof(CustomRobotData));
                        break;
                    case ID_map_data_screen:                //0x0303					
                        memcpy(&MapCommand, (ReadFromUsart + DATA), sizeof(MapCommand));
                        break;
                    case ID_key_mouse:                      //0x0304
                        memcpy(&RemoteControlData, (ReadFromUsart + DATA), sizeof(RemoteControlData));
                        break;  
                }
            }
        }
//        //首地址加帧长度,指向CRC16下一字节,用来判断是否为0xA5,用来判断一个数据包是否有多帧数据
        if(*(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + FrameHeader.DataLength + LEN_TAIL) == 0xA5)
        {
            //如果一个数据包出现了多帧数据,则再次读取
            referee_data_solve(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + FrameHeader.DataLength + LEN_TAIL);
        }
    }
    return 0;
}