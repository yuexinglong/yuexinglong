#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
extern float pitch,roll,yaw; 		//ŷ����
extern short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
extern short gyrox,gyroy,gyroz;	//������ԭʼ����
extern float angle_filter;	//�˲���Ƕ�
extern float acc_x,gyro_x;
extern float acc_y,gyro_y;
extern float acc_z,gyro_z;
extern float mid_angle;

void TIM1_Int_Init(u16 arr,u16 psc); 
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc); 
 
#endif
