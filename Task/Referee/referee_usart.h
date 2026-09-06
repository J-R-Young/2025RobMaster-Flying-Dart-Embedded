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
#ifndef __Referee_Uart_h
#define __Referee_Uart_h

#include "headfiles.h"

/* =========== 宏定义 ================ */
#define REFEREE_RX_BUF_NUM  512

//UI是否刷新
#define YES_ 1
#define NO_  0

/* =========== 全局变量声明 ========== */
/* 自定义帧头 */
typedef struct __attribute__((packed))
{
	uint8_t  SOF;
	uint16_t DataLength;
	uint8_t  Seq;
	uint8_t  CRC8;
  //uint16_t Cmd_ID;
}xFrameHeader;

/* =========== 函数声明 ============== */
void referee_usart_init(void);
void RE_IRQHandler(void);
void RE_usart_tx_dma_enable(uint8_t *data, uint16_t len);

#endif /* __Referee_Uart_h */


