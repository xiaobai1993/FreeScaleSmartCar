#include"all.h"

 //阈值  17
/*****************************************************/

 int std_yuzhi=17;
 int start=0;
 char start_if=0;
 /********************************************************/
    
unsigned char Pixel[128];
extern char str_send[12];
extern char str_rec[12];

#define STEER_MID 1950
unsigned char TimerFlag20ms=0; 
extern uint	M_PACNT;
char TimerCnt20ms=0;
char integration_piont;
char g_inter_flag;//中断标志
void PIT_Init(void)
{
    PITCE_PCE0 = 1;             
    PITMUX_PMUX0 = 0;           
    PITMTLD0 = 199;            
    PITLD0 = 199;              
    PITINTE_PINTE0 = 1;         
    PITCFLMT_PITE = 1;         
}

float steer_control_D=65;//42;    
float steer_control_P=24.4;//29;//29.9 27.9;//33.9;

int steer_control_out_max=2550; //2550
int steer_control_out_min=1350;//1350;
int g_line_new=Middle;

int g_line_old=Middle;
float steer_out_this=STEER_MID;//中间位置
float steer_out_last=STEER_MID;
float g_steer_err_cur=0;
float g_steer_err_pre=0;
float g_steer_err_last=0;

void steer_pd(void) 
{
  

      g_line_new=(int)Calculate_line();
      //if(b_left_last_flag==0||b_right_last_flag==0)
        
        if(b_left_flag==0&&b_right_flag==0) 
        {
            if(g_steer_err_pre>40||g_steer_err_pre<-40) 
            {
               g_line_new=g_line_old;
            }
        } 
        g_steer_err_cur=Middle-g_line_new;
        //if(b_left_flag==0&&b_right_flag==0)
          
        b_left_last_flag=b_left_flag;
        b_right_last_flag=b_right_flag;
        if(b_right_last_flag==1)
        {
           b_right_last=b_right;
        }
        if(b_left_last_flag==1) 
        {
             b_left_last=b_left;
        }
        steer_out_this=STEER_MID-steer_control_P*g_steer_err_cur-steer_control_D*(g_steer_err_cur-g_steer_err_pre);//+steer_control_D*D;
        if(steer_out_this>=steer_control_out_max) 
        {                                          
            steer_out_this=steer_control_out_max;
        }                                                                
        else if(steer_out_this<=steer_control_out_min)
        {
           steer_out_this=steer_control_out_min;
        }
        steering((unsigned int)steer_out_this); 
        g_steer_err_pre=g_steer_err_cur;
        steer_out_last=steer_out_this;
        g_line_old=g_line_new; 
    
}
void delay(void)
{
   unsigned int i,j;
   for(i=0;i<65500;i++)
   {
     for(j=0;j<400;j++)
     {
     }
   }
 }

void bubble_sorts(unsigned char  a[],int start,int end)
{
	int i,j;
	unsigned char temp;
	for(j=start;j<end-1;j++)
	for(i=start;i<end-1-j;i++)
	if(a[i]>a[i+1])
	{
		temp=a[i];
		a[i]=a[i+1];
		a[i+1]=temp;
	}
}
void m_strcpy(unsigned char *des,unsigned char *src,int start,int end)
{
	int i;
	for(i=0;i<end-start;i++)
	{
		des[i]=src[start+i];
	}
}
void mid_val_3(unsigned char *A)
{
	int i;
	unsigned char A_temp[128];
	unsigned char A_temp2[3];
	memcpy(A_temp,A,128);
	for(i=0;i+3<128;i++)
	{
		m_strcpy(A_temp2,A_temp,i,i+3);
		bubble_sorts(A_temp2,0,3);
		A[3/2+i]=A_temp2[1];
	}

}
void mid_val_5(unsigned char *A)
{
	int i;
	unsigned char A_temp[128];
	unsigned char A_temp2[5];
	memcpy(A_temp,A,128);
	for(i=0;i+5<128;i++)
	{
		m_strcpy(A_temp2,A_temp,i,i+5);
		bubble_sorts(A_temp2,0,5);
		A[5/2+i]=A_temp2[1];
	}

}
void juzhi_lvbo(unsigned char *A)
{
  	int i,j;
  	int ave=0;
  	unsigned char A_temp[128];
  	unsigned char A_temp2[4];
  	memcpy(A_temp,A,128);
  	for(i=0;i+4<128;i++)
  	{
  		m_strcpy(A_temp2,A_temp,i,i+4);
  		for(j=0;j<4;j++)
  		{
  			ave+=A_temp2[j];

  		}	
  		A[4/2+i]=ave/4;
  		ave=0;
  	}

}	
int speed=25;


/***************************************/

//临时
#define PID_P  0.2//100//80  155    120
#define PID_I   0.2//100
#define PID_D 0.1//2
int set_speed=23; 
float Current_speed=0;  

float error=0;      
float last_error=0; 
float pre_error=0;	 
void car_speed(void)     //增量式PID正常处理部分
{       
   int pwmtemp;
   error=set_speed*16.5-Current_speed;
   pwmtemp=PWMDTY01+PID_P*(error-last_error)+PID_I*(error)+PID_D*(error+pre_error-2*last_error);					 
            if(pwmtemp<0) 
              {            
                  pwmtemp = 0;
              }                                                                
              else if(pwmtemp>58) 
             {
               pwmtemp = 58;
               }
            Motor_forward(pwmtemp);
            pre_error=last_error;
            last_error=error;
} 
/**************************************************************************/
int key_scan(int speed_1,int speed_2,int speed_3,int speed_4) 
{
    unsigned char stat;
    stat=PORTE&0x0f;
    stat=stat&0x0c;//取按键的高两位
    if(stat==0x0c)//全部在下 
    {  
        return speed_1;
    }
    if(stat==0x04)//最右边一个上去 
    {
        
       return speed_2; 
    }
    if(stat==0x08)//倒数第二个上去
    {
          
         return speed_3; 
    } 
    if(stat==0x00)//全部上去
    {
      
       return speed_4;
    }    
}

int select_yuzhi() 
{
    unsigned char stat=0;
    stat=PORTE&0x01;
    if(stat==1)//不按下去 
    {
        return std_yuzhi;
    } 
    else if(stat==0)//拨码开关左边第一上去，按键不按
    {
         return std_yuzhi+6;
    }   
}
void main(void) 
{ 
    char send_cnt=0;
    
    int speed_now;
    int daoche_count=0;
    int Stop_Flag=0;
    int got_yuzhi=20;
    SetBusCLK_40M();
    SCI0_Init();     
    PIT_Init();
    AD_Init();
    CCD_IO_Init();
    PWM_Init();
    PAC_Init();
    DDRT_DDRT0=1;
    
    DDRE=0XF0; // 拨码开关
    DDRM=0XFF;//寻线指示灯
    
    PTT_PTT0=1;
    DDRS_DDRS0=0;
   
    // // 拨码开关
    delay();

    steering(STEER_MID);
   ////////// speed_now=key_scan(36,40,44,46);
    speed_now=key_scan(39,42,45,48); 
    // 42 可以跑  45可以 48  近似可以 
    // key_scan(33,45,48,51);
    got_yuzhi=select_yuzhi();
    if(PTS_PTS0==0)
    {
          start_if=1;
    } 
    else 
    {
          start_if=0;
    }
    //7.64V speed_now=key_scan(24,32,38,42,52,54); 54无法适应 ，52可以 
    //set_speed=key_scan(22,24,26,28,30);
    Motor_forward(speed_now);
    
    EnableInterrupts;  
       
    for(;;)
    {
        
        if(TimerFlag20ms == 1) 
        {
            DisableInterrupts;
            ImageCapture(Pixel);
            TimerFlag20ms = 0;
            if(start_if==1) 
            {
              
              if(start<500) 
              {
                     start++;
              }     
              if(start>=500) 
              {
                    if(b_left_last_flag==1&&b_right_last_flag==1) 
                    {
                 
                      if(start_check(Pixel,3,got_yuzhi)>=6) 
                      {
                       
                         Stop_Flag=1;
                      } 
                    }
            
              }
              if(Stop_Flag==1) 
              {
                     
                     set_speed=0;
                     daoche_count++;
                     if(daoche_count<= 70) 
                     {
                         Motor_back(speed_now/2);
                     } 
                     else 
                     {    
  	                    PTM_PTM1=1;
  	                    PTM_PTM4=1;
                        Motor_brake();  
                     }
               }
            }
            
            mid_val_3(Pixel); 
            mid_val_3(Pixel);   
            CCD_P2(Pixel,3,got_yuzhi);
            steer_pd();
            EnableInterrupts;    
        }
    }
             
}


#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void) 
{
    static unsigned char TimerCnt20ms = 0;
    static int speed_count=0;
   // unsigned char integration_piont;

    PITTF_PTF0 = 1;

    TimerCnt20ms++;

    /* 根据曝光时间计算20ms周期内的曝光点 */
    /*integration_piont = 20 - IntegrationTime;
    if(integration_piont >= 2) {     
        if(integration_piont == TimerCnt20ms)
        StartIntegration();          
    }  */


    if(TimerCnt20ms >= 10) 
    {
        TimerCnt20ms = 0;
        TimerFlag20ms = 1;
        speed_count++;
        if(speed_count==2) 
        {
           speed_count=0;
           M_PACNT=PACNT;
           PACNT=0;
           Current_speed=M_PACNT; //这是速度
        }
    }
    
}
interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
{
  char sci_data;static i=0;
  char data_tem;
  int j=0;
  if(SCI0SR1_RDRF) 
  {
      sci_data = SCI0DRL;			
      if(sci_data=='S') 
      {
            Motor_brake();
      } 
      else  if(sci_data=='G')
      {
           Motor_forward(15);
      }
      else if(sci_data!='\n')
      {
          str_rec[i]=sci_data;
          i++;
      } 
      else
      {
          i=0; 
          if(str_rec[i]=='s'||str_rec[i]=='g') 
          {
            data_tem=str_rec[i];
            for(j=1;j<strlen(str_rec);j++) 
            {
                str_rec[j-1]=str_rec[j];
            }
            j=0;                     
            if(data_tem=='s') 
            {
                steer_control_P=atof(str_rec);
                Sci0_send_strings("the  steer_control_P is");
                send_pc(steer_control_P); 
            }
            else if(data_tem=='g')
            {
                speed=atof(str_rec);
                Motor_forward(speed);
                Sci0_send_strings("the speed is"); 
                send_pc(speed); 
            }
                
          }
       }
     
  }
}
#pragma CODE_SEG DEFAULT




