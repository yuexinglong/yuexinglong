#include "PID.h"

extern float gyro_y;				//�����Ƕ��õ�ԭʼֵ
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
�������� �� motor_PID_Loc
��    �� �� PIDλ��(Location)����
��    �� �� SetValue ------ ����ֵ(����ֵ)
            ActualValue --- ʵ��ֵ(����ֵ)
            PID ----------- PID���ݽṹ
�� �� ֵ �� PIDLoc -------- PIDλ��
��    �� �� strongerHuang
*************************************************/
float motor_PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID)
{
  float PIDLoc;//,index=1;                                  //λ��
 
  PID->Ek = SetValue - ActualValue;
//	if(modu(PID->Ek)>80)
//	{
//		index = 0;
//	}
//	else
//	{
//		index = 1;
//		PID->LocSum += PID->Ek;
//	}                       //�ۼ����
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
�������� �� motor_PID_Inc
��    �� �� PID����(Increment)����
��    �� �� SetValue ------ ����ֵ(����ֵ)
            ActualValue --- ʵ��ֵ(����ֵ)
            PID ----------- PID���ݽṹ
�� �� ֵ �� PIDInc -------- ����PID����(+/-)
��    �� �� strongerHuang
*************************************************/
float motor_PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID)
{
  float PIDInc;                                  //����
 
  PID->Ek = SetValue - ActualValue;
  PIDInc = (PID->Kp * PID->Ek) + (PID->Ki * PID->Ek1);// + (PID->Kd * PID->Ek2);
 
  PID->Ek2 = PID->Ek1;
  PID->Ek1 = PID->Ek;  return PIDInc;
}


/************************************************
�������� �� motor_PID_Loc
��    �� �� PIDλ��(Location)����
��    �� �� SetValue ------ ����ֵ(����ֵ)
            ActualValue --- ʵ��ֵ(����ֵ)
            PID ----------- PID���ݽṹ
�� �� ֵ �� PIDLoc -------- PIDλ��
��    �� �� strongerHuang
*************************************************/
//float motor_PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID)
//{
//  float PIDLoc;                                  //λ��
// 
//  PID->Ek = SetValue - ActualValue;
//  PID->LocSum += PID->Ek;                         //�ۼ����
//	if(PID->LocSum>=70)PID->LocSum=70;
//	else if(PID->LocSum<=-70)PID->LocSum=-70;
// 
//	PIDLoc = PID->Kp * PID->Ek  + PID->Kd * (PID->Ek1 - PID->Ek); //+ (PID->Ki * PID->LocSum)
// 
//  PID->Ek1 = PID->Ek;  
//	return PIDLoc;
//}




