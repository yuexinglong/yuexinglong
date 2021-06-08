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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

void TIM1_Int_Init(u16 arr,u16 psc)
{    
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(  //使能或者失能指定的TIM中断
	TIM1, //TIM5
	TIM_IT_Update ,
	ENABLE  //使能
	);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设

}
void MiniBalance_PWM_Init(u16 arr,u16 psc)
{		 
		GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);// 使能定时器1
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能
	                                                                     	

   //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80KHZ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx  通道1
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx  通道2
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx  通道3
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx  通道4

  TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1预装载使能	
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1预装载使能	
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1预装载使能	
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1预装载使能	 
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM1
 
} 





//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM3
		TIM_IT_Update ,
		ENABLE  //使能
		);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}




float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
float angle_filter;	//滤波后角度
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

void TIM3_IRQHandler(void)   //TIM3中断
{
	static int ON=0;
	static int cmt_100ms;
	int set_speed=30;
	int speed;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
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
			
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//		acc_x = filter(&angle_kfp, aacx); 
//		gyro_y = filter(&angle_kfp, gyroy);
		acc_x = aacx/10;
		gyro_y = gyroy/10;
		angle_filter = angle_calc_filter(acc_x,gyro_y);
		//speed = (exti_dat_L+exti_dat_R) / 2;
		motor_duty = motor_PID_Loc(0,angle_filter,&motor_Loc_PID);
		if(motor_duty>=0)
		{
			TIM_SetCompare1(TIM2, 0);										//通道1和2控制右边电机，通道2占空比为0时前进
			TIM_SetCompare2(TIM2, (int)motor_duty);
			TIM_SetCompare3(TIM2, 0);					//通道3和4控制左边电机，通道4占空比为0时前进
			TIM_SetCompare4(TIM2, (int)motor_duty);
		}
		else
		{
			TIM_SetCompare1(TIM2, -(int)motor_duty);										//通道1和2控制右边电机，通道2占空比为0时前进
			TIM_SetCompare2(TIM2, 0);
			TIM_SetCompare3(TIM2, -(int)motor_duty);					//通道3和4控制左边电机，通道4占空比为0时前进
			TIM_SetCompare4(TIM2, 0);
		}
//		motor_duty = motor_PID_Loc(mid_angle,angle_filter,&motor_Loc_PID);
//		if(MOTOR_ON_flag)
//		{
//			if(motor_duty<0)
//			{
//				TIM_SetCompare1(TIM2, 0);										//通道1和2控制右边电机，通道2占空比为0时前进
//				TIM_SetCompare2(TIM2, -motor_duty);
//				TIM_SetCompare3(TIM2, 0);					//通道3和4控制左边电机，通道4占空比为0时前进
//				TIM_SetCompare4(TIM2, -motor_duty);
//			}
//			else
//			{
//				TIM_SetCompare1(TIM2, motor_duty);										//通道1和2控制右边电机，通道2占空比为0时前进
//				TIM_SetCompare2(TIM2, 0);
//				TIM_SetCompare3(TIM2, motor_duty);					//通道3和4控制左边电机，通道4占空比为0时前进
//				TIM_SetCompare4(TIM2, 0);
//			}
//		}
//		else
//		{
//			TIM_SetCompare1(TIM2, 0);										//通道1和2控制左边电机，通道1占空比为0时前进
//			TIM_SetCompare2(TIM2, 0);
//			TIM_SetCompare3(TIM2, 0);					//通道3和4控制右边电机，通道4占空比为0时前进
//			TIM_SetCompare4(TIM2, 0);
//		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}

//void TIM1_IRQHandler(void)   //TIM3中断
//{
//	static int cnt_100ms;
//	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//	{
//			OLED_ShowNum(56,0,2,4,16);//显示角度机械中值
//		cnt_100ms++;
//		if(cnt_100ms>=25)
//		{
//			exti_dat_L=read_dat_L_exti();
//			exti_dat_R=read_dat_R_exti();
//			cnt_100ms=0;
//		}
//		
//		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
//	}
//}








