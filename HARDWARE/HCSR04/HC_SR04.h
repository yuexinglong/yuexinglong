#ifndef __HC_SR04_H
#define __HC_SR04_H	
#include "stm32f10x.h"
#include "sys.h"

#define Tr1g PCout(13)
#define Echo PCin(14)

#define HCSR05_PORT GPIOC
#define HCSR05_CLK RCC_APB2Periph_GPIOC
#define HCSR05_TRIG GPIO_Pin_13
#define HCSR05_ECHO GPIO_Pin_14

void Hcsr05Init(void);
void hcsr05_NVIC(void);
static void OpenTimerForHc(void);
static void CloseTimerForHc(void);
void TIM4_IRQHandler(void);
u32 GetEchoTimer(void);
float Hcsr05GetLength(void);

#endif 


