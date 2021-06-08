#ifndef _Kalmanfilter_H
#define _Kalmanfilter_H


extern float acc_ratio;
extern float gyro_ratio;
extern float dt;



typedef struct
{
	float Wk;//�������
	float	Qk;//��������Э����
	float	Rk;//�۲�����Э����
	float	Xk;//�������ֵ
	float	K;//����
	float	Pk;//����Э����
}KFP;


float angle_calc_filter(float icm_acc, float icm_gyro);

void filter_init(KFP *kfp); //�������˲�������ʼ������
float filter(KFP *kfp, float new_data);//�������˲���ʽ����



#endif



