#include "all.h"


#define BUS_FREQ     40000000		//总线频率，运用了PLL超频技术
#define PWM_PRCLK   0x11			//CH_A = CH_B = BUS_FREQ/2
#define CH_A_FREQ    20000000		//时钟A频率
#define CH_B_FREQ    20000000		//时钟B频率
#define CH_SA_FREQ   2000000		//时钟SA频率
#define CH_SB_FREQ   2000000		//时钟SB频率
#define CH_SA_DIV    (CH_A_FREQ / (CH_SA_FREQ * 2))	 //CH_SA_DIV = 5
#define CH_SB_DIV    (CH_B_FREQ / (CH_SB_FREQ * 2))	 //CH_SB_DIV = 5
#define STEER_FREQ   100			//舵机频率100 Hz
#define MOTOR_FREQ  10000			//DC电动机频率10k

//CH_SA_FREQ =2M
//CH_SB_FREQ =2M

//此处的直流电机分别接的是 1 ，3通道，舵机接的是5通道
uint	M_PACNT;					//读取脉冲累加器的计数值
void SetBusCLK_40M(void)
{      
    CLKSEL_PLLSEL = 0;			//不使能锁相环时钟 
    PLLCTL_PLLON=1;				//锁相环电路允许
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //		)   */ 
  SYNR =0xc0 | 0x04;  // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80 MHz; 
                        // VCOFRQ[1:0]=1:1,代表VCO_clock在80~160 MHz之间
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
  REFDV=0x80 | 0x01;// VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80 MHz;
 					// REF_clock=osc_clock/(REFDV+1)=16/(1+1)=8 MHz
 					// REFFRQ[1:0]=1:0,因为参考时钟在6~12 MHz之间

    POSTDIV=0x00; 				//VCO_clock = PLL_clock
    
    _asm(nop);					//短暂延时，等待时钟频率稳定
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))	//时钟频率已稳定，锁相环频率锁定
    {
        ;
    }
    
    CLKSEL_PLLSEL =1; 			//使能锁相环时钟
}


void PAC_Init(void)	            
{
     PACTL = 0x40;     				//脉冲累加器A允许
     PACNT = 0x00;     				//脉冲累加器A计数初值
}
void PWM_Init(void)
{
    PWMPRCLK = PWM_PRCLK;
    PWMSCLA = CH_SA_DIV;	
    PWMSCLB = CH_SB_DIV;
    
    PWMCTL_CON45 = 1;					// 16位级联PWM45
    PWMCTL_CON23 = 1;					// 16位级联PWM23
    PWMCTL_CON01 = 1;					// 16位级联PWM01
    
    PWMCLK_PCLK5 = 1;					// PWM5使用SA
    PWMCLK_PCLK3 = 1;					// PWM3使用SB
    PWMCLK_PCLK1 = 1;					// PWM1使用SA
    
    PWMPER45 = (CH_SA_FREQ / STEER_FREQ);		// 写PWM45的周期寄存器
    PWMPER23 = (CH_SB_FREQ / MOTOR_FREQ);		// 写PWM23的周期寄存器  200
    PWMPER01 = (CH_SB_FREQ / MOTOR_FREQ);		// 写PWM01的周期寄存器  200
    PWMPOL_PPOL5 = 1;							// 极性为正
    PWMPOL_PPOL3 = 1;							// 极性为正
    PWMPOL_PPOL1 = 1; 
    PWMCAE = 0x00;								// 左对齐
    PWME_PWME5 = 1;								// 使能PWM45 
}

/*************************************************************************
//功    能：直流电动机正转
//入口参数：unsigned int duty，占空比值
//出口参数：无
*************************************************************************/
void Motor_forward (int duty)//最大是2M/5000 
{
  //电机要亲自测试，这里可以触发中断，P,H,J口
    PWME_PWME1 = 1;						// 使能IN1
    PWME_PWME3 = 0;						// 禁止IN2
    DDRP_DDRP3 = 1;						// PTP_PTP3为输出,考虑去掉
    PTP_PTP3 = 0;						// 输出0
    PWMDTY01 = duty;
}

/*************************************************************************
//功    能：直流电动机反转
//入口参数：unsigned int duty，占空比值
//出口参数：无
*************************************************************************/

void Motor_back(int duty)
{
    PWME_PWME3 = 1;
    PWME_PWME1 = 0;
    DDRP_DDRP1 = 1;
    PTP_PTP1 = 0;
    PWMDTY23 = duty;
}

/*************************************************************************
//功    能：直流电动机停车
//入口参数：无
//出口参数：无
*************************************************************************/
void Motor_brake(void)
{
    PWME_PWME3 = 0;
    PWME_PWME1 = 0;  		// 禁止特殊功能
    DDRP_DDRP3 = 1;
    DDRP_DDRP1 = 1;   		// 端口为输出
    PTP_PTP3 = 0;
    PTP_PTP1 = 0;       		// 端口输出0
}

void steering(int duty)
{
    PWMDTY45 = duty;
}	// 端口输出0


