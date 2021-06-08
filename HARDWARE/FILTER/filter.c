#include "filter.h"

float acc_ratio = 3.0; 
float gyro_ratio = -16;
float dt = 0.002;

KFP angle_kfp;

float angle_calc_filter(float icm_acc, float icm_gyro)
{
	float temp;
	float gyro_now;
	float err_angle;
	
	static float last_angle;
	static int first_angle = 1;
	
	if(first_angle)
	{
		first_angle = 0;
		last_angle = icm_acc;
	}
	gyro_now = icm_gyro * gyro_ratio;
	
	err_angle = (icm_acc - last_angle) * acc_ratio;
	
	temp = last_angle + (err_angle + gyro_now) * dt;
	
	last_angle = temp;
	
	return temp;
	
}

void filter_init(KFP *kfp)
{
	kfp->Qk = 0.001;//过程噪声协方差
	kfp->Rk = 1.93;//观测噪声协方差
	kfp->Xk = 0;//输出最优值
	kfp->K = 0;//增益
	kfp->Pk = 0.02;//估算协方差 初始值不可为零！
	
}

float filter(KFP *kfp, float new_data)
{
	
	kfp->Xk = kfp->Xk;
	
	kfp->Pk = kfp->Pk + kfp->Qk;
	
	kfp->K = kfp->Pk / (kfp->Pk + kfp->Rk);
	
	
	kfp->Xk = kfp->Xk + kfp->K*(new_data - kfp->Xk);

	kfp->Pk = (1 - kfp->K)*kfp->Pk;
	
	return kfp->Xk;
}


