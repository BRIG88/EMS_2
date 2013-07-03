#include "stm32f10x.h"


extern int32_t Udsin;                             ///  Sum_dsin/n;
extern int32_t Udcos;                             ///  Sum_dcos/n;
extern int16_t ADC1_RES[24];                   /// массив результатов 1-го АЦП
extern int16_t ADC2_RES[24];                   /// массив результатов 2-шо АЦП
extern __IO uint32_t ADC_ARRAY[240]; 



void EMS_MATH_1(void);


