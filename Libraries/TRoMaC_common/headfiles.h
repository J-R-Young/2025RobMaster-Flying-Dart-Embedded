#ifndef __headfiles_h
#define __headfiles_h

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_conf.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "cmsis_os.h"
#include "task.h"
#include "freertos.h"

#include "can.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "main.h"
#include "rtc.h"

/* ============ common ============= */
#include "pid.h"
#include "Fun.h"
#include "DeviceCheck.h"
#include "OLEDFun.h"


/* ============ device ============= */
#include "Motor.h"
// RC
#include "rc_SBUS.h"
#include "monitoring_key.h"
// IMU
#include "bsp_iic.h"

/* ============ task =============== */
// µ×ÅÌ
#include "ChassisTask.h"

#include "Vision.h"
#include "Auto_Load.h"

#include "Dart.h"

// µ÷ÊÔ´òÓ¡
#include "debugTask.h"
// ²ÃÅÐ
#include "Referee.h"
#include "referee_usart.h"
#include "crc.h"



/* ============ application ======== */
#include "Ctrl.h"

#endif /* __headfiles_h */

