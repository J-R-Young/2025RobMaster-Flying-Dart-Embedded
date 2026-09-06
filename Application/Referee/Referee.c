#include "Referee.h"
#include "crc.h"

#define BLUE  0
#define RED   1

/*****************系统数据定义**********************/
game_status_t       				    GameState;								//0x0001
game_result_t                  		    GameResult;								//0x0002
game_robot_HP_t		       				GameRobotHP;							//0x0003

event_data_t        			       	EventData;								//0x0101
ext_supply_projectile_action_t	        SupplyProjectileAction;		            //0x0102
referee_warning_t      					RefereeWarning;						    //0x0104
dart_remaining_time_t   				DartRemaingTime;					    //0x0105

robot_status_t		         	  	    GameRobotState;				     		//0x0201
power_heat_data_t		      			PowerHeatData;					     	//0x0202
robot_pos_t		             			GameRobotPos;							//0x0203
buff_t				        			BuffMusk;								//0x0204
air_support_data_t		    			AerialRobotEnergy;				        //0x0205
hurt_data_t			         			RobotHurt;								//0x0206
shoot_data_t					    	ShootData;								//0x0207
projectile_allowance_t					ProjectileAllowance;				    //0x0208
rfid_status_t                           RFIDStatus;                             //0x0209
dart_client_cmd_t                       Dart_Client_Cmd;                        //0x020A
ground_robot_position_t                 GroundRobotPos;                         //0x020B
radar_mark_data_t                       RadarMarkData;                          //0x020C

custom_robot_data_t                     CustomRobotData;                        //0X0302
map_command_t                           MapCommand;                             //0X0303
remote_control_t                        RemoteControlData;                      //0x0304

xFrameHeader                            FrameHeader;		//发送帧头信息
/****************************************************/

uint8_t 	Self_ID;//当前机器人的ID
uint16_t 	SelfClient_ID;//发送者机器人对应的客户端ID


int referee_data_solve(uint8_t *ReadFromUsart)
{
    uint16_t referee_length;//统计一帧数据长度

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
                        memcpy(&GameRobotHP, (ReadFromUsart + DATA), sizeof(GameRobotHP));
                        break;

                    case ID_event_data:    				    //0x0101 ProjectileAllowance
                        memcpy(&EventData, (ReadFromUsart + DATA), sizeof(EventData));
                        break;
                    
                    case ID_supply_projectile_action:       //0x0102 
                        memcpy(&SupplyProjectileAction, (ReadFromUsart + DATA), sizeof(SupplyProjectileAction));
                        break;

                    case ID_referee_warning:                //0x0104 
                        memcpy(&RefereeWarning, (ReadFromUsart + DATA), sizeof(RefereeWarning));
                        break;

                    case ID_dart_remaining_time:            //0x0105 
                        memcpy(&DartRemaingTime, (ReadFromUsart + DATA), sizeof(DartRemaingTime));
                        break;

                    case ID_game_robot_performance:         //0x0201 
                        memcpy(&GameRobotState, (ReadFromUsart + DATA), sizeof(GameRobotState));
                        break;

                    case ID_power_heat_data:      		    //0x0202 
                        memcpy(&PowerHeatData, (ReadFromUsart + DATA), sizeof(PowerHeatData));
                        break;

                    case ID_game_robot_pos_self:      		    //0x0203
                        memcpy(&GameRobotPos, (ReadFromUsart + DATA), sizeof(GameRobotPos));
                        break;

                    case ID_buff_musk:      			    //0x0204
                        memcpy(&BuffMusk, (ReadFromUsart + DATA), sizeof(BuffMusk));
                        break;

                    case ID_aerial_robot_energy:      	    //0x0205
                        memcpy(&AerialRobotEnergy, (ReadFromUsart + DATA), sizeof(AerialRobotEnergy));
                        break;

                    case ID_robot_hurt:      			    //0x0206
                        memcpy(&RobotHurt, (ReadFromUsart + DATA), sizeof(RobotHurt));
                        ArmorHurtCallback();
                        break;

                    case ID_shoot_data:      			    //0x0207
                        memcpy(&ShootData, (ReadFromUsart + DATA), sizeof(ShootData));
                        break;

                    case ID_bullet_remaining:      			//0x0208
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
                    case ID_self_controller_robot:          //0x0302
                        memcpy(&CustomRobotData, (ReadFromUsart + DATA), sizeof(CustomRobotData));
                        break;
                    case ID_map_data_screen:                //0x0303
                        memcpy(&MapCommand, (ReadFromUsart + DATA), sizeof(MapCommand));
                        Map_MapDataCallback();
                        break;
                    case ID_key_mouse:                      //0x0304
                        memcpy(&RemoteControlData, (ReadFromUsart + DATA), sizeof(RemoteControlData));
                        break;  
                }
            }
        }
        //首地址加帧长度,指向CRC16下一字节,用来判断是否为0xA5,用来判断一个数据包是否有多帧数据
        if(*(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + FrameHeader.DataLength + LEN_TAIL) == 0xA5)
        {
            //如果一个数据包出现了多帧数据,则再次读取
            referee_data_solve(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + FrameHeader.DataLength + LEN_TAIL);
        }
    }
    return 0;
}


//int is_red_or_blue(void)
//{
//  Self_ID = GameRobotState.robot_id;

//  if(GameRobotState.robot_id > 100)
//    {
//      return BLUE;
//    }
//  else
//    {
//      return RED;
//    }
//}

//void determine_ID(void)
//{
//  int Color = is_red_or_blue();
//  if(Color == BLUE)
//    {
//      SelfClient_ID = 0x0110 + (Self_ID-100);//计算客户端ID
//    }
//  else if(Color == RED)
//    {
//      SelfClient_ID = 0x0100 + Self_ID;//计算客户端ID
//    }
//}

//uint8_t get_robot_id(void)
//{
//  return GameRobotState.robot_id;
//}

//void get_chassis_power_and_buffer(float *power, float *buffer,uint16_t *powmax)
//{
//  *power = PowerHeatData.chassis_power;
//  *buffer = PowerHeatData.chassis_power_buffer;
//  *powmax = GameRobotState.chassis_power_limit;

//}

//void get_shoot_heat0_limit_and_heat0(uint16_t *heat0_limit, uint16_t *heat0)
//{
//  *heat0_limit = GameRobotState.shooter_id1_17mm_cooling_limit;
//  *heat0 = PowerHeatData.shooter_heat0;

//}

//void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1)
//{
//  *heat1_limit = GameRobotState.shooter_id1_17mm_cooling_limit;
//  *heat1 = PowerHeatData.shooter_heat1;
//}

//void get_shoot_data(uint8_t *bullet_freq,	float *bullet_speed, uint8_t *bullet_speedlimit)
//{
//  *bullet_freq = ShootData.bullet_freq;
//  *bullet_speed = ShootData.bullet_speed;
//  *bullet_speedlimit = GameRobotState.shooter_id1_17mm_speed_limit;
//}

uint8_t Referee_GetParty(void)
{
    if(GameRobotState.robot_id > 100)   // 蓝方
    {
        return RobotParty_Blue;
    }
    
    return RobotParty_Red;   // 红方
}

