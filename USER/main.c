//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PA5（SCL）
//              D1   接PA7（SDA）
//              RES  接PB0
//              DC   接PB1
//              CS   接PA4               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/

#include "delay.h"
#include "sys.h"
#include "ec11.h"
#include "oled.h"
#include "led.h"
#include "PID.h"
#include "bmp.h"
#include "usart.h"		
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "timer.h"
#include "dmpmap.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "bluetooth.h"
#include "HC_SR04.h"
#include "exti.h"
#include "filter.h"


extern PID_LocTypeDef motor_Loc_PID;
int setting = 0;
int setflag = 0;
int OLED_ON = 0;
int MOTOR_ON_flag = 0;
float length_HC_SR04;
extern KFP angle_kfp;
extern float motor_duty;
#define MID_ANGLE_SET 0
#define KP_SET 2
#define KD_SET 4
#define MOTOR_ON 4


int main(void)
{
	u8 key=0;
	int i=3000,b=0;
	char buff[100];
	uart_init(115200);
	printf("OKOKOKOK\r\n");
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	BEEP_init();
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
	while(MPU_Init())
	{
		OLED_ShowString(16,0,"MPU INITING!");
	}
	
	EC11_Init();
	EXTIX_Init();
	TIM3_Int_Init(10,7199);//2ms定时
//	Hcsr05Init();
	MiniBalance_PWM_Init(7199,0);
	uart3_init(115200);								//蓝牙初始化  
	filter_init(&angle_kfp);
	

	
	motor_Loc_PID.Kp = 17;
//	motor_Loc_PID.Ki = 10  ;
	motor_Loc_PID.Kd = 0;
//	motor_Loc_PID.LocSum = 0;
	
	while(1)
	{
  
		sprintf(buff,"{plotter:%d,%d,%d}\r\n",key,DIR_L,DIR_R);
		//sprintf(buff,"{plotter:%.1f,%.1f,%.1f}\r\n",acc_x,gyro_y,angle_filter);
		MY_SEND_BUFF(buff);
		
		key = EC11_Scan(0);

		OLED_ShowString(16,0,"Ki:");
		OLED_ShowNum(40,0,i,3,16);//显示PID Kp值
		OLED_ShowString(16,1,"XF:");
		OLED_ShowNum(40,1,abs(exti_dat_R),3,16);//显示PID Kd值
		OLED_ShowString(16,2,"MA:");
		OLED_ShowNum(40,2,abs(exti_dat_L),3,16);//显示角度机械中值
		OLED_ShowString(16,3,"Kp:");
		OLED_ShowNum(40,3,abs(exti_dat_R),3,16);//显示PID Kp值
		OLED_ShowString(16,4,"Kd:");
			OLED_ShowString(16,4,"MOTOR ON");
			if(setting)
			{
				OLED_ShowString(0,setflag,">>");
			}
			else
			{
				OLED_ShowString(0,setflag,"<>");
			}
			if(key)
			{
				switch(key)
				{
					case 1://左键
					{
						if(setting)
						{
							switch(setflag)
							{
								case MID_ANGLE_SET:
								{
									mid_angle-=1;
								}break;
								case KP_SET:
								{
									//motor_Loc_PID.Kp-=1;
									motor_Loc_PID.Ki-=1;
								}break;
								case KD_SET:
								{
									//motor_Loc_PID.Kd-=1;
									xianfu -=5;
								}break;
								default:break;
							}
						}
						else
						{
							OLED_ShowString(0,setflag,"  ");
							setflag-=1;
							if(setflag<0)setflag = 6;
						}
					}break;
					case 3://中键，确认键
					{
						if(setflag == 6)
						{OLED_ON=0;MOTOR_ON_flag=1;OLED_Clear();}
						else setting = ~setting;
					}break;
					case 2://右键
					{
						if(setting)
						{
							switch(setflag)
							{
								case MID_ANGLE_SET:
								{
									mid_angle+=1;
								}break;
								case KP_SET:
								{
									//motor_Loc_PID.Kp+=1;
									motor_Loc_PID.Ki+=1;
								}break;
								case KD_SET:
								{
									//motor_Loc_PID.Kd+=1;
									xianfu +=5;
								}break;
								default:break;
							}
						}
						else
						{
							OLED_ShowString(0,setflag,"  ");
							setflag+=1;
							if(setflag>6)setflag = 0;
						}
					}break;
					default:break;
				}
			}
//		else
//		{
//			key = EC11_Scan(0);
//			if(key)
//			{
//				OLED_ON = 1;
//				MOTOR_ON_flag = 0;
//				OLED_Clear(); 
//			}
//		}
		
//		OLED_ShowCHinese(0,0,0);//中
//		OLED_ShowCHinese(18,0,1);//景
//		OLED_ShowCHinese(36,0,2);//园
//		OLED_ShowCHinese(54,0,3);//电
//		OLED_ShowCHinese(72,0,4);//子
//		OLED_ShowCHinese(90,0,5);//科
//		OLED_ShowCHinese(108,0,6);//技
		//printf("{plotter:%.2f,%.2f,%.2f,%.2f}\n", acc_z,gyro_y,angle_filter,motor_Loc_PID.LocSum );
		
	}
}




