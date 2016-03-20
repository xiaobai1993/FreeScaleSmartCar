#include"all.h"
char str_send[12];
char str_rec[12];
void Sci0_send_space(void)
{
      Sci0_send_data(' ');   
}

void Sci0_send_enter(void)
{
      
        Sci0_send_data('\n');
        
}
void dtos(float res)//小数转字符
{
	long pow[]={1,10,100,1000,10000,100000,1000000};
	long temp;
	int i=0;
	int t=0;
	int len=0;
	temp=(long)(res*100);
	if(res!=0)
	{
	  
  for(;;)
	{
		if(temp>=pow[i])
		{
			i++;
		}
		else
		{
			break;
		}
	}
	for(t=0;t<i;t++)
	{
		str_send[t]=(char)(temp/pow[i-1-t]+'0');
		temp=temp%pow[i-1-t];
	}
	str_send[i]='\0';
	len=strlen(str_send);
	str_send[len+1]='\0';
	str_send[len]=str_send[len-1];
	str_send[len-1]=str_send[len-2];
	str_send[len-2]='.';
	if(str_send[0]=='.')
	{
		str_send[strlen(str_send)+1]='\0';
		for(i=strlen(str_send);i>0;i--)
		{
			str_send[i]=str_send[i-1];
		}
		str_send[0]='0';
	}
	}
	else
  {
      str_send[0]='0';
      str_send[1]='\0';
  }
	  
	
}
void Sci0_send_data(unsigned char dat) 
{

      while(!SCI0SR1_TDRE)     // 等待发送缓冲区空
      {
            ;
      }                 
       
      SCI0DRL = dat;		                    
}

void Sci0_send_strings_dot(void)
{
   
    unsigned char str[]={".|"};     
}

void Sci0_send_strings(char *str) 
{
      unsigned char dat,i=0;
    
      for(;;)
      {
          dat = str[i];
          if(dat=='\0')    
          {
          	break;
          }
          Sci0_send_data(dat); 
          i++ ;   
      }
} 
 

double receive_pc(void) 
{
  char i=0,sci_data;
  float rec=-1;
  for(; ;)
  {
     while (!SCI0SR1_RDRF)
     {
         ;
     }
     sci_data = SCI0DRL;			//接收数据
     if(sci_data!='\n')
      {
          str_rec[i]=sci_data;
          i++;
      } 
      else 
      {
        break;
      }
  }
  rec=atof(str_rec);
  memset(str_rec,0x00,12);
  return rec;
}


void send_pc(float num)
{
    dtos(num);
    Sci0_send_strings(str_send);
    memset(str_send,0x00,12);
}
void SCI0_Init(void)
{
    SCI0BD = 40000000/16/115200;       	     //设置波特率为115200 bps
    SCI0CR1 = 0x00;      	//LOOPS=0,SCISWAI=0,RSRC=0,M=0,
	                    		//WAKE=0,ILT=0,PE=0,PT=0 	
    SCI0CR2 = 0x0C;      	//TIE=0,TCIE=0,RIE=0,ILIE=0,
	                    		//TE=1,RE=1,RWU=0,SBK=0
}


/*
*********************************************************************************************************
* Description: SCI0_SendChar
*********************************************************************************************************
*/

uint8_t SCI0_SendChar(uint8_t Chr)
{
  if(SCI0SR1_TDRE == 0) {              /* Is the transmitter empty? */
    return ERR_TXFULL;                 /* If no then error */
  }
  SCI0DRL = (uint8_t)Chr;
  return ERR_OK;                       /* OK */
}


/*
*********************************************************************************************************
* Description: SCI0_SendChar1
*********************************************************************************************************
*/
void SCI0_SendChar1(uint8_t Chr)
{
  while(SCI0SR1_TDRE == 0) {           /* Is the transmitter empty? */
  }
  SCI0DRL = (uint8_t)Chr;
}


/*
*********************************************************************************************************
* Description: SCI0_RecvChar
*********************************************************************************************************
*/
uint8_t SCI0_RecvChar(uint8_t *Chr)
{
  uint8_t Result = ERR_OK;             /* Return error code */
  uint8_t StatReg = SCI0SR1;           /* Read status register */

  if (StatReg & (SCI0SR1_OR_MASK|SCI0SR1_NF_MASK|SCI0SR1_FE_MASK|SCI0SR1_PF_MASK)) { /* Is any error flag set? */
    Result = ERR_COMMON;               /* If yes then set common error value */
  } else if ((StatReg & SCI0SR1_RDRF_MASK) == 0) {  /* Is the reciver empty and no error is set? */
    return ERR_RXEMPTY;                /* If yes then error */
  } else {
    /* Else branch due to MISRA compliance */
  }
  *Chr = SCI0DRL;                      /* Read data from the receiver */
  return Result;                       /* Return error code */
}


/*
*********************************************************************************************************
* Description: SCI0_RecvChar1
*********************************************************************************************************
*/
 
uint8_t SCI0_RecvChar1(void)
{
  while(SCI0SR1_RDRF == 0) {           /* Is the reciver empty? */
  }
  return (uint8_t)SCI0DRL;             /* Read data from the receiver */
}
void SendHex(unsigned char hex) {
  unsigned char temp;
  temp = hex >> 4;
  if(temp < 10) {
    SCI0_SendChar1(temp + '0');
  } else {
    SCI0_SendChar1(temp - 10 + 'A');
  }
  temp = hex & 0x0F;
  if(temp < 10) {
    SCI0_SendChar1(temp + '0');
  } else {
    SCI0_SendChar1(temp - 10 + 'A');
  }
}






















 