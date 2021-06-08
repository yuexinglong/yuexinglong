#include "exti.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序
void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTC时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;//PB11 -- DIR-L
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PC15 -- DIR-R
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Line=EXTI_Line11;	//PA11 -- INT-L
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//PB3 -- INT-R
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 

}

int DAT_L;
int DAT_R;


 //外部中断4服务程序 
void EXTI4_IRQHandler(void)
{
//	if(DIR_R == 0)
//	{
//		DAT_R++;
//	}
//	else
	{
		DAT_R--;
	}
	EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE4上的中断标志位 
}
//外部中断3服务程序 
void EXTI15_10_IRQHandler(void) 
{
//	if(DIR_L)
//	{
//		DAT_L++;
//	}
//	else
	{
		DAT_L--;
	}
	EXTI_ClearITPendingBit(EXTI_Line11); //清除LINE11上的中断标志位 
}
int exti_dat_L;
int exti_dat_R;

void read_dat_exti(void)
{
	exti_dat_R = DAT_R;
	exti_dat_L = DAT_L;
	DAT_R=0;
	DAT_L=0;
}


 
