#include "all.h"
unsigned char PixelAverageValue;
/* 128�����ص��ƽ����ѹֵ��10�� */
unsigned char PixelAverageVoltage;

/* �趨Ŀ��ƽ����ѹֵ��ʵ�ʵ�ѹ��10�� */
int TargetPixelAverageVoltage = 30;
/* �趨Ŀ��ƽ����ѹֵ��ʵ��ֵ��ƫ�ʵ�ʵ�ѹ��10�� */
int PixelAverageVoltageError = 0;
/* �趨Ŀ��ƽ����ѹֵ�����ƫ�ʵ�ʵ�ѹ��10�� */
int TargetPixelAverageVoltageAllowError = 2;

/* �ع�ʱ�䣬��λms */
unsigned char IntegrationTime = 10;



void CalculateIntegrationTime(void) {

    /* ����128�����ص��ƽ��ADֵ */
    PixelAverageValue = PixelAverage(128,Pixel);
    /* ����128�����ص��ƽ����ѹֵ,ʵ��ֵ��10�� */
    PixelAverageVoltage = (unsigned char)((int)PixelAverageValue * 25 / 128);

    PixelAverageVoltageError = TargetPixelAverageVoltage - PixelAverageVoltage;
    if(PixelAverageVoltageError < -TargetPixelAverageVoltageAllowError)
        IntegrationTime--;
    if(PixelAverageVoltageError > TargetPixelAverageVoltageAllowError)
        IntegrationTime++;
    if(IntegrationTime <= 1)
        IntegrationTime = 1;
    if(IntegrationTime >= 20)
        IntegrationTime = 20;
}


unsigned char PixelAverage(unsigned char len, unsigned char *data) {
  unsigned char i;
  unsigned int sum = 0;
  for(i = 0; i<len; i++) {
    sum = sum + *data++;
  }
  return ((unsigned char)(sum/len));
}


void SendImageData(unsigned char * ImageData) 
{

    unsigned char i;
    unsigned char crc = 0;

    /* Send Data */
    SCI0_SendChar1('*');
    SCI0_SendChar1('L');
    SCI0_SendChar1('D');

    SendHex(0);
    SendHex(0);
    SendHex(0);
    SendHex(0);

    for(i=0; i<128; i++) {
      SendHex(*ImageData++);
    }

    SendHex(crc);
    SCI0_SendChar1('#');
}


void StartIntegration(void) {

    unsigned char i;

    TSL1401_SI = 1;         /* SI  = 1 */
    SamplingDelay();
    TSL1401_CLK = 1;        /* CLK = 1 */
    SamplingDelay();
    TSL1401_SI = 0;         /* SI  = 0 */
    SamplingDelay();
    TSL1401_CLK = 0;        /* CLK = 0 */

    for(i=0; i<127; i++) {
        SamplingDelay();
        SamplingDelay();
        TSL1401_CLK = 1;    /* CLK = 1 */
        SamplingDelay();
        SamplingDelay();
        TSL1401_CLK = 0;    /* CLK = 0 */
    }
    SamplingDelay();
    SamplingDelay();
    TSL1401_CLK = 1;        /* CLK = 1 */
    SamplingDelay();
    SamplingDelay();
    TSL1401_CLK = 0;        /* CLK = 0 */
}

void ImageCapture(unsigned char * ImageData) {

    unsigned char i;
    unsigned int  temp_int;

    TSL1401_SI = 1;         /* SI  = 1 */
    SamplingDelay();
    TSL1401_CLK = 1;        /* CLK = 1 */
    SamplingDelay();
    TSL1401_SI = 0;         /* SI  = 0 */
    SamplingDelay();

    //Delay 20us for sample the first pixel
    for(i = 0; i < 20; i++) {
        Cpu_Delay1us();
    }

    //Sampling Pixel 1
    temp_int = AD_Measure12(0);
    *ImageData++ = (byte)(temp_int>>4);
    TSL1401_CLK = 0;        /* CLK = 0 */

    for(i=0; i<127; i++) {
        SamplingDelay();
        SamplingDelay();
        TSL1401_CLK = 1;    /* CLK = 1 */
        SamplingDelay();
        SamplingDelay();
        //Sampling Pixel 2~128
        temp_int = AD_Measure12(0);
        *ImageData++ = (byte)(temp_int>>4);
        TSL1401_CLK = 0;    /* CLK = 0 */
    }
    SamplingDelay();
    SamplingDelay();
    TSL1401_CLK = 1;        /* CLK = 1 */
    SamplingDelay();
    SamplingDelay();
    TSL1401_CLK = 0;        /* CLK = 0 */
   
}


void CCD_IO_Init(void) {
    TSL1401_CLK_DDR = 1;
    TSL1401_SI_DDR  = 1;
    TSL1401_CLK = 0;
    TSL1401_SI  = 0;
}
void AD_Init(void) {
    ATD0CTL4 = 0x04;                    /* Set sample time and prescaler */
    ATD0CTL3 = 0x88;                    /* Right justified data,Conversion Sequence Length = 1 */
    ATD0CTL0 = 0x0F;                    /* Set wrap around */
    ATD0CTL1 = 0x4F;                    /* 12-bit data */
    ATD0CTL2 = 0x40;                    /* Fast Flag Clear */
}
uint16_t AD_Measure12(uint8_t Channel) 
{
    ATD0CTL5_Cx = Channel;
    while(ATD0STAT0_SCF == 0);
    return ATD0DR0;
}


   























