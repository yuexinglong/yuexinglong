#include "ec11.h"
#include "delay.h"

/**************************************************************************
�������ܣ�������ʼ������
��ڲ�������
����  ֵ����
**************************************************************************/	 
void EC11_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTCʱ��

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

} 
/**************************************************************************
�������ܣ�����ɨ�躯����
��ڲ������Ƿ�֧������������1��֧�֣�0����֧��
����  ֵ����
**************************************************************************/	 
u8 EC11_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
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
			delay_ms(10);//ȥ���� 
			if(EC11S==0)return EC11S_PRES;
		}
	}else if(EC11S==1&&EC11A==1&&EC11B ==1)key_up=1; 

	return 0;// �ް�������
	
}
