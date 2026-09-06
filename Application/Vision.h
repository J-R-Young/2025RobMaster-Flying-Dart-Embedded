#ifndef __Vision_h
#define __Vision_h

#include "headfiles.h"


#define VISION_FRAME_LENGTH_RX      (sizeof(VisionFrameRX_structTypedef))
#define VISION_BUFFER_LENGTH_RX     (VISION_FRAME_LENGTH_RX+ 5)
#define VISION_FRAME_LENGTH_TX      (sizeof(VisionFrameTX_structTypedef))
#define VISION_BUFFER_LENGTH_TX     (VISION_FRAME_LENGTH_TX)




typedef enum
{
    VisionMode_None         = 0,    // 无
    VisionMode_AutoAim      = 1,    // 自瞄模式
    VisionMode_NormalBuff   = 2,    // 小符模式
    VisionMode_HugeBuff     = 3,    // 大符模式
    VisionMode_Spin         = 4,    // 反小陀螺模式
    VisionMode_Tower        = 5,    // 前哨战模式
	VisionMode_Base			= 6,	// 基地模式
}VisionMode_enum;

/* ==== 云台发送帧结构体&联合体 ==== */
typedef struct __packed
{   
    float       Pitch;              // 云台Pitch轴
    float       Yaw;                // 云台Yaw轴
    uint8_t     RobotID;            // 机器人ID
    uint8_t     VisionMode;         // 视觉模式
    uint8_t     Exposure;           // 预留调节曝光的一个字节
    float       TimeStamp;          // 秒
}VisionFrameTX_structTypedef;   

typedef union
{
    VisionFrameTX_structTypedef     VisionFrameTX;
    uint8_t                         u8arr[sizeof(VisionFrameTX_structTypedef)];
}VisionFrameTX_unionTypeDef;

/* ==== 云台接收帧结构体&联合体 ==== */
typedef struct __packed
{   
    uint8_t     IdentifyFlag;         // 识别状态
	float 		x_pixel_err;		  // 图像X轴像素偏差
	float 		x_distance_err;		  // 图像X轴像素偏差
	uint8_t		tail;				  // 帧尾
}VisionFrameRX_structTypedef;   


typedef union
{
    VisionFrameRX_structTypedef     VisionFrameRX;
    uint8_t                         u8arr[sizeof(VisionFrameRX_structTypedef)];
}VisionFrameRX_unionTypeDef;





//typedef struct __packed
//{
//    float           PitchErr;      // 俯仰偏差
//    float           YawErr;        // 滚转偏差
//    float           Distance;   // 距离
//}VisionData_TypeDef;

typedef struct
{
	uint8_t* Start_ADDR;
    uint8_t* End_ADDR;	
}VisionData_ADDR;

typedef union
{
	float f;
	unsigned char c[4];
} float2uchar;

extern VisionFrameRX_structTypedef  VisionData;
extern VisionFrameTX_unionTypeDef TXFrame;
void Vision_Init(void);
void Vision_Upload(void);
void Vision_Clear(void);
void VisionRx_callback(void);

#endif /* __Vision_h */

