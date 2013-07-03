/**
  ******************************************************************************
  * @file    DAC/OneChannelDMA_Escalator/main.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main program body.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "math.h"
#include "ADC_EMS.h"
#include "SPI_EMS.h"
#include "SINE_EMS.h"
#include "MATH_EMS.h"
#include "COMPENSATION.h"





/* Init Structure definition */


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



#define VOLTAGE_1 1241
#define ARU_SAMPLES 240
#define VREF 2048

//const uint16_t Sine12bit[1] ={3580};
uint16_t RESISTOR; 
uint16_t COMMAND; 
uint16_t COMMAND1; 
uint16_t COMMAND2; 
uint16_t REGIM;
uint16_t i=0;
uint16_t N=0;
uint16_t z=0;     //счетчик для фильтра

uint8_t W;  








char C=0;

char FREQ=8;



TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructur;





uint16_t TEST=0;
#define SLEGENIE 1;
#define POISK 2;

int32_t Ufsin_mass[1000];                       /// массив последних 80 Udsin
int32_t Ufcos_mass[1000];                       /// массив последних 80 Ufsin
int64_t Sum_fsin;                          /// cумма массива Ufsin_mass[80]; 
int64_t Sum_fcos;                          /// сумма массива Ufcos_mass[80];
int32_t Ufsin;                             /// Sum_fsin/64;
int32_t Ufcos;                             /// Sum_fcos/64;


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void Delay(__IO uint32_t nCount);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
 


  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     

  /* System Clocks Configuration */
  RCC_Configuration();   

  /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically 
     connected to the DAC converter. In order to avoid parasitic consumption, 
     the GPIO pin should be configured in analog */
  GPIO_Configuration();

  
   
  

 
   





USART_InitTypeDef USART_InitStruct;
USART_InitStruct.USART_BaudRate = 57600;
USART_InitStruct.USART_WordLength = USART_WordLength_8b;
USART_InitStruct.USART_StopBits = USART_StopBits_1;
USART_InitStruct.USART_Parity = USART_Parity_No ;
USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_Init(USART1,&USART_InitStruct);
USART_Cmd(USART1, ENABLE);

NVIC_EnableIRQ(USART1_IRQn);
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          /// прерывание по приему USART


  


SINE_2_EMS();  
 
SPI_EMS_Init();

ADC_EMS_2_Start(1);

COMP_init();

TIM_TimeBaseStructInit(&TIM_TimeBaseStructur);             /// настройка TIM2 для передачи данных и переключения излучающих электродов
TIM_TimeBaseStructur.TIM_Period = 1152;                     ///6.25 Гц
TIM_TimeBaseStructur.TIM_Prescaler = 10000;       
TIM_TimeBaseStructur.TIM_ClockDivision = 0;    
TIM_TimeBaseStructur.TIM_CounterMode = TIM_CounterMode_Up;  
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructur);
NVIC_EnableIRQ(TIM2_IRQn);    /// TIM2
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 



TIM_Cmd(TIM2, ENABLE);                                        //// Запуск таймера для передачи данных и переключения электродов

//////////////////////////////// установили оптроны временно!

GPIO_SetBits(GPIOA, GPIO_Pin_11);
GPIO_SetBits(GPIOA, GPIO_Pin_12);
///////////////////////////////

SPI_SETUP(9, 224); //становили излучатель в 1,5 Вольта

while (1)
  {
    


  


  }
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{   
  /* Enable peripheral clocks ------------------------------------------------*/

 
 


RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    // Тактирование альтернативных функций GPIO.







RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  // Включение тактирования USART1.

RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); // Включение SPI2.

RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // Включение тактирования ADC1.
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);    // Включение тактирования ADC2.

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    // Включение тактирования TIM3 для срабатывания АЦП.
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);    // Включение тактирования TIM7 для генерации синуса.
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    // Включение тактирования TIM2 для передачи данных и переключения излучающих электродов.
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);    // Включение тактирования TIM6 для формирования компенсационного сигнала.




RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);        // Включение тактирования DAC.

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);      // Включение тактирования DMA.
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);      // Включение тактирования DMA 2.
  
 
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;




////////// USART



GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_9;            /// USART
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_Init(GPIOA,&GPIO_InitStruct);



GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_10;                 ///USART
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOA,&GPIO_InitStruct);



GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_4;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;           // Включение выхода DAC1 и DAC2
GPIO_Init(GPIOA,&GPIO_InitStruct);


GPIO_InitTypeDef GPIO_InitStructure;             //SPI2

// Configure First_String_SPI pins: SCK 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; /////////3
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_Init(GPIOB, &GPIO_InitStructure);     //////B

// Configure First_String_SPI pins: MISO 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;  ////////4
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOB, &GPIO_InitStructure);     /////B

// Configure First_String_SPI pins: MOSI 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;        ////////5
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_Init(GPIOB, &GPIO_InitStructure);            ///////B


GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;                 // ADC1_0
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOA,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;                 // ADC1_1
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOA,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2;                 // ADC1_2
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOA,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3;                 // ADC1_3
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOA,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6;                 // ADC1_6
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOA,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7;                 // ADC1_7
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOA,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;                 // ADC1_8
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOB,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;                 // ADC1_9
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOB,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;                 // ADC1_10
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOC,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;                 // ADC1_11
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOC,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2;                 // ADC1_12
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOC,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3;                 // ADC1_13
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOC,&GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4;                 // ADC2_14  !!!!!!!!!!!!!
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOC,&GPIO_InitStruct);





GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5;                 // ADC2 не работает!!!!!!!!!!!!!!!!!!!
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOC,&GPIO_InitStruct);


GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_12;                 // оптрон
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOA,&GPIO_InitStruct);


GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_11;                 // оптрон
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOA,&GPIO_InitStruct);
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
    
      
    
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 


void USART1_IRQHandler (void){
 
COMMAND=USART_ReceiveData(USART1);  

USART_ClearFlag(USART1,USART_FLAG_RXNE); 

  if((COMMAND==102||COMMAND==101||COMMAND==100||COMMAND==99)&&(C==0)){                                   //стнадартные команды ЭМИ
    C=0;
    switch(COMMAND){
    case 102:   SINE_2_EMS();  N=0; z=0; ADC_EMS_2_Start(1);  FREQ=2; break;
    case 101:   SINE_8_EMS();  N=0; z=0; ADC_EMS_8_Start(1);  FREQ=8; break;
    case 100:   SINE_4_EMS();  N=0; z=0; ADC_EMS_4_Start(1);  FREQ=4; break;
    case 99: COMP_FLAG=1; break;                                                                 //установили флаг компенсации
    };
    
    
    
  }
  
///////////////////////////////////////////////////// 
  
  else { // дополнительные команды ЭМИ принимаем и считаем байты
 
C++; 
if(C==1){
  
COMMAND1=COMMAND;  
USART_ClearFlag(USART1,USART_FLAG_RXNE); 
  
};


if(C==2){
C=0;

COMMAND2=COMMAND; 
USART_ClearFlag(USART1,USART_FLAG_RXNE); 

if(COMMAND1==65){
 
  char A=0;
  
 switch (COMMAND2){
 case 48: A=9; break;  
 case 49: A=1; break;
 case 50: A=2; break;
 case 51: A=3; break;
 case 52: A=4; break;
 default: ;
 
 }   
  
  
RESISTOR=SPI_MORE(A);

USART_SendData(USART1, (A));  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
USART_SendData(USART1, (RESISTOR>>8));  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
USART_SendData(USART1, RESISTOR&0xFF);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

};



if(COMMAND1==66){
  
char A=0;
  
 switch (COMMAND2){
 case 48: A=9; break;
 case 49: A=1; break;
 case 50: A=2; break;
 case 51: A=3; break;
 case 52: A=4; break;
 default: ;  
 } 
  
RESISTOR=SPI_LESS(A);

USART_SendData(USART1, (A));  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
USART_SendData(USART1, (RESISTOR>>8));  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
USART_SendData(USART1, RESISTOR&0xFF);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

};
};
};
}






