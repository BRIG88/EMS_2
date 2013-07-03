#include "stm32f10x_dma.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "COMPENSATION.h"
#include "ADC_EMS.h"
#include "MATH_EMS.h"

int16_t COMP=0;
long float K1=0;
long float K2=0;
char COMP_FLAG=0;
uint16_t MINIMUM;
int k=0;

void COMP_init(void){
 
 DAC_InitTypeDef            DAC_InitStructure; 
  
 DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
 DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None ;
 DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;
 DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
 DAC_Init(DAC_Channel_2, &DAC_InitStructure);
 
 
 DAC_Cmd(DAC_Channel_2, ENABLE); 
  
 DAC_SetChannel2Data(DAC_Align_12b_R, COMP); 
};





