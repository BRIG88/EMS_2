#include "stm32f10x_dma.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "SINE_EMS.h"





DAC_InitTypeDef            DAC_InitStructur;         //объявлять в начале (иначе глюки при сбросе питания МК, хз почему)!!!!!!!!!!!!!
DMA_InitTypeDef            DMA_InitStructur;


#define DAC_DHR8R1_Address      0x40007408

const uint16_t Sine12bit[90] =                                  /// таблица для генерации синуса
{2047,   2154,  2261, 
 2366,  2470,   
2572,    2672,  2768, 
  2861,   2950,    
3034,    3114,    3188,  
  3257,    3320,   
3377,    3427,    3471,  
  3507,   3537,    
3559,    3574,    3582,  
  3582,    3574,   
3559,   3537,    3507, 
 3471,   3427,    
3377,    3320,    3257,  
 3188,    3114, 
3034,    2950,   2861,  
 2768,    2672,   
2572,    2470,    2366,  
  2261,   2154,   
2047,   1940,    1833,  
  1728,    1624,    
1522,    1422,    1326,  
  1233,    1144,   
1060,    980,   906, 
 837,   774,   
717,    667,    623,  
  587,    557, 
535,    520,    512,  
  512,   520,    
535,    557,    587,  
  623,    667,    
717,   774,    837, 
  906,   980,   
1060,   1144,    1233,  
  1326,    1422,    
1522,    1624,    1728,  
  1833,    1940};



void SINE_8_EMS(void){

 
  


  


 
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructur;
  
  /* TIM6 Configuration */
  /* Time base configuration */
//  8kHz
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructur);                       ///Таймер для генерации синуса
  TIM_TimeBaseStructur.TIM_Period = 100;          
  TIM_TimeBaseStructur.TIM_Prescaler = 0x0;       
  TIM_TimeBaseStructur.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructur.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructur);


  
  
  
  
  
  
  
  

/* TIM7 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  
  /* DAC channel1 Configuration */
  
  

  DAC_InitStructur.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructur.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructur.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructur);


  /* DMA2 channel3 configuration */
  DMA_DeInit(DMA2_Channel3);

  

DMA_InitStructur.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;
DMA_InitStructur.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit;
DMA_InitStructur.DMA_DIR = DMA_DIR_PeripheralDST;
DMA_InitStructur.DMA_BufferSize = 90;
DMA_InitStructur.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructur.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructur.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
DMA_InitStructur.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
DMA_InitStructur.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructur.DMA_Priority = DMA_Priority_High;
DMA_InitStructur.DMA_M2M = DMA_M2M_Disable;
DMA_Init(DMA2_Channel3, &DMA_InitStructur);
DMA_Cmd(DMA2_Channel3, ENABLE);
  
DAC_Cmd(DAC_Channel_1, ENABLE); // включить синус
DAC_DMACmd(DAC_Channel_1, ENABLE);
TIM_Cmd(TIM6, ENABLE);
  
}



void SINE_4_EMS(void){




 
 
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructur;
  
  /* TIM6 Configuration */
  /* Time base configuration */
//  8kHz
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructur);                       ///Таймер для генерации синуса 4 KHz
  TIM_TimeBaseStructur.TIM_Period = 200;          
  TIM_TimeBaseStructur.TIM_Prescaler = 0x0;       
  TIM_TimeBaseStructur.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructur.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructur);



/* TIM7 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  
  /* DAC channel1 Configuration */
  
  

DAC_InitStructur.DAC_Trigger = DAC_Trigger_T6_TRGO;
DAC_InitStructur.DAC_WaveGeneration = DAC_WaveGeneration_None;
DAC_InitStructur.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
DAC_Init(DAC_Channel_1, &DAC_InitStructur);


  /* DMA2 channel3 configuration */
  DMA_DeInit(DMA2_Channel3);

  

DMA_InitStructur.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;
DMA_InitStructur.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit;
DMA_InitStructur.DMA_DIR = DMA_DIR_PeripheralDST;
DMA_InitStructur.DMA_BufferSize = 90;
DMA_InitStructur.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructur.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructur.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
DMA_InitStructur.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
DMA_InitStructur.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructur.DMA_Priority = DMA_Priority_High;
DMA_InitStructur.DMA_M2M = DMA_M2M_Disable;
DMA_Init(DMA2_Channel3, &DMA_InitStructur);
DMA_Cmd(DMA2_Channel3, ENABLE);


DAC_Cmd(DAC_Channel_1, ENABLE); // включить синус
DAC_DMACmd(DAC_Channel_1, ENABLE);
TIM_Cmd(TIM6, ENABLE);
  
}



void SINE_2_EMS(void){




 
 
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructur;
  
  /* TIM6 Configuration */
  /* Time base configuration */
//  8kHz
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructur);                       ///Таймер для генерации синуса 4 KHz
  TIM_TimeBaseStructur.TIM_Period = 400;          
  TIM_TimeBaseStructur.TIM_Prescaler = 0x0;       
  TIM_TimeBaseStructur.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructur.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructur);



/* TIM7 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  
  /* DAC channel1 Configuration */
  
  

DAC_InitStructur.DAC_Trigger = DAC_Trigger_T6_TRGO;
DAC_InitStructur.DAC_WaveGeneration = DAC_WaveGeneration_None;
DAC_InitStructur.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
DAC_Init(DAC_Channel_1, &DAC_InitStructur);


  /* DMA2 channel3 configuration */
DMA_DeInit(DMA2_Channel3);

  

DMA_InitStructur.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;
DMA_InitStructur.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit;
DMA_InitStructur.DMA_DIR = DMA_DIR_PeripheralDST;
DMA_InitStructur.DMA_BufferSize = 90;
DMA_InitStructur.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructur.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructur.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
DMA_InitStructur.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
DMA_InitStructur.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructur.DMA_Priority = DMA_Priority_High;
DMA_InitStructur.DMA_M2M = DMA_M2M_Disable;


  DMA_Init(DMA2_Channel3, &DMA_InitStructur);
 
  DMA_Cmd(DMA2_Channel3, ENABLE);
  
DAC_Cmd(DAC_Channel_1, ENABLE); // включить синус
DAC_DMACmd(DAC_Channel_1, ENABLE);
TIM_Cmd(TIM6, ENABLE);
  
}