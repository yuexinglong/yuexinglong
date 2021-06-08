#ifndef __EC11_H
#define __EC11_H	 
#include "sys.h"


#define EC11A   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//读取按键1
#define EC11B   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//读取按键1
#define EC11S  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//读取按键2 
 
 
#define EC11A_PRES	1
#define EC11B_PRES	2
#define EC11S_PRES	3

void EC11_Init(void);//IO初始化
u8 EC11_Scan(u8 mode);  	//按键扫描函数					    
#endif
