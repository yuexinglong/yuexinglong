#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
extern float pitch,roll,yaw; 		//欧拉角
extern short aacx,aacy,aacz;		//加速度传感器原始数据
extern short gyrox,gyroy,gyroz;	//陀螺仪原始数据
extern float angle_filter;	//滤波后角度
extern float acc_x,gyro_x;
extern float acc_y,gyro_y;
extern float acc_z,gyro_z;
extern float mid_angle;

void TIM1_Int_Init(u16 arr,u16 psc); 
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc); 
 
#endif
