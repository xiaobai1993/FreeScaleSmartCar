
#include"sci.h"
#include"pwm.h"
#include"ccd.h"
#include"def.h"
#include"control.h"
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include<string.h>
#include<math.h>
#include<stdlib.h>

extern uint M_PACNT;
extern unsigned char Pixel[128];
extern unsigned char PixelAverageValue; /* 128个像素点的平均AD值 */
extern unsigned char PixelAverageVoltage; /* 128个像素点的平均电压值的10倍 */
extern int TargetPixelAverageVoltage;  /* 设定目标平均电压值，实际电压的10倍 */
extern int PixelAverageVoltageError ;/* 设定目标平均电压值与实际值的偏差，实际电压的10倍 */
extern int TargetPixelAverageVoltageAllowError ;  /* 设定目标平均电压值允许的偏差，实际电压的10倍 */
extern unsigned char IntegrationTime ; /* 曝光时间，单位ms */

extern char g_inter_flag;//中断标志
float average(int n,float *x);
float spfh(int n,float *x) ;
float shpf(int n,float *x);
float dcj(int n,float *x,float *y);
float djc(int n,float *x,float *y) ;
float xsa(int n,float *x,float *y) ;
float he(int n,float *y);
float xsb(int n,float *x,float *y,float a) ;
void update(float *f,int n,float input);
void bubble_sorts(unsigned char  a[],int start,int end);
/*extern char g_speed_read_count;
extern char g_speed_control_count;
extern char g_speed_control_flag;

extern float g_speed_pid_this;
extern float g_speed_pid_last;

extern float g_speed_out;

extern float g_speed_p;
extern float g_speed_i;
extern float g_speed_d;

extern float speed_sum_max,speed_sum_min;//误差积分的临界
extern float speed_out_max,speed_out_min;//pid的pwm输出的最大最小边界

extern float g_speed_err;
extern float g_speed_err_d;
extern float g_speed_err_pre;

extern float g_speed_new;
extern float g_speed_old ;
extern float g_speed_set;

extern float g_speed_control_period;
extern float g_speed_err_sum;*/
/************************************************************/
//舵机
extern int b_left,b_right;//黑线的左右边界
extern int b_left_count;
extern int b_right_count;
extern int b_right_last;
extern int b_left_last;
extern int b_left_flag,b_right_flag;
extern int b_left_last_flag;
extern int b_right_last_flag;
/*extern char guodu,b_width;
extern char yuzhi;
extern char stop_flag;*/
//extern char b_average;//整条黑线检测的平均值
/*extern char s_yuzhi;
extern int g_line_old;
extern int g_line_new;
extern char g_steer_control_flag;
extern char g_steer_control_count;
 
extern float g_steer_err_old;	
extern float g_steer_err_new;

extern float d_steer_err;///////
extern float steer_control_D;
extern float steer_control_P;

extern float steer_out_old;
extern float steer_out_new;

extern float steer_control_out_max;
extern float steer_control_out_min;

extern float steer_control_out;//计数

extern float g_steer_control_period; //周期  */