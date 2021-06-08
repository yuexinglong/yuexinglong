#include "PID.h"

extern float gyro_y;				//陀螺仪读得的原始值
float xianfu=300;
PID_LocTypeDef motor_Loc_PID;
PID_IncTypeDef motor_Inc_PID;


float modu(float floats)
{
	float temp;
	if(floats<0) temp = -floats;
	else temp = floats;
	return temp;
}

/************************************************
函数名称 ： motor_PID_Loc
功    能 ： PID位置(Location)计算
参    数 ： SetValue ------ 设置值(期望值)
            ActualValue --- 实际值(反馈值)
            PID ----------- PID数据结构
返 回 值 ： PIDLoc -------- PID位置
作    者 ： strongerHuang
*************************************************/
float motor_PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID)
{
  float PIDLoc;//,index=1;                                  //位置
 
  PID->Ek = SetValue - ActualValue;
//	if(modu(PID->Ek)>80)
//	{
//		index = 0;
//	}
//	else
//	{
//		index = 1;
//		PID->LocSum += PID->Ek;
//	}                       //累计误差
//	PID->LocSum += PID->Ek;
//	if(PID->LocSum>=xianfu)PID->LocSum=xianfu;
//	else if(PID->LocSum<=-xianfu)PID->LocSum=-xianfu;
 
	PIDLoc = PID->Kp * PID->Ek  + PID->Kd * gyro_y;//+ index * PID->Ki * PID->LocSum
	//PIDLoc = PID->Kp * PID->Ek + index * PID->Ki * PID->LocSum + PID->Kd * (PID->Ek1 - PID->Ek);
  //PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->LocSum) + PID->Kd * (PID->Ek1 - PID->Ek);
 
  PID->Ek1 = PID->Ek;  
	if(PIDLoc>=7200)PIDLoc=7200;
	else if(PIDLoc<=-7200)PIDLoc=-7200;
	return PIDLoc;
}

/************************************************
函数名称 ： motor_PID_Inc
功    能 ： PID增量(Increment)计算
参    数 ： SetValue ------ 设置值(期望值)
            ActualValue --- 实际值(反馈值)
            PID ----------- PID数据结构
返 回 值 ： PIDInc -------- 本次PID增量(+/-)
作    者 ： strongerHuang
*************************************************/
float motor_PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID)
{
  float PIDInc;                                  //增量
 
  PID->Ek = SetValue - ActualValue;
  PIDInc = (PID->Kp * PID->Ek) + (PID->Ki * PID->Ek1);// + (PID->Kd * PID->Ek2);
 
  PID->Ek2 = PID->Ek1;
  PID->Ek1 = PID->Ek;  return PIDInc;
}


/************************************************
函数名称 ： motor_PID_Loc
功    能 ： PID位置(Location)计算
参    数 ： SetValue ------ 设置值(期望值)
            ActualValue --- 实际值(反馈值)
            PID ----------- PID数据结构
返 回 值 ： PIDLoc -------- PID位置
作    者 ： strongerHuang
*************************************************/
//float motor_PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID)
//{
//  float PIDLoc;                                  //位置
// 
//  PID->Ek = SetValue - ActualValue;
//  PID->LocSum += PID->Ek;                         //累计误差
//	if(PID->LocSum>=70)PID->LocSum=70;
//	else if(PID->LocSum<=-70)PID->LocSum=-70;
// 
//	PIDLoc = PID->Kp * PID->Ek  + PID->Kd * (PID->Ek1 - PID->Ek); //+ (PID->Ki * PID->LocSum)
// 
//  PID->Ek1 = PID->Ek;  
//	return PIDLoc;
//}




