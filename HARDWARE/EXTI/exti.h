#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
#include "stm32f10x_exti.h"
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

#define DIR_L PBin(12)
#define DIR_R PCin(15)

extern int exti_dat_L;
extern int exti_dat_R;


void EXTIX_Init(void);//�ⲿ�жϳ�ʼ��		 	
void read_dat_exti(void);	
#endif

