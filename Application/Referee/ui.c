/**
 ******************************************************************************
  *
  *	UI的封装程序，直接使用
  * 基于裁判协议v1.2
  * 暂未实现浮点数，整型的绘制
  * 2022.3.5 裁判v1.3 简单补充，未添加更多功能
  ******************************************************************************
**/
//#include "ui.h"
//#include "FreeRTOS.h"
//#include "task.h"//需要用到taskDelay
//#include "crc.h"
////#include <stdio.h>
//#include <string.h>
//#include "usart.h"
//#include "icm20602.h"
//#include "ChassisTask.h"
//#include "control_tool.h"
//#include "ChassisTask.h"
//#include "arm_math.h"
//#include "ReceiveTask.h"

#include "ui.h"
#include "crc.h"
#include "Referee.h"

ext_client_custom_graphic_single_t ext_client_custom_graphic_single;
ext_client_custom_character_t ext_client_custom_character;
long int i;
uint8_t shoot_last;

extern CtrlHandle_TypeDef hCtrl;

void ui_init(void)
{
  osDelay(40);
  //瞄准辅助线_竖
  referee_draw_line(GameRobotState.robot_id,50,UI_ADD,COLOR_YELLOW,925,550,925,325,3);    
  //瞄准辅助线_横上_4m
  referee_draw_line(GameRobotState.robot_id,51,UI_ADD,COLOR_YELLOW,850,450,1000,450,3);
  //瞄准辅助线_横下_5m
  referee_draw_line(GameRobotState.robot_id,52,UI_ADD,COLOR_YELLOW,850,425,1000,425,3);
      
  //瞄准辅助点
  referee_draw_circle(GameRobotState.robot_id,53,UI_ADD,COLOR_PINK,925,450,3,4);  
  referee_draw_circle(GameRobotState.robot_id,54,UI_ADD,COLOR_PINK,925,425,3,4);
      
  //死区警示_横
  referee_draw_line(GameRobotState.robot_id,55,UI_ADD,COLOR_GREEN,760,160,1160,160,4);
  //死区警示_斜
  referee_draw_line(GameRobotState.robot_id,56,UI_ADD,COLOR_GREEN,650,0,760,160,4);
  referee_draw_line(GameRobotState.robot_id,57,UI_ADD,COLOR_GREEN,1160,160,1270,0,4);   

  //小陀螺
  referee_draw_char(GameRobotState.robot_id,"SPINSWITCH",58,UI_ADD,COLOR_YELLOW,1450,680);  
  referee_draw_circle(GameRobotState.robot_id,SPINSWITCH,UI_ADD,COLOR_YELLOW,1400,677,7,5);      
  
  //拨盘送弹状态
  referee_draw_char(GameRobotState.robot_id,"PLATE",59,UI_ADD,COLOR_YELLOW,1450,730);
  referee_draw_circle(GameRobotState.robot_id,PLATE,UI_ADD,COLOR_ORANGE,1400,727,7,5);
  
  //摩擦轮
  referee_draw_char(GameRobotState.robot_id,"FRIC",60,UI_ADD,COLOR_YELLOW,1450,780);
  referee_draw_circle(GameRobotState.robot_id,FRICSTATUS,UI_ADD,COLOR_GREEN,1400,777,7,5);
}

void ui_renew(void)
{
  //瞄准辅助线_竖
  referee_draw_line(GameRobotState.robot_id,50,UI_CORRECT,COLOR_YELLOW,925,550,925,325,3);    
  //瞄准辅助线_横上_4m
  referee_draw_line(GameRobotState.robot_id,51,UI_CORRECT,COLOR_YELLOW,850,450,1000,450,3);
  //瞄准辅助线_横下_5m
  referee_draw_line(GameRobotState.robot_id,52,UI_CORRECT,COLOR_YELLOW,850,425,1000,425,3);
      
  //瞄准辅助点
  referee_draw_circle(GameRobotState.robot_id,53,UI_CORRECT,COLOR_PINK,925,450,3,4);  
  referee_draw_circle(GameRobotState.robot_id,54,UI_CORRECT,COLOR_PINK,925,425,3,4);
      
  //死区警示_横
  referee_draw_line(GameRobotState.robot_id,55,UI_CORRECT,COLOR_GREEN,760,160,1160,160,4);
  //死区警示_斜
  referee_draw_line(GameRobotState.robot_id,56,UI_CORRECT,COLOR_GREEN,650,0,760,160,4);
  referee_draw_line(GameRobotState.robot_id,57,UI_CORRECT,COLOR_GREEN,1160,160,1270,0,4);   

  //小陀螺
  referee_draw_char(GameRobotState.robot_id,"SPINSWITCH",58,UI_CORRECT,COLOR_YELLOW,1450,680);  
  referee_draw_circle(GameRobotState.robot_id,SPINSWITCH,UI_CORRECT,COLOR_YELLOW,1400,677,7,5);      
  
  //拨盘送弹状态
  referee_draw_char(GameRobotState.robot_id,"PLATE",59,UI_CORRECT,COLOR_YELLOW,1450,730);
  referee_draw_circle(GameRobotState.robot_id,PLATE,UI_CORRECT,COLOR_ORANGE,1400,727,7,5);
  
  //摩擦轮
  referee_draw_char(GameRobotState.robot_id,"FRIC",60,UI_CORRECT,COLOR_YELLOW,1450,780);
  referee_draw_circle(GameRobotState.robot_id,FRICSTATUS,UI_CORRECT,COLOR_GREEN,1400,777,7,5);
}

void ui_show(void)
{
//      //小陀螺
//      if(hCtrl.SpinSwitch == 1)
//        referee_draw_circle(GameRobotState.robot_id,SPINSWITCH,UI_CORRECT,COLOR_GREEN,1400,677,7,5);
//      else if(hCtrl.SpinSwitch == 0)
//        referee_draw_circle(GameRobotState.robot_id,SPINSWITCH,UI_CORRECT,COLOR_PURPLE,1400,677,7,5);
//      
//      //拨盘
//      if(hCtrl.Launch.PickStatus == 0)
//        referee_draw_circle(GameRobotState.robot_id,PLATE,UI_CORRECT,COLOR_ORANGE,1400,727,7,5);
//      else if(hCtrl.Launch.PickStatus == 3)
//        referee_draw_circle(GameRobotState.robot_id,PLATE,UI_CORRECT,COLOR_PURPLE,1400,727,7,5);
//      else
//        referee_draw_circle(GameRobotState.robot_id,PLATE,UI_CORRECT,COLOR_GREEN,1400,727,7,5);
//      
//      //摩擦轮
//      if(hCtrl.Launch.Fric_Switch == 1)
//        referee_draw_circle(GameRobotState.robot_id,FRICSTATUS,UI_CORRECT,COLOR_GREEN,1400,777,7,5);
//      else
//        referee_draw_circle(GameRobotState.robot_id,FRICSTATUS,UI_CORRECT,COLOR_PURPLE,1400,777,7,5);
      
	// 弹舱盖状态
//    if(status_data.cover_status == COVER_OPEN)
//        referee_draw_circle(GameRobotState.robot_id,COVER_STATUS,UI_CORRECT,COLOR_GREEN,200,797,7,5);
//    else if(status_data.cover_status == COVER_CLOSE)
//        referee_draw_circle(GameRobotState.robot_id,COVER_STATUS,UI_CORRECT,C9 OLOR_PURPLE,200,797,7,5);

    
    // 发射模式
//    if(status_data.fire_mode == CONTINUE_MODE)
//        referee_draw_char(GameRobotState.robot_id,"NORMAL",SHOOT_MODE_,UI_CORRECT,COLOR_YELLOW,250,655);
//    else if(status_data.fire_mode == THREE_MODE)
//        referee_draw_char(GameRobotState.robot_id,"THREE",SHOOT_MODE_,UI_CORRECT,COLOR_YELLOW,250,655);
//    else if(status_data.fire_mode == SINGLE_MODE)
//        referee_draw_char(GameRobotState.robot_id,"SINGLE",SHOOT_MODE_,UI_CORRECT,COLOR_YELLOW,250,655);

    // 视觉模式
//    if(status_data.vision_mode == AUTO_AIM)
//        referee_draw_char(GameRobotState.robot_id,"AUTO",VISION_MODE_,UI_CORRECT,COLOR_YELLOW,250,625);
//    else if(status_data.vision_mode == SMALL_BUFF)
//        referee_draw_char(GameRobotState.robot_id,"SMALL_BUFF",VISION_MODE_,UI_CORRECT,COLOR_CYAN,250,625);
//    else if(status_data.vision_mode == BIG_BUFF)
//        referee_draw_char(GameRobotState.robot_id,"BIG_BUFF",VISION_MODE_,UI_CORRECT,COLOR_GREEN,250,625);

    // 超电状态
//    if(status_data.super_status == 1)
//        referee_draw_circle(GameRobotState.robot_id,SUPER_STATUS,UI_CORRECT,COLOR_GREEN,200,747,7,5);
//    else if(status_data.super_status == 0)
//        referee_draw_circle(GameRobotState.robot_id,SUPER_STATUS,UI_CORRECT,COLOR_PURPLE,200,747,7,5);
//    if(superData.CapVot>15)
//        referee_draw_line(GameRobotState.robot_id,SUPER_VOLT,UI_CORRECT,COLOR_CYAN,95,683,95+superData.CapVot*10,683,12);
//    else
//        referee_draw_line(GameRobotState.robot_id,SUPER_VOLT,UI_CORRECT,COLOR_ORANGE,95,683,95+superData.CapVot*10,683,12);
  
    // 是否开启视觉
//    if(status_data.vision_stsus == 0)
//        referee_draw_rectangle(GameRobotState.robot_id,VISION_STATUS,UI_CORRECT,COLOR_YELLOW,650,300,1270,800);	
//    else if(status_data.vision_stsus == 1)
//        referee_draw_rectangle(GameRobotState.robot_id,VISION_STATUS,UI_CORRECT,COLOR_CYAN,650,300,1270,800);	
//    else if(status_data.vision_stsus == 2)
//        referee_draw_rectangle(GameRobotState.robot_id,VISION_STATUS,UI_CORRECT,COLOR_ORANGE,650,300,1270,800);	
//    else if(status_data.vision_stsus == 3)
//        referee_draw_rectangle(GameRobotState.robot_id,VISION_STATUS,UI_CORRECT,COLOR_GREEN,650,300,1270,800);	

    // 显示底盘相对云台的角度
//    referee_draw_line(GameRobotState.robot_id,CHASSIS_DIRECTION,UI_CORRECT,COLOR_CYAN,
//                    1300,600,1300 - sin(Degree_gimbal_for_chassis)*20,600 + cos(Degree_gimbal_for_chassis)*20,1);

    // 发射状态
//    shoot_last = status_data.fire_mode;
//  referee_draw_line(GameRobotState.robot_id,PITCH_RULER_Y,UI_CORRECT,COLOR_YELLOW,
//                    750,640 - icmData.pitch_mahony/32*100,750,440 - icmData.pitch_mahony/32*100,3);
//  referee_draw_line(GameRobotState.robot_id,PITCH_RULER_X,UI_CORRECT,COLOR_YELLOW,
//                    744,540 - icmData.pitch_mahony/32*100,756,540 - icmData.pitch_mahony/32*100,3);


}
void ui_test(void)
{
//	uint8_t name[3] = "001";
//	Draw_line_(name,UI_ADD,COLOR_YELLOW,880,550,950,550);
//	Draw_line_(name,UI_ADD,COLOR_YELLOW,910,520,950,520);
//	Draw_line_(name,UI_ADD,COLOR_YELLOW,910,490,950,490);
//	Draw_line_(name,UI_ADD,COLOR_YELLOW,880,460,950,460);
//	Draw_line_(name,UI_ADD,COLOR_YELLOW,910,430,950,430);
//
  //referee_draw_rectangle(GameRobotState.robot_id,61,UI_ADD,COLOR_YELLOW,400,400,800,600);
  //referee_draw_ellipse(GameRobotState.robot_id,63,UI_ADD,COLOR_YELLOW,600,600,20,9);
  //referee_draw_arc(GameRobotState.robot_id,64,UI_ADD,COLOR_YELLOW,0,60,1100,700,20,9);
  //referee_draw_float(GameRobotState.robot_id,(int32_t)37.6789*1000,65,UI_ADD,COLOR_YELLOW,700,1100);
  //referee_draw_int(GameRobotState.robot_id,37,65,UI_ADD,COLOR_YELLOW,700,1100);
  //referee_draw_char(GameRobotState.robot_id,"WPY",65,UI_ADD,COLOR_YELLOW,960,700);
}

uint16_t referee_get_receiver_ID(uint16_t send_ID)
{
  return (send_ID + 0x100);
}

void referee_draw_char(uint8_t robot_id,char *string,uint8_t string_dex,uint8_t control_way,uint8_t color,uint16_t x,uint16_t y)
{
  ext_client_custom_character.txFrameHeader.SOF = START_ID;
  ext_client_custom_character.txFrameHeader.DataLength = Drawing_Char_LENGTH;
  ext_client_custom_character.txFrameHeader.Seq = 0;
  Append_CRC8_Check_Sum((unsigned char*)&ext_client_custom_character,5);
  ext_client_custom_character.Cmd_ID = ID_student_ui_;
  ext_client_custom_character.dataFrameHeader.data_cmd_id = Drawing_Char_ID;
  ext_client_custom_character.dataFrameHeader.send_ID = robot_id;
  ext_client_custom_character.dataFrameHeader.receiver_ID = referee_get_receiver_ID(robot_id);
  ext_client_custom_character.grapic_data_struct.graphic_name[0]=string_dex;
  ext_client_custom_character.grapic_data_struct.graphic_name[1]=0x0;
  ext_client_custom_character.grapic_data_struct.graphic_name[2]=0x0;
  ext_client_custom_character.grapic_data_struct.operate_tpye = control_way;
  ext_client_custom_character.grapic_data_struct.graphic_tpye = CHAR_;
  ext_client_custom_character.grapic_data_struct.layer = 0;
  ext_client_custom_character.grapic_data_struct.color = color;
  ext_client_custom_character.grapic_data_struct.start_angle = 15;
  ext_client_custom_character.grapic_data_struct.end_angle = 10;
  ext_client_custom_character.grapic_data_struct.width = 2;
  ext_client_custom_character.grapic_data_struct.start_x = x;
  ext_client_custom_character.grapic_data_struct.start_y = y;
  ext_client_custom_character.grapic_data_struct.radius = 0;
  ext_client_custom_character.grapic_data_struct.end_x = 0;
  ext_client_custom_character.grapic_data_struct.end_y = 0;
  memset(ext_client_custom_character.data, 0, 30);//清空1号fifo缓存区
  strcpy((char *)ext_client_custom_character.data,string);
  Append_CRC16_Check_Sum((unsigned char*)&ext_client_custom_character,60);
  if(HAL_UART_Transmit(&huart_Referee,(unsigned char*)&ext_client_custom_character,60,200) != HAL_OK) {}
//  HAL_Delay(40);
        osDelay(40);
}

void referee_draw_float(uint8_t robot_id,int32_t data,uint8_t string_dex,uint8_t control_way,uint8_t color,uint16_t x,uint16_t y)
{
  ext_client_custom_character.txFrameHeader.SOF = START_ID;
  ext_client_custom_character.txFrameHeader.DataLength = Drawing_Char_LENGTH;
  ext_client_custom_character.txFrameHeader.Seq = 0;
  Append_CRC8_Check_Sum((unsigned char*)&ext_client_custom_character,5);
  ext_client_custom_character.Cmd_ID = ID_student_ui_;
  ext_client_custom_character.dataFrameHeader.data_cmd_id = Drawing_Char_ID;
  ext_client_custom_character.dataFrameHeader.send_ID = robot_id;
  ext_client_custom_character.dataFrameHeader.receiver_ID = referee_get_receiver_ID(robot_id);
  ext_client_custom_character.grapic_data_struct.graphic_name[0]=string_dex;
  ext_client_custom_character.grapic_data_struct.graphic_name[1]=0x0;
  ext_client_custom_character.grapic_data_struct.graphic_name[2]=0x0;
  ext_client_custom_character.grapic_data_struct.operate_tpye = control_way;
  ext_client_custom_character.grapic_data_struct.graphic_tpye = FLOAT_;
  ext_client_custom_character.grapic_data_struct.layer = 0;
  ext_client_custom_character.grapic_data_struct.color = color;
  ext_client_custom_character.grapic_data_struct.start_angle = 15;
  ext_client_custom_character.grapic_data_struct.end_angle = 2;
  ext_client_custom_character.grapic_data_struct.width = 2;
  ext_client_custom_character.grapic_data_struct.start_x = x;
  ext_client_custom_character.grapic_data_struct.start_y = y;
  ext_client_custom_character.grapic_data_struct.end_x = data>>8;
  ext_client_custom_character.grapic_data_struct.end_y = data;
  Append_CRC16_Check_Sum((unsigned char*)&ext_client_custom_character,60);
  if(HAL_UART_Transmit(&huart_Referee,(unsigned char*)&ext_client_custom_character,60,200) != HAL_OK) {}
//  HAL_Delay(40);
        osDelay(40);
  
}



void referee_draw_int(uint8_t robot_id,int32_t data,uint8_t string_dex,uint8_t control_way,uint8_t color,uint16_t x,uint16_t y)
{
  ext_client_custom_character.txFrameHeader.SOF = START_ID;
  ext_client_custom_character.txFrameHeader.DataLength = Drawing_Char_LENGTH;
  ext_client_custom_character.txFrameHeader.Seq = 0;
  Append_CRC8_Check_Sum((unsigned char*)&ext_client_custom_character,5);
  ext_client_custom_character.Cmd_ID = ID_student_ui_;
  ext_client_custom_character.dataFrameHeader.data_cmd_id = Drawing_Char_ID;
  ext_client_custom_character.dataFrameHeader.send_ID = robot_id;
  ext_client_custom_character.dataFrameHeader.receiver_ID = referee_get_receiver_ID(robot_id);
  ext_client_custom_character.grapic_data_struct.graphic_name[0]=string_dex;
  ext_client_custom_character.grapic_data_struct.graphic_name[1]=0x0;
  ext_client_custom_character.grapic_data_struct.graphic_name[2]=0x0;
  ext_client_custom_character.grapic_data_struct.operate_tpye = control_way;
  ext_client_custom_character.grapic_data_struct.graphic_tpye = INT_;
  ext_client_custom_character.grapic_data_struct.layer = 0;
  ext_client_custom_character.grapic_data_struct.color = color;
  ext_client_custom_character.grapic_data_struct.start_angle = 15;
  ext_client_custom_character.grapic_data_struct.end_angle = 0;
  ext_client_custom_character.grapic_data_struct.width = 2;
  ext_client_custom_character.grapic_data_struct.start_x = x;
  ext_client_custom_character.grapic_data_struct.start_y = y;
  memcpy((void*)ext_client_custom_character.grapic_data_struct.radius, &data, 4);
  Append_CRC16_Check_Sum((unsigned char*)&ext_client_custom_character,60);
  if(HAL_UART_Transmit(&huart_Referee,(unsigned char*)&ext_client_custom_character,60,200) != HAL_OK) {}
//  HAL_Delay(40);
        osDelay(40);
}


void referee_draw_line(uint8_t robot_id,uint8_t dex,uint8_t control_way,uint8_t color,uint16_t x,uint16_t y,uint16_t x_end,uint16_t y_end,uint8_t width)
{
  ext_client_custom_graphic_single.txFrameHeader.SOF = START_ID;
  ext_client_custom_graphic_single.txFrameHeader.DataLength = Drawing_1_LENGTH;
  ext_client_custom_graphic_single.txFrameHeader.Seq = 0;
  Append_CRC8_Check_Sum((unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single.txFrameHeader));
  ext_client_custom_graphic_single.Cmd_ID = ID_student_ui_;
  ext_client_custom_graphic_single.dataFrameHeader.data_cmd_id = Drawing_1_ID;
  ext_client_custom_graphic_single.dataFrameHeader.send_ID = robot_id;
  ext_client_custom_graphic_single.dataFrameHeader.receiver_ID = referee_get_receiver_ID(robot_id);
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[0]=dex;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[1]=0x0;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[2]=0x0;
  ext_client_custom_graphic_single.grapic_data_struct.operate_tpye = control_way;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_tpye = LINE;
  ext_client_custom_graphic_single.grapic_data_struct.layer = 0;
  ext_client_custom_graphic_single.grapic_data_struct.color = color;
  ext_client_custom_graphic_single.grapic_data_struct.start_angle = 0;
  ext_client_custom_graphic_single.grapic_data_struct.end_angle = 0;
  ext_client_custom_graphic_single.grapic_data_struct.width = width;
  ext_client_custom_graphic_single.grapic_data_struct.start_x = x;
  ext_client_custom_graphic_single.grapic_data_struct.start_y = y;
  ext_client_custom_graphic_single.grapic_data_struct.radius = 0;
  ext_client_custom_graphic_single.grapic_data_struct.end_x = x_end;
  ext_client_custom_graphic_single.grapic_data_struct.end_y = y_end;
  Append_CRC16_Check_Sum((unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single));
  if(HAL_UART_Transmit(&huart_Referee,(unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single),200) != HAL_OK)
    {
      i++;
    }
//  HAL_Delay(40);
    osDelay(40);
}

void referee_draw_rectangle(uint8_t robot_id,uint8_t dex,uint8_t control_way,uint8_t color,uint16_t x,uint16_t y,uint16_t x_end,uint16_t y_end)
{
  ext_client_custom_graphic_single.txFrameHeader.SOF = START_ID;
  ext_client_custom_graphic_single.txFrameHeader.DataLength = Drawing_1_LENGTH;
  ext_client_custom_graphic_single.txFrameHeader.Seq = 0;
  Append_CRC8_Check_Sum((unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single.txFrameHeader));
  ext_client_custom_graphic_single.Cmd_ID = ID_student_ui_;
  ext_client_custom_graphic_single.dataFrameHeader.data_cmd_id = Drawing_1_ID;
  ext_client_custom_graphic_single.dataFrameHeader.send_ID = robot_id;
  ext_client_custom_graphic_single.dataFrameHeader.receiver_ID = referee_get_receiver_ID(robot_id);
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[0]=dex;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[1]=0x0;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[2]=0x0;
  ext_client_custom_graphic_single.grapic_data_struct.operate_tpye = control_way;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_tpye = RECTANGLE;
  ext_client_custom_graphic_single.grapic_data_struct.layer = 0;
  ext_client_custom_graphic_single.grapic_data_struct.color = color;
  ext_client_custom_graphic_single.grapic_data_struct.start_angle = 0;
  ext_client_custom_graphic_single.grapic_data_struct.end_angle = 0;
  ext_client_custom_graphic_single.grapic_data_struct.width = 1;
  ext_client_custom_graphic_single.grapic_data_struct.start_x = x;
  ext_client_custom_graphic_single.grapic_data_struct.start_y = y;
  ext_client_custom_graphic_single.grapic_data_struct.radius = 0;
  ext_client_custom_graphic_single.grapic_data_struct.end_x = x_end;
  ext_client_custom_graphic_single.grapic_data_struct.end_y = y_end;
  Append_CRC16_Check_Sum((unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single));
  if(HAL_UART_Transmit(&huart_Referee,(unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single),200) != HAL_OK)
    {
      i++;
    }
//  HAL_Delay(40);
    osDelay(40);
}
void referee_draw_circle(uint8_t robot_id,uint8_t dex,uint8_t control_way,uint8_t color,uint16_t x_centre,uint16_t y_centre,uint16_t radius,uint8_t width)
{
  ext_client_custom_graphic_single.txFrameHeader.SOF = START_ID;
  ext_client_custom_graphic_single.txFrameHeader.DataLength = Drawing_1_LENGTH;
  ext_client_custom_graphic_single.txFrameHeader.Seq = 0;
  Append_CRC8_Check_Sum((unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single.txFrameHeader));
  ext_client_custom_graphic_single.Cmd_ID = ID_student_ui_;
  ext_client_custom_graphic_single.dataFrameHeader.data_cmd_id = Drawing_1_ID;
  ext_client_custom_graphic_single.dataFrameHeader.send_ID = robot_id;
  ext_client_custom_graphic_single.dataFrameHeader.receiver_ID = referee_get_receiver_ID(robot_id);
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[0]=dex;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[1]=0x0;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[2]=0x0;
  ext_client_custom_graphic_single.grapic_data_struct.operate_tpye = control_way;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_tpye = CIRCLE;
  ext_client_custom_graphic_single.grapic_data_struct.layer = 0;
  ext_client_custom_graphic_single.grapic_data_struct.color = color;
  ext_client_custom_graphic_single.grapic_data_struct.start_angle = 0;
  ext_client_custom_graphic_single.grapic_data_struct.end_angle = 0;
  ext_client_custom_graphic_single.grapic_data_struct.width = width;
  ext_client_custom_graphic_single.grapic_data_struct.start_x = x_centre;
  ext_client_custom_graphic_single.grapic_data_struct.start_y = y_centre;
  ext_client_custom_graphic_single.grapic_data_struct.radius = radius;
  ext_client_custom_graphic_single.grapic_data_struct.end_x = 0;
  ext_client_custom_graphic_single.grapic_data_struct.end_y = 0;
  Append_CRC16_Check_Sum((unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single));
  if(HAL_UART_Transmit(&huart_Referee,(unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single),200) != HAL_OK)
    {
      i++;
    }
//  HAL_Delay(40);
    osDelay(40);
}
void referee_draw_ellipse(uint8_t robot_id,uint8_t dex,uint8_t control_way,uint8_t color,uint16_t x_centre,uint16_t y_centre,uint16_t x_radius,uint16_t y_radius)
{
  ext_client_custom_graphic_single.txFrameHeader.SOF = START_ID;
  ext_client_custom_graphic_single.txFrameHeader.DataLength = Drawing_1_LENGTH;
  ext_client_custom_graphic_single.txFrameHeader.Seq = 0;
  Append_CRC8_Check_Sum((unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single.txFrameHeader));
  ext_client_custom_graphic_single.Cmd_ID = ID_student_ui_;
  ext_client_custom_graphic_single.dataFrameHeader.data_cmd_id = Drawing_1_ID;
  ext_client_custom_graphic_single.dataFrameHeader.send_ID = robot_id;
  ext_client_custom_graphic_single.dataFrameHeader.receiver_ID = referee_get_receiver_ID(robot_id);
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[0]=dex;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[1]=0x0;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[2]=0x0;
  ext_client_custom_graphic_single.grapic_data_struct.operate_tpye = control_way;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_tpye = ELLIPSE;
  ext_client_custom_graphic_single.grapic_data_struct.layer = 0;
  ext_client_custom_graphic_single.grapic_data_struct.color = color;
  ext_client_custom_graphic_single.grapic_data_struct.start_angle = 0;
  ext_client_custom_graphic_single.grapic_data_struct.end_angle = 0;
  ext_client_custom_graphic_single.grapic_data_struct.width = 1;
  ext_client_custom_graphic_single.grapic_data_struct.start_x = x_centre;
  ext_client_custom_graphic_single.grapic_data_struct.start_y = y_centre;
  ext_client_custom_graphic_single.grapic_data_struct.radius = 0;
  ext_client_custom_graphic_single.grapic_data_struct.end_x = x_radius;
  ext_client_custom_graphic_single.grapic_data_struct.end_y = y_radius;
  Append_CRC16_Check_Sum((unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single));
  if(HAL_UART_Transmit(&huart_Referee,(unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single),200) != HAL_OK)
    {
      i++;
    }
//  HAL_Delay(40);
    osDelay(40);
}

void referee_draw_arc(uint8_t robot_id,uint8_t dex,uint8_t control_way,uint8_t color,int16_t start_angle,int16_t end_angle,uint16_t x_centre,uint16_t y_centre,uint16_t x_radius,uint16_t y_radius)
{
  ext_client_custom_graphic_single.txFrameHeader.SOF = START_ID;
  ext_client_custom_graphic_single.txFrameHeader.DataLength = Drawing_1_LENGTH;
  ext_client_custom_graphic_single.txFrameHeader.Seq = 0;
  Append_CRC8_Check_Sum((unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single.txFrameHeader));
  ext_client_custom_graphic_single.Cmd_ID = ID_student_ui_;
  ext_client_custom_graphic_single.dataFrameHeader.data_cmd_id = Drawing_1_ID;
  ext_client_custom_graphic_single.dataFrameHeader.send_ID = robot_id;
  ext_client_custom_graphic_single.dataFrameHeader.receiver_ID = referee_get_receiver_ID(robot_id);
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[0]=dex;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[1]=0x0;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_name[2]=0x0;
  ext_client_custom_graphic_single.grapic_data_struct.operate_tpye = control_way;
  ext_client_custom_graphic_single.grapic_data_struct.graphic_tpye = ARC;
  ext_client_custom_graphic_single.grapic_data_struct.layer = 0;
  ext_client_custom_graphic_single.grapic_data_struct.color = color;
  ext_client_custom_graphic_single.grapic_data_struct.start_angle = start_angle;
  ext_client_custom_graphic_single.grapic_data_struct.end_angle = end_angle;
  ext_client_custom_graphic_single.grapic_data_struct.width = 1;
  ext_client_custom_graphic_single.grapic_data_struct.start_x = x_centre;
  ext_client_custom_graphic_single.grapic_data_struct.start_y = y_centre;
  ext_client_custom_graphic_single.grapic_data_struct.radius = 0;
  ext_client_custom_graphic_single.grapic_data_struct.end_x = x_radius;
  ext_client_custom_graphic_single.grapic_data_struct.end_y = y_radius;
  Append_CRC16_Check_Sum((unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single));
  if(HAL_UART_Transmit(&huart_Referee,(unsigned char*)&ext_client_custom_graphic_single,sizeof(ext_client_custom_graphic_single),200) != HAL_OK)
    {
      i++;
    }
//  HAL_Delay(40);
    osDelay(40);
}