void ADC1_2_IRQHandler (void) {
  
  if(COMP_FLAG==0){         //компенсации нет обычная работа 
ADC_ARRAY[N]=ADC_RES;

if(i<(K)){             //определяем W смещение на 90
W=i+(TOCHEK-K);
  }
  else {W=i-(K);};

///////////////////////////////////////////////////////////////////// выработка компенсирующего сигнала
//COMP=((ADC_ARRAY[W]>>16))/4+((ADC_ARRAY[N]>>16))/4;
int16_t QQ=(ADC_ARRAY[N]>>16)-VREF;
int16_t QQQ=(ADC_ARRAY[W]>>16)-VREF;
COMP=(((QQ)*((K1))))+(((QQQ)*K2))+VREF;


DAC_SetChannel2Data(DAC_Align_12b_R, COMP); 
///////////////////////////////////////////////////////////////

N=N+1;




if(N==TOCHEK){                                             //обработка после заполнения массива отсчетами(точки)
N=0;
 

EMS_MATH_1(); //функция детектора расчитывает Udsin Udcos ничего не возвращает, сохраняет их как глобальные переменные

                                                                 ///Фильтр НЧ



z=z+1;                                                                        /// считаем z

Sum_fsin+=Udsin;
Sum_fcos+=Udcos;  

if(z==300) {           //фильтр ФНЧ
  
z=0;



Ufsin=(Sum_fsin>>8)/128;
Ufcos=(Sum_fcos>>8)/128;

Sum_fsin=0;
Sum_fcos=0;

/*
if(COMP_FLAG){
COMP_FLAG=0;  
K1=Ufsin/11000; ///11000 максимальный отклик при входном сигнале аналогичном опорному добавить в компенсацию!!!!!!!
K2=Ufcos/11000;
};
*/

};
};
  }else{     //если флаг компенсации 
  
 ADC_ARRAY[N]=ADC_RES;
 N++;
 
if(N==ARU_SAMPLES){ //собрали массив из 240 точек
 N=0; 
for(i=0; i<ARU_SAMPLES; i++) {
ADC_ARRAY[i]=(ADC_ARRAY[i] & 0xFFFF);   //signal        выделили 2-ой ацп из общих данных ///
};

MINIMUM=ADC_ARRAY[0];

for(i=1;i<ARU_SAMPLES;i++){          ///нашли минимум синусоиды ( по сути максимальное значение отрицательной амплитуды)
if((MINIMUM)>(ADC_ARRAY[i])){
MINIMUM=ADC_ARRAY[i];  
};
};

if(MINIMUM<VOLTAGE_1){ // от 1 В снизу 1В -1241
RESISTOR=SPI_MORE(1);  //уменьшаем коэффициент усиления приемных ОУ
if(RESISTOR==1023){
COMP_FLAG=0;            // минимальное усиление, сбросили флаг  
};
}else{
COMP_FLAG=0;            // ару готов, сбросили флаг
};
   
 };
    
  };


}

void TIM2_IRQHandler (void){ ///////////таймер передачи данных
  
 TIM2->SR &= ~TIM_SR_UIF;                                                    //////// сброс флага прерывания 

 
 
if(!COMP_FLAG){ //передача только при отсутсвии компенсации
  
USART_SendData(USART1, 0x0);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
 
USART_SendData(USART1, 230);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

USART_SendData(USART1, (Ufsin>>8));  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

USART_SendData(USART1, Ufsin);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  

USART_SendData(USART1, Ufcos>>8);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

USART_SendData(USART1, Ufcos);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  

USART_SendData(USART1, Ufsin>>8);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

USART_SendData(USART1, Ufsin);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  

USART_SendData(USART1, Ufcos>>8);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

USART_SendData(USART1, Ufcos);  
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  

USART_SendData(USART1, 0xFF);                                    ///status
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

USART_SendData(USART1, FREQ);                                   ///frequency
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  
 };
 
}






/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/