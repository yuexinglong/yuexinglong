#ifndef _Kalmanfilter_H
#define _Kalmanfilter_H


extern float acc_ratio;
extern float gyro_ratio;
extern float dt;



typedef struct
{
	float Wk;//调零参数
	float	Qk;//过程噪声协方差
	float	Rk;//观测噪声协方差
	float	Xk;//输出最优值
	float	K;//增益
	float	Pk;//估算协方差
}KFP;


float angle_calc_filter(float icm_acc, float icm_gyro);

void filter_init(KFP *kfp); //卡尔曼滤波参数初始化函数
float filter(KFP *kfp, float new_data);//卡尔曼滤波公式函数



#endif



