#include "timer.h"
#include "led.h"
#include "PID.h"
#include "filter.h"
#include "mpu6050.h"
#include "stm32f10x_tim.h"
#include "exti.h"
#include "HC_SR04.h"
#include "oled.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

void TIM1_Int_Init(u16 arr,u16 psc)
{    
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
	TIM1, //TIM5
	TIM_IT_Update ,
	ENABLE  //ʹ��
	);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx����

}
void MiniBalance_PWM_Init(u16 arr,u16 psc)
{		 
		GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);// ʹ�ܶ�ʱ��1
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	                                                                     	

   //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80KHZ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx  ͨ��1
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx  ͨ��2
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx  ͨ��3
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx  ͨ��4

  TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM1
 
} 





//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM3
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}




float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
float angle_filter;	//�˲���Ƕ�
float acc_x,gyro_x;
float acc_y,gyro_y;
float acc_z,gyro_z;
float mid_angle = 303;
float motor_duty;
extern float length_HC_SR04;
extern PID_LocTypeDef motor_Loc_PID;
extern PID_IncTypeDef motor_Inc_PID;
extern int MOTOR_ON_flag;
extern KFP angle_kfp;

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	static int ON=0;
	static int cmt_100ms;
	int set_speed=30;
	int speed;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		cmt_100ms++;
		if(cmt_100ms>=25)
		{
			read_dat_exti();
			cmt_100ms=0;
		}
		//LED = ~LED;
//		if(ON)
//		{
//			TIM_SetCompare1(TIM2, 7200);
//			TIM_SetCompare2(TIM2, 0);
//			motor_duty++;
//			if(motor_duty>=7200)ON=0;
//			if(motor_duty>=0)
//			{
//				TIM_SetCompare1(TIM2, 0);
//				TIM_SetCompare2(TIM2, motor_duty);
//			}
//			else
//			{
//				TIM_SetCompare1(TIM2, -motor_duty);
//				TIM_SetCompare2(TIM2, 0);
//			}
//		}
//		else
//		{
//			motor_duty--;
//			if(motor_duty<=-7200)ON=1;
//			if(motor_duty>=0)
//			{
//				TIM_SetCompare1(TIM2, 0);
//				TIM_SetCompare2(TIM2, motor_duty);
//			}
//			else
//			{
//				TIM_SetCompare1(TIM2, -motor_duty);
//				TIM_SetCompare2(TIM2, 0);
//			}
//			
//		}
			
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
//		acc_x = filter(&angle_kfp, aacx); 
//		gyro_y = filter(&angle_kfp, gyroy);
		acc_x = aacx/10;
		gyro_y = gyroy/10;
		angle_filter = angle_calc_filter(acc_x,gyro_y);
		//speed = (exti_dat_L+exti_dat_R) / 2;
		motor_duty = motor_PID_Loc(0,angle_filter,&motor_Loc_PID);
		if(motor_duty>=0)
		{
			TIM_SetCompare1(TIM2, 0);										//ͨ��1��2�����ұߵ����ͨ��2ռ�ձ�Ϊ0ʱǰ��
			TIM_SetCompare2(TIM2, (int)motor_duty);
			TIM_SetCompare3(TIM2, 0);					//ͨ��3��4������ߵ����ͨ��4ռ�ձ�Ϊ0ʱǰ��
			TIM_SetCompare4(TIM2, (int)motor_duty);
		}
		else
		{
			TIM_SetCompare1(TIM2, -(int)motor_duty);										//ͨ��1��2�����ұߵ����ͨ��2ռ�ձ�Ϊ0ʱǰ��
			TIM_SetCompare2(TIM2, 0);
			TIM_SetCompare3(TIM2, -(int)motor_duty);					//ͨ��3��4������ߵ����ͨ��4ռ�ձ�Ϊ0ʱǰ��
			TIM_SetCompare4(TIM2, 0);
		}
//		motor_duty = motor_PID_Loc(mid_angle,angle_filter,&motor_Loc_PID);
//		if(MOTOR_ON_flag)
//		{
//			if(motor_duty<0)
//			{
//				TIM_SetCompare1(TIM2, 0);										//ͨ��1��2�����ұߵ����ͨ��2ռ�ձ�Ϊ0ʱǰ��
//				TIM_SetCompare2(TIM2, -motor_duty);
//				TIM_SetCompare3(TIM2, 0);					//ͨ��3��4������ߵ����ͨ��4ռ�ձ�Ϊ0ʱǰ��
//				TIM_SetCompare4(TIM2, -motor_duty);
//			}
//			else
//			{
//				TIM_SetCompare1(TIM2, motor_duty);										//ͨ��1��2�����ұߵ����ͨ��2ռ�ձ�Ϊ0ʱǰ��
//				TIM_SetCompare2(TIM2, 0);
//				TIM_SetCompare3(TIM2, motor_duty);					//ͨ��3��4������ߵ����ͨ��4ռ�ձ�Ϊ0ʱǰ��
//				TIM_SetCompare4(TIM2, 0);
//			}
//		}
//		else
//		{
//			TIM_SetCompare1(TIM2, 0);										//ͨ��1��2������ߵ����ͨ��1ռ�ձ�Ϊ0ʱǰ��
//			TIM_SetCompare2(TIM2, 0);
//			TIM_SetCompare3(TIM2, 0);					//ͨ��3��4�����ұߵ����ͨ��4ռ�ձ�Ϊ0ʱǰ��
//			TIM_SetCompare4(TIM2, 0);
//		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}

//void TIM1_IRQHandler(void)   //TIM3�ж�
//{
//	static int cnt_100ms;
//	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
//	{
//			OLED_ShowNum(56,0,2,4,16);//��ʾ�ǶȻ�е��ֵ
//		cnt_100ms++;
//		if(cnt_100ms>=25)
//		{
//			exti_dat_L=read_dat_L_exti();
//			exti_dat_R=read_dat_R_exti();
//			cnt_100ms=0;
//		}
//		
//		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//	}
//}








