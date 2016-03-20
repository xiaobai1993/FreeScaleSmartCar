#include "all.h"


#define BUS_FREQ     40000000		//����Ƶ�ʣ�������PLL��Ƶ����
#define PWM_PRCLK   0x11			//CH_A = CH_B = BUS_FREQ/2
#define CH_A_FREQ    20000000		//ʱ��AƵ��
#define CH_B_FREQ    20000000		//ʱ��BƵ��
#define CH_SA_FREQ   2000000		//ʱ��SAƵ��
#define CH_SB_FREQ   2000000		//ʱ��SBƵ��
#define CH_SA_DIV    (CH_A_FREQ / (CH_SA_FREQ * 2))	 //CH_SA_DIV = 5
#define CH_SB_DIV    (CH_B_FREQ / (CH_SB_FREQ * 2))	 //CH_SB_DIV = 5
#define STEER_FREQ   100			//���Ƶ��100 Hz
#define MOTOR_FREQ  10000			//DC�綯��Ƶ��10k

//CH_SA_FREQ =2M
//CH_SB_FREQ =2M

//�˴���ֱ������ֱ�ӵ��� 1 ��3ͨ��������ӵ���5ͨ��
uint	M_PACNT;					//��ȡ�����ۼ����ļ���ֵ
void SetBusCLK_40M(void)
{      
    CLKSEL_PLLSEL = 0;			//��ʹ�����໷ʱ�� 
    PLLCTL_PLLON=1;				//���໷��·����
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //		)   */ 
  SYNR =0xc0 | 0x04;  // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80 MHz; 
                        // VCOFRQ[1:0]=1:1,����VCO_clock��80~160 MHz֮��
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
  REFDV=0x80 | 0x01;// VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80 MHz;
 					// REF_clock=osc_clock/(REFDV+1)=16/(1+1)=8 MHz
 					// REFFRQ[1:0]=1:0,��Ϊ�ο�ʱ����6~12 MHz֮��

    POSTDIV=0x00; 				//VCO_clock = PLL_clock
    
    _asm(nop);					//������ʱ���ȴ�ʱ��Ƶ���ȶ�
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))	//ʱ��Ƶ�����ȶ������໷Ƶ������
    {
        ;
    }
    
    CLKSEL_PLLSEL =1; 			//ʹ�����໷ʱ��
}


void PAC_Init(void)	            
{
     PACTL = 0x40;     				//�����ۼ���A����
     PACNT = 0x00;     				//�����ۼ���A������ֵ
}
void PWM_Init(void)
{
    PWMPRCLK = PWM_PRCLK;
    PWMSCLA = CH_SA_DIV;	
    PWMSCLB = CH_SB_DIV;
    
    PWMCTL_CON45 = 1;					// 16λ����PWM45
    PWMCTL_CON23 = 1;					// 16λ����PWM23
    PWMCTL_CON01 = 1;					// 16λ����PWM01
    
    PWMCLK_PCLK5 = 1;					// PWM5ʹ��SA
    PWMCLK_PCLK3 = 1;					// PWM3ʹ��SB
    PWMCLK_PCLK1 = 1;					// PWM1ʹ��SA
    
    PWMPER45 = (CH_SA_FREQ / STEER_FREQ);		// дPWM45�����ڼĴ���
    PWMPER23 = (CH_SB_FREQ / MOTOR_FREQ);		// дPWM23�����ڼĴ���  200
    PWMPER01 = (CH_SB_FREQ / MOTOR_FREQ);		// дPWM01�����ڼĴ���  200
    PWMPOL_PPOL5 = 1;							// ����Ϊ��
    PWMPOL_PPOL3 = 1;							// ����Ϊ��
    PWMPOL_PPOL1 = 1; 
    PWMCAE = 0x00;								// �����
    PWME_PWME5 = 1;								// ʹ��PWM45 
}

/*************************************************************************
//��    �ܣ�ֱ���綯����ת
//��ڲ�����unsigned int duty��ռ�ձ�ֵ
//���ڲ�������
*************************************************************************/
void Motor_forward (int duty)//�����2M/5000 
{
  //���Ҫ���Բ��ԣ�������Դ����жϣ�P,H,J��
    PWME_PWME1 = 1;						// ʹ��IN1
    PWME_PWME3 = 0;						// ��ֹIN2
    DDRP_DDRP3 = 1;						// PTP_PTP3Ϊ���,����ȥ��
    PTP_PTP3 = 0;						// ���0
    PWMDTY01 = duty;
}

/*************************************************************************
//��    �ܣ�ֱ���綯����ת
//��ڲ�����unsigned int duty��ռ�ձ�ֵ
//���ڲ�������
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
//��    �ܣ�ֱ���綯��ͣ��
//��ڲ�������
//���ڲ�������
*************************************************************************/
void Motor_brake(void)
{
    PWME_PWME3 = 0;
    PWME_PWME1 = 0;  		// ��ֹ���⹦��
    DDRP_DDRP3 = 1;
    DDRP_DDRP1 = 1;   		// �˿�Ϊ���
    PTP_PTP3 = 0;
    PTP_PTP1 = 0;       		// �˿����0
}

void steering(int duty)
{
    PWMDTY45 = duty;
}	// �˿����0


