//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PA5��SCL��
//              D1   ��PA7��SDA��
//              RES  ��PB0
//              DC   ��PB1
//              CS   ��PA4               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
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
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	BEEP_init();
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear(); 
	while(MPU_Init())
	{
		OLED_ShowString(16,0,"MPU INITING!");
	}
	
	EC11_Init();
	EXTIX_Init();
	TIM3_Int_Init(10,7199);//2ms��ʱ
//	Hcsr05Init();
	MiniBalance_PWM_Init(7199,0);
	uart3_init(115200);								//������ʼ��  
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
		OLED_ShowNum(40,0,i,3,16);//��ʾPID Kpֵ
		OLED_ShowString(16,1,"XF:");
		OLED_ShowNum(40,1,abs(exti_dat_R),3,16);//��ʾPID Kdֵ
		OLED_ShowString(16,2,"MA:");
		OLED_ShowNum(40,2,abs(exti_dat_L),3,16);//��ʾ�ǶȻ�е��ֵ
		OLED_ShowString(16,3,"Kp:");
		OLED_ShowNum(40,3,abs(exti_dat_R),3,16);//��ʾPID Kpֵ
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
					case 1://���
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
					case 3://�м���ȷ�ϼ�
					{
						if(setflag == 6)
						{OLED_ON=0;MOTOR_ON_flag=1;OLED_Clear();}
						else setting = ~setting;
					}break;
					case 2://�Ҽ�
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
		
//		OLED_ShowCHinese(0,0,0);//��
//		OLED_ShowCHinese(18,0,1);//��
//		OLED_ShowCHinese(36,0,2);//԰
//		OLED_ShowCHinese(54,0,3);//��
//		OLED_ShowCHinese(72,0,4);//��
//		OLED_ShowCHinese(90,0,5);//��
//		OLED_ShowCHinese(108,0,6);//��
		//printf("{plotter:%.2f,%.2f,%.2f,%.2f}\n", acc_z,gyro_y,angle_filter,motor_Loc_PID.LocSum );
		
	}
}




