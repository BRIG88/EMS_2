#include "stm32f10x.h"


extern uint16_t TOCHEK;
extern __IO uint32_t ADC_RES;  


extern uint16_t K;



void ADC_EMS_8_Start(uint16_t Channel);
void ADC_EMS_4_Start(uint16_t Channel);
void ADC_EMS_2_Start(uint16_t Channel);