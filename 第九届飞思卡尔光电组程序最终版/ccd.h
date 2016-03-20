#include"def.h"
void AD_Init(void);
void CCD_IO_Init(void);
uint16_t AD_Measure12(uint8_t Channel);
void CalculateIntegrationTime(void) ;
unsigned char PixelAverage(unsigned char len, unsigned char *data);
void StartIntegration(void);
void ImageCapture(unsigned char * ImageData) ;
unsigned char PixelAverage(unsigned char len, unsigned char *data);