#ifndef  __CRC_H
#define  __CRC_H

#include "headfiles.h"

//------------------------------供外部调用-------------------------------------
//四个函数的参数dwLength = data + crc(即数据长度加上crc的长度)
//自定义帧头 CRC8 校验
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
//检测一段数据后的crc是否正确
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
//在一段数据后加上crc8校验
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
//检测一段数据后的crc是否正确
unsigned int Verify_CRC16_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
//在一段数据后加上crc16校验
void Append_CRC16_Check_Sum(unsigned char * pchMessage,unsigned int dwLength);
//帧尾CRC16校验
unsigned short Get_CRC16_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned short wCRC);
//------------------------------------------------------------------------------

#endif


