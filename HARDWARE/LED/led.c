#include <stm32f10x.h>
#include "led.h"


void LED_init(void)
{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 
////=============================================================================
////LED -> PC13
////=============================================================================			 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 						 
//=============================================================================
//LED -> PC13
//=============================================================================			 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void BEEP_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 						 		 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}
