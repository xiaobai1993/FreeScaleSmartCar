#include"all.h"

//龙邱ccd广角镜头无畸变 前瞻35-40
//P二次方的系数是0.2，D给30  速度值 20。可跑完。
/**********************************************/	
unsigned char Pixel[128];
extern char str_send[12];
extern char str_rec[12];
#define STEER_MID 1935

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
float steer_control_D=30;//30 200 180 130        
float steer_control_P=12.5;//15

int steer_control_out_max=2580;
int steer_control_out_min=1340;
int g_line_new=Middle;

int g_line_old=Middle;
float steer_out_this=STEER_MID;//中间位置
float steer_out_last=STEER_MID;
float g_steer_err_cur=0;
float g_steer_err_pre=0;
float g_steer_err_last=0;

void send_mes() 
{
   double temp;
  temp= PixelAverageVoltage;
  send_pc(temp);
  Sci0_send_space();
  /*temp=b_left;   
  send_pc(temp);
  Sci0_send_space();
  
  temp=b_left_flag;
  send_pc(temp);
  Sci0_send_space();

  temp=b_left_last;
  send_pc(temp);
  Sci0_send_space();
  
  temp=b_right;
  send_pc(temp);
  Sci0_send_space();
  
  temp=b_right_last;
  send_pc(temp);
  Sci0_send_space();
  
  temp=b_right_flag;
  send_pc(temp);
  Sci0_send_space();
  
  temp=g_line_old;
  send_pc(temp);
  Sci0_send_space();
  Sci0_send_enter(); */
} 
void steer_pd(void) 
{
  
    //float D=0;
    //float P=0;

      g_line_new=(int)Calculate_line();
    //g_line_new=DirectionControl();
      g_steer_err_cur=Middle-g_line_new;
      if(fabs(g_line_new-g_line_old)>40) 
      {
          g_line_new=g_line_old;
       }
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
        //if(fabs(g_steer_err_cur)<20)
        {
           steer_control_P=23;
        } 
        //else 
        {
           // steer_control_P=(g_steer_err_cur*g_steer_err_cur*0.015+10);//0.2 0.5 0.9
        }
   
        // 0.02 有点小 //d 150
        /*****************************单系数*************/
        // 0.06 d 150   打大角度晚了 有点摇晃大角度好
        //0.06  d 120 感觉进弯道 打大角度晚了 不摇晃了
        // 0.08  打角度小
        //0.16  感觉提前打角度了
        //0.115 看到赛道外 
        // 0.095  感觉小
        
        //0.005 轻轻走着 近似可以出弯道
        //0.003 轻轻走着 弯道出去
        //0.00
        /*系数ab********************************/
        //0.002 ,5 小，过不去
        //0.003  ,10效果不错
        //0.004+5   拐弯的时候靠外  出弯道车身不正，偏外。
        //0.003+5   拐弯的时候靠外+1 出弯道车身不正，偏外。
        //0.0045+5   拐弯的时候靠外,出弯道车身不正，偏外。
        
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
     for(j=0;j<200;j++)
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

int speed=25;


/***************************************/

//临时
#define PID_P 120//80  155    120
#define PID_I   0//100
#define PID_D 0//2

int Current_speed=0;  
int set_speed=20; 
int error=0;      //现在偏差 
int last_error=0; //上次偏差
int pre_error=0;	 //再上一次偏差
void car_speed(void)     //增量式PID正常处理部分
{       
   int pwmtemp;
//set_speed=25;
   error=set_speed-Current_speed;
   pwmtemp=PWMDTY01+PID_P*(error-last_error)+PID_I*(error)+PID_D*(error+pre_error-2*last_error);					 
            if(pwmtemp<25) 
              {            
                  pwmtemp = 25;
              }                                                                
              else if(pwmtemp>35) 
             {
               pwmtemp = 35;
               }
            Motor_forward(pwmtemp);
            pre_error=last_error;
            last_error=error;
} 
/**************************************************************************/
void main(void) 
{ 
    char send_cnt=0;
    SetBusCLK_40M();
    SCI0_Init();     
    PIT_Init();
    AD_Init();
    CCD_IO_Init();
    PWM_Init();
    PAC_Init();
    DDRT_DDRT0=1;
    PTT_PTT0=1;
    delay();
    Motor_forward(26);
    steering(STEER_MID);
    DDRM=0XFF;
    EnableInterrupts;
    for(;;)
    {

        if(TimerFlag20ms == 1) 
        {
            DisableInterrupts;
            TimerFlag20ms = 0;
            ImageCapture(Pixel);
            //CalculateIntegrationTime();
            //mid_val_3(Pixel);
            //send_cnt++;
            /*if(send_cnt>10) 
            {
                send_cnt=0;
                SendImageData(Pixel);
            } */
            find(Pixel,5,3,20);
            //CCD_P2(Pixel,3,18);
           steer_pd();
           EnableInterrupts;    
        }
    }
             
}


#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void) 
{
    static unsigned char TimerCnt20ms = 0;
   // unsigned char integration_piont;

    PITTF_PTF0 = 1;

    TimerCnt20ms++;

    /* 根据曝光时间计算20ms周期内的曝光点 */
    /*integration_piont = 20 - IntegrationTime;
    if(integration_piont >= 2) {     
        if(integration_piont == TimerCnt20ms)
        StartIntegration();          
    }  */


    if(TimerCnt20ms >= 10) {
        TimerCnt20ms = 0;
        TimerFlag20ms = 1;
        Current_speed=M_PACNT; //这是速度
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




