#include "exti.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTCʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;//PB11 -- DIR-L
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PC15 -- DIR-R
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Line=EXTI_Line11;	//PA11 -- INT-L
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//PB3 -- INT-R
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 

}

int DAT_L;
int DAT_R;


 //�ⲿ�ж�4������� 
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
	EXTI_ClearITPendingBit(EXTI_Line4); //���LINE4�ϵ��жϱ�־λ 
}
//�ⲿ�ж�3������� 
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
	EXTI_ClearITPendingBit(EXTI_Line11); //���LINE11�ϵ��жϱ�־λ 
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


 
