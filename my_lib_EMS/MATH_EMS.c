#include "stm32f10x_dma.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "ADC_EMS.h"
#include "MATH_EMS.h"


int16_t ADC1_RES[24];                   /// массив результатов 1-го АЦП
int16_t ADC2_RES[24];                   /// массив результатов 2-шо АЦП

int32_t Udsin_mass[24];                        /// сумма АЦП1 и АЦП2
int32_t Udcos_mass[24];                        /// сумма АЦП1 и АЦП2 смещенное на 90 


int32_t Sum_dsin=0;                         /// сумма массива Udsin_mass[n];
int32_t Sum_dcos=0;                         /// сумма массива Udcos_mass[n];

int32_t Udsin;                             ///  Sum_dsin/n;
int32_t Udcos;                             ///  Sum_dcos/n;


__IO uint32_t ADC_ARRAY[240]; 

int w;











void EMS_MATH_1(void){
  
uint16_t i;  




for(i=0; i<TOCHEK; i++) {
  
ADC1_RES[i]=(ADC_ARRAY[i] & 0xFFFF)-2048;   //signal        выделили 2 ацп из общих данных ///2048 center vref вернуть для устройства с аналоговой частью
ADC2_RES[i]=(ADC_ARRAY[i] >> 16)-2048;       // sine DAC
 
};



/*
for(i=0; i<6; i++) {                   /// в 10 бит сместили (шум убрали, проще вычисления)
  ADC1_RES[i]=ADC1_RES[i] >> 2;
  ADC2_RES[i]=ADC2_RES[i] >> 2;
};
*/


Sum_dsin=0;
Sum_dcos=0;

for(i=0; i<TOCHEK; i++) {

  if(i<(K)){  
w=i+(TOCHEK-K);
  }
  else {w=i-(K);};
  
Udsin_mass[i]=ADC1_RES[i]*ADC2_RES[i];
Udcos_mass[i]=ADC1_RES[i]*ADC2_RES[w];    

Sum_dsin+=Udsin_mass[i];
Sum_dcos+=Udcos_mass[i];

};

Udsin=Sum_dsin/TOCHEK;
Udcos=Sum_dcos/TOCHEK;
   
};


