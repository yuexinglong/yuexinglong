#include "ec11.h"
#include "delay.h"

/**************************************************************************
函数功能：按键初始化函数
入口参数：无
返回  值：无
**************************************************************************/	 
void EC11_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTC时钟

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

} 
/**************************************************************************
函数功能：按键扫描函数，
入口参数：是否支持连按参数，1：支持，0：不支持
返回  值：无
**************************************************************************/	 
u8 EC11_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(EC11S==0|EC11A==0|EC11B==0))
	{
		key_up=0;
		if(EC11B==0)
		{
			if(EC11A==1)return EC11A_PRES;
		}
		else if(EC11A==0)
		{
			if(EC11B==1)return EC11B_PRES;
		}
		else if(EC11S==0)
		{
			delay_ms(10);//去抖动 
			if(EC11S==0)return EC11S_PRES;
		}
	}else if(EC11S==1&&EC11A==1&&EC11B ==1)key_up=1; 

	return 0;// 无按键按下
	
}
