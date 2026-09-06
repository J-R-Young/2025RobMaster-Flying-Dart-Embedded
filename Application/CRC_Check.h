#ifndef  CRC_CHECK_H_
#define  CRC_CHECK_H_

#include <stdint.h>

uint32_t cal_crc(uint32_t *ptr, int len);
unsigned char Get_CRC8_Check_Sum(unsigned char* pchMessage, unsigned int dwLength, unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char* pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char* pchMessage, unsigned int dwLength);
unsigned int Verify_CRC16_Check_Sum(unsigned char* pchMessage, unsigned int dwLength);
void Append_CRC16_Check_Sum(unsigned char* pchMessage, unsigned int dwLength);
unsigned short Get_CRC16_Check_Sum(unsigned char* pchMessage, unsigned int dwLength, unsigned short wCRC);
uint32_t Get_CRC32_Check_Sum(unsigned char* pchMessage, unsigned int dwLength, uint32_t wCRC);
unsigned int Verify_CRC32_Check_Sum(unsigned char* pchMessage, unsigned int dwLength);
void Append_CRC32_Check_Sum(unsigned char* pchMessage, unsigned int dwLength);

#endif
