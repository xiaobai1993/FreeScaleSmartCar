/**********************************************************************************
********************* (C) COPYRIGHT 2012 蓝宙电子科技有限公司 *********************
***********************************************************************************
 * 描述         ：线性CCD测试程序
 *
 * 实验平台     ：蓝宙电子XS128系统板 + TSL1401线性CCD传感器
 *
 * 作者         ：蓝宙电子科技有限公司
 * 淘宝店       ：http://landzo.taobao.com
 * 网站         ：http://www.landzo.cn
**********************************************************************************/
/*
*********************************************************************************************************
*
*                                      MCU: MC9S12XS128MAL - 112Pin
*                                      OSC: 16.000MHz
*                                      BUS: 40.0MHz
*
* File : SCI.c
* By   : Ke Chao
*********************************************************************************************************
*/

#include "includes.h"


/*
*********************************************************************************************************
* Description: SCI0_Init
* Note: Fbus = 40M ---> Bit Rate = 115200
*********************************************************************************************************
*/
void SCI0_Init(void)
{
/*  Init_SCI init code */
  SCI0CR1 = 0x00;
  SCI0SR2 = 0x80;                      /* Switch to the alternative register set */
  SCI0ASR1 = 0x83;                     /* Clear alternative status flags */
  SCI0ACR1 = 0x00;
  SCI0ACR2 = 0x00;
  SCI0SR2 = 0x00;                      /* Switch to the normal register set */
  (void) SCI0SR1;                      /* Reset interrupt request flags */
  SCI0CR2 = 0x00;                      /* Disable error interrupts */
  SCI0BD = 0x16;                       /* Set prescaler bits */
  SCI0CR2 |= (SCI0CR2_TE_MASK | SCI0CR2_RE_MASK); /* Enable transmitter, Enable receiver */
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