#include "Fun.h"

double Rad_Remap(double Raw)
{
    double Turns;
    double tmp;
    
    Turns = floor(Raw / (2.0 * PI));
    
    tmp = Raw - Turns * (2.0 * PI);
    
    // 过零处理
    tmp -= (tmp > PI) ? (2.0 * PI) : 0;
    tmp += (tmp < -PI) ? (2.0 * PI) : 0;
		
    return tmp;
}


double GetRealErr_RemapRad(double Target, double Current)
{
    double TmpErr = Target - Current;
    
    TmpErr = ABS(TmpErr) > PI ? \
            (TmpErr > 0 ? TmpErr - 2.0 * PI : TmpErr + 2.0 * PI) : \
            TmpErr;
    
    return TmpErr;
}

/**
  * @name   PWM_SetDuty()
  * @brief  设置PWM通道的占空比
  * @call   External
  * @param  *Channel    PWM通道对应的比较寄存器地址
  * @param  Duty        PWM通道要设置的占空比
  * @RetVal NULL
  */
void PWM_SetDuty(uint32_t *Channel, int32_t Duty)
{
    Duty = abs(Duty);
    
    *Channel = Duty;
}/* PWM_SetDuty() */





