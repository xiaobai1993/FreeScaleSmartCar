#include"def.h"
void SCI0_Init(void); 

void SendImageData(unsigned char * ImageData);
uint8_t SCI0_RecvChar1(void);
uint8_t SCI0_RecvChar(uint8_t *Chr);
void SCI0_SendChar1(uint8_t Chr);
uint8_t SCI0_SendChar(uint8_t Chr);
 void SCI0_Init(void);
void send_pc(float num); 
double receive_pc() ;
void Sci0_send_strings(char *str) ;
void Sci0_send_data(unsigned char dat) ;
void dtos(float res) ;
void Sci0_send_space(void) ;
void Sci0_send_enter(void) ;
void SendHex(unsigned char hex);
