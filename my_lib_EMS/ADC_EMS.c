#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "ADC_EMS.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
__IO uint32_t ADC_RES;                    /// массив результатов 2-х АЦП  для 8 кГц
                   

uint16_t TOCHEK;
uint16_t K;

void ADC_EMS_8_Start(uint16_t Channel){

 TOCHEK=6;
 K=2; //сдвиг 90 градусов
TIM_Cmd(TIM3, DISABLE);  

  
  
  
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructur;  
TIM_TimeBaseStructInit(&TIM_TimeBaseStructur);             /// настройка TIM3 для срабатывания АЦП
TIM_TimeBaseStructur.TIM_Period = 500;                     ///48 kHz
TIM_TimeBaseStructur.TIM_Prescaler = 3;       
TIM_TimeBaseStructur.TIM_ClockDivision = 0;    
TIM_TimeBaseStructur.TIM_CounterMode = TIM_CounterMode_Up;  
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructur);
TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
//NVIC_EnableIRQ(TIM3_IRQn);    /// TIM2
//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

DMA_InitTypeDef DMA_InitStructure;                                        /// конфигурация DMA1_Channel1 для работы АЦП
DMA_DeInit(DMA1_Channel1);
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;     /// Адрес регистра данных АЦП
DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_RES;                /// Адрес регистра массива для записи данных
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
DMA_InitStructure.DMA_BufferSize = 1;                                     /// Для 8 кГц массив из 6
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
DMA_Init(DMA1_Channel1, &DMA_InitStructure);
DMA_Cmd(DMA1_Channel1, ENABLE);
//NVIC_EnableIRQ(DMA1_Channel1_IRQn);
//DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);




ADC_InitTypeDef ADC_InitStruct;                                         /// Конфигурация АЦП 1
ADC_InitStruct.ADC_Mode = ADC_Mode_FastInterl;
ADC_InitStruct.ADC_ScanConvMode = DISABLE;
ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;     /// Запуск по Таймеру Т3
ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStruct.ADC_NbrOfChannel = 1;
ADC_Init(ADC1, &ADC_InitStruct);



 switch (Channel){
 //case 0: ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_13Cycles5);  break; //занят для АЦП2
 case 1: ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);  break;
 case 2: ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_13Cycles5);  break;
 case 3: ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_13Cycles5);  break;
 case 4: ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_13Cycles5);  break;
 case 5: ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5);  break;
 case 6: ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_13Cycles5);  break;
 case 7: ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_13Cycles5);  break;
 case 8: ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_13Cycles5);  break;
  default: ;
 }




ADC_DMACmd(ADC1, ENABLE);
ADC_ExternalTrigConvCmd(ADC1, ENABLE);
 NVIC_EnableIRQ(ADC1_2_IRQn);  /// ADC1_2 NVIC 
ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);                                  /// прерывание по завершению АЦП



ADC_InitStruct.ADC_Mode = ADC_Mode_FastInterl;                           /// Конфигурация АЦП 2
ADC_InitStruct.ADC_ScanConvMode = DISABLE;
ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStruct.ADC_NbrOfChannel = 1;
ADC_Init(ADC2, &ADC_InitStruct);
ADC_RegularChannelConfig(ADC2, ADC_Channel_14, 1, ADC_SampleTime_13Cycles5);            ///22 пин
ADC_DMACmd(ADC2, ENABLE);
ADC_ExternalTrigConvCmd(ADC2, ENABLE);
  
  ADC_Cmd(ADC1, ENABLE);                               //// Включение и калибровка АЦП1
  
 /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));  
  
/* Enable ADC2 */                                     //// Включение и калибровка АЦП2
  ADC_Cmd(ADC2, ENABLE);

  /* Enable ADC2 reset calibaration register */   
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC2 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));

  /* Start ADC2 calibaration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC2 calibration */
  while(ADC_GetCalibrationStatus(ADC2));

TIM_Cmd(TIM3, ENABLE);                                        //// Запуск АЦП 
}



void ADC_EMS_4_Start(uint16_t Channel){

  
 TOCHEK=12;  
 K=4; 
  
  
TIM_Cmd(TIM3, DISABLE);   
  
  
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructur;  
TIM_TimeBaseStructInit(&TIM_TimeBaseStructur);             /// настройка TIM3 для срабатывания АЦП
TIM_TimeBaseStructur.TIM_Period = 500;                     ///48 kHz
TIM_TimeBaseStructur.TIM_Prescaler = 3;       
TIM_TimeBaseStructur.TIM_ClockDivision = 0;    
TIM_TimeBaseStructur.TIM_CounterMode = TIM_CounterMode_Up;  
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructur);
TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);


DMA_InitTypeDef DMA_InitStructure;                                        /// конфигурация DMA1_Channel1 для работы АЦП
DMA_DeInit(DMA1_Channel1);
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;     /// Адрес регистра данных АЦП
DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_RES;                /// Адрес регистра массива для записи данных
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
DMA_InitStructure.DMA_BufferSize = 1;                                     /// Для 8 кГц массив из 6
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
DMA_Init(DMA1_Channel1, &DMA_InitStructure);
DMA_Cmd(DMA1_Channel1, ENABLE);




ADC_InitTypeDef ADC_InitStruct;                                         /// Конфигурация АЦП 1
ADC_InitStruct.ADC_Mode = ADC_Mode_FastInterl;
ADC_InitStruct.ADC_ScanConvMode = DISABLE;
ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;     /// Запуск по Таймеру Т3
ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStruct.ADC_NbrOfChannel = 1;
ADC_Init(ADC1, &ADC_InitStruct);



 switch (Channel){
// case 0: ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_13Cycles5);  break;
 case 1: ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);  break;
 case 2: ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_13Cycles5);  break;
 case 3: ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_13Cycles5);  break;
 case 4: ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_13Cycles5);  break;
 case 5: ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5);  break;
 case 6: ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_13Cycles5);  break;
 case 7: ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_13Cycles5);  break;
 case 8: ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_13Cycles5);  break;
  default: ;
 }




ADC_DMACmd(ADC1, ENABLE);
ADC_ExternalTrigConvCmd(ADC1, ENABLE);
NVIC_EnableIRQ(ADC1_2_IRQn);  /// ADC1_2 NVIC 
ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);                                  /// прерывание по завершению АЦП


ADC_InitStruct.ADC_Mode = ADC_Mode_FastInterl;                           /// Конфигурация АЦП 2
ADC_InitStruct.ADC_ScanConvMode = DISABLE;
ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStruct.ADC_NbrOfChannel = 1;
ADC_Init(ADC2, &ADC_InitStruct);
ADC_RegularChannelConfig(ADC2, ADC_Channel_14, 1, ADC_SampleTime_13Cycles5);
ADC_DMACmd(ADC2, ENABLE);
ADC_ExternalTrigConvCmd(ADC2, ENABLE);
  
  ADC_Cmd(ADC1, ENABLE);                               //// Включение и калибровка АЦП1
  
 /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));  
  
/* Enable ADC2 */                                     //// Включение и калибровка АЦП2
  ADC_Cmd(ADC2, ENABLE);

  /* Enable ADC2 reset calibaration register */   
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC2 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));

  /* Start ADC2 calibaration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC2 calibration */
  while(ADC_GetCalibrationStatus(ADC2));

TIM_Cmd(TIM3, ENABLE);                                        //// Запуск АЦП 
}




void ADC_EMS_2_Start(uint16_t Channel){

 TOCHEK=24;  
 K=6; 
  
  
  
TIM_Cmd(TIM3, DISABLE);   
  
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructur;  
TIM_TimeBaseStructInit(&TIM_TimeBaseStructur);             /// настройка TIM3 для срабатывания АЦП
TIM_TimeBaseStructur.TIM_Period = 500;                     ///48 kHz
TIM_TimeBaseStructur.TIM_Prescaler = 3;       
TIM_TimeBaseStructur.TIM_ClockDivision = 0;    
TIM_TimeBaseStructur.TIM_CounterMode = TIM_CounterMode_Up;  
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructur);
TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);


DMA_InitTypeDef DMA_InitStructure;                                        /// конфигурация DMA1_Channel1 для работы АЦП
DMA_DeInit(DMA1_Channel1);
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;     /// Адрес регистра данных АЦП
DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_RES;                /// Адрес регистра массива для записи данных
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
DMA_InitStructure.DMA_BufferSize = 1;                                     /// Для 8 кГц массив из 6
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
DMA_Init(DMA1_Channel1, &DMA_InitStructure);
DMA_Cmd(DMA1_Channel1, ENABLE);




ADC_InitTypeDef ADC_InitStruct;                                         /// Конфигурация АЦП 1
ADC_InitStruct.ADC_Mode = ADC_Mode_FastInterl;
ADC_InitStruct.ADC_ScanConvMode = DISABLE;
ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;     /// Запуск по Таймеру Т3
ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStruct.ADC_NbrOfChannel = 1;
ADC_Init(ADC1, &ADC_InitStruct);


 switch (Channel){
 //case 0: ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_13Cycles5);  break;
 case 1: ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);  break;
 case 2: ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_13Cycles5);  break;
 case 3: ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_13Cycles5);  break;
 case 4: ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_13Cycles5);  break;
 case 5: ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5);  break;
 case 6: ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_13Cycles5);  break;
 case 7: ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_13Cycles5);  break;
 case 8: ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_13Cycles5);  break;
  default: ;
 }

ADC_DMACmd(ADC1, ENABLE);
ADC_ExternalTrigConvCmd(ADC1, ENABLE);
NVIC_EnableIRQ(ADC1_2_IRQn);  /// ADC1_2 NVIC 
ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);                                  /// прерывание по завершению АЦП


ADC_InitStruct.ADC_Mode = ADC_Mode_FastInterl;                           /// Конфигурация АЦП 2
ADC_InitStruct.ADC_ScanConvMode = DISABLE;
ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStruct.ADC_NbrOfChannel = 1;
ADC_Init(ADC2, &ADC_InitStruct);
ADC_RegularChannelConfig(ADC2, ADC_Channel_14, 1, ADC_SampleTime_13Cycles5);
ADC_DMACmd(ADC2, ENABLE);
ADC_ExternalTrigConvCmd(ADC2, ENABLE);
  
  ADC_Cmd(ADC1, ENABLE);                               //// Включение и калибровка АЦП1
  
 /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));  
  
/* Enable ADC2 */                                     //// Включение и калибровка АЦП2
  ADC_Cmd(ADC2, ENABLE);

  /* Enable ADC2 reset calibaration register */   
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC2 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));

  /* Start ADC2 calibaration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC2 calibration */
  while(ADC_GetCalibrationStatus(ADC2));

TIM_Cmd(TIM3, ENABLE);                                        //// Запуск АЦП 
}