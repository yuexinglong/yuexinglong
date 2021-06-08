#include "bluetooth.h"
#include "stm32f10x_usart.h"
#include "delay.h"

/**************************************************************************
函数功能：串口3初始化
入口参数： bound:波特率
返回  值：无
**************************************************************************/
void uart3_init(u32 bound)
{  	 
	  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能UGPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能UGPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART3_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  //USART3_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure);     //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口3 
}



void MY_SEND_BUFF(char *buff)
{
	int i=0;
		while(buff[i]!='\0')
		{
			delay_us(100);
			USART_SendData(USART3,buff[i]);
			i++;
		}
	
}








/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
u8 ERR = 'E';

void USART3_IRQHandler(void)
{	
//	static	char uart_receive=0;//蓝牙接收相关变量

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	  
//  	 uart_receive=USART_ReceiveData(USART3); 

//	   switch (uart_receive)
//		 {
//			 case  '1':
//			 {
//				 LED = ~LED;				//反转小灯
//			 }break;
//		   case  '2': 
//			 {
//				LED = ~LED;				//反转小灯
//				 USART_SendData(USART3,'2');
//			 }break;
//			 case  '3':
//			 {
//				LED = ~LED;				//反转小灯
//				 USART_SendData(USART3,'3');
//			 }break;
//			 case  '4':	
//			 {
//				LED = ~LED;				//反转小灯
//				 USART_SendData(USART3,'4');
//			 }break;
//			 case  '5': 
//			 {
//				 USART_SendData(USART3,'5');
//			 }break;
//			 default: 
//			 {
//				//LED = ~LED;				//反转小灯
//				//USART_SendData(USART3,ERR);
//			 }break;
//		 }
	}  											 
} 



