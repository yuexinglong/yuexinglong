#include "HC_SR04.h"
#include "misc.h"
#include "delay.h"
#include "stm32f10x_tim.h"


u16 msHcCount;

void Hcsr05Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HCSR05_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =HCSR05_TRIG;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(HCSR05_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(HCSR05_PORT,HCSR05_TRIG);
	 
	GPIO_InitStructure.GPIO_Pin =   HCSR05_ECHO;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(HCSR05_PORT, &GPIO_InitStructure);  
	GPIO_ResetBits(HCSR05_PORT,HCSR05_ECHO);    
	 
				
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   
	 
	TIM_DeInit(TIM4);  //复位
	TIM_TimeBaseStructure.TIM_Period = (1000-1); 
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1);   //1MS进入一次中断
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);          
			
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);  
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);    
	hcsr05_NVIC();
	TIM_Cmd(TIM4,DISABLE);    
}

void hcsr05_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       
	NVIC_Init(&NVIC_InitStructure);
}

//打开定时器
static void OpenTimerForHc(void)
{
	TIM_SetCounter(TIM4,0);
	msHcCount = 0;
	TIM_Cmd(TIM4, ENABLE);
}

//关闭定时器
static void CloseTimerForHc(void)
{
	TIM_Cmd(TIM4, DISABLE);
}

//定时器中断函数
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update );
		msHcCount++;
	}
}

//获取定时器计数值
u32 GetEchoTimer(void)
{
	u32 t = 0;
	t = msHcCount*1000;
	t += TIM_GetCounter(TIM4);
	 TIM4->CNT = 0;
	delay_ms(50);
	return t;
}
//通过定时器计数的值计算距离
float Hcsr05GetLength(void)
{
	u32 t = 0;
	float lengthTemp = 0;
	
	GPIO_SetBits(HCSR05_PORT,HCSR05_TRIG); //拉高TRIG脚 发送超过10us的高电平
	delay_us(20);
	GPIO_ResetBits(HCSR05_PORT,HCSR05_TRIG); //拉低TRIG
	
	while(GPIO_ReadInputDataBit(HCSR05_PORT,HCSR05_ECHO) == 0);  //等待ECHO脚高电平 跳出while循环
	OpenTimerForHc();         //打开定时器
	
	while(GPIO_ReadInputDataBit(HCSR05_PORT,HCSR05_ECHO) == 1){} //等待ECHO脚高电平 跳出while循环
	CloseTimerForHc();         //关闭定时器
		
	t = GetEchoTimer();        //获取定时器计数的值
	lengthTemp = ((float)t/58.0);//cm
		
	return lengthTemp;
}



