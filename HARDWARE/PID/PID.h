#ifndef _STEERPID_H
#define _STEERPID_H

extern	float xianfu;

typedef struct
{
  float Kp;                       //比例系数Proportional
  float Ki;                       //积分系数Integral
  float Kd;                       //微分系数Derivative
 
  float Ek;                       //当前误差
  float Ek1;                      //前一次误差 e(k-1)
  float Ek2;                      //再前一次误差 e(k-2)
  float LocSum;                   //累计积分位置
}PID_LocTypeDef;
 

typedef struct
{
  float Kp;                       //比例系数Proportional
  float Ki;                       //积分系数Integral
  float Kd;                       //微分系数Derivative
 
  float Ek;                       //当前误差
  float Ek1;                      //前一次误差 e(k-1)
  float Ek2;                      //再前一次误差 e(k-2)
}PID_IncTypeDef;

float motor_PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID);
float motor_PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID);
float modu(float floats);


#endif

