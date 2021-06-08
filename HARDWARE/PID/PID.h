#ifndef _STEERPID_H
#define _STEERPID_H

extern	float xianfu;

typedef struct
{
  float Kp;                       //����ϵ��Proportional
  float Ki;                       //����ϵ��Integral
  float Kd;                       //΢��ϵ��Derivative
 
  float Ek;                       //��ǰ���
  float Ek1;                      //ǰһ����� e(k-1)
  float Ek2;                      //��ǰһ����� e(k-2)
  float LocSum;                   //�ۼƻ���λ��
}PID_LocTypeDef;
 

typedef struct
{
  float Kp;                       //����ϵ��Proportional
  float Ki;                       //����ϵ��Integral
  float Kd;                       //΢��ϵ��Derivative
 
  float Ek;                       //��ǰ���
  float Ek1;                      //ǰһ����� e(k-1)
  float Ek2;                      //��ǰһ����� e(k-2)
}PID_IncTypeDef;

float motor_PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID);
float motor_PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID);
float modu(float floats);


#endif

