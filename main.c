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
uint16_t z=0;     //ñ÷åò÷èê äëÿ ôèëüòðà

uint8_t W;  








char C=0;

char FREQ=8;



TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructur;





uint16_t TEST=1;
#define SLEGENIE 1;
#define POISK 2;

int32_t Ufsin_mass[1000];                       /// ìàññèâ ïîñëåäíèõ 80 Udsin
int32_t Ufcos_mass[1000];                       /// ìàññèâ ïîñëåäíèõ 80 Ufsin
int64_t Sum_fsin;                          /// cóììà ìàññèâà Ufsin_mass[80]; 
int64_t Sum_fcos;                          /// ñóììà ìàññèâà Ufcos_mass[80];
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
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          /// ïðåðûâàíèå ïî ïðèåìó USART


  


SINE_2_EMS();  
 
SPI_EMS_Init();

ADC_EMS_2_Start(1);

COMP_init();

TIM_TimeBaseStructInit(&TIM_TimeBaseStructur);             /// íàñòðîéêà TIM2 äëÿ ïåðåäà÷è äàííûõ è ïåðåêëþ÷åíèÿ èçëó÷àþùèõ ýëåêòðîäîâ
TIM_TimeBaseStructur.TIM_Period = 1152;                     ///6.25 Ãö
TIM_TimeBaseStructur.TIM_Prescaler = 10000;       
TIM_TimeBaseStructur.TIM_ClockDivision = 0;    
TIM_TimeBaseStructur.TIM_CounterMode = TIM_CounterMode_Up;  
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructur);
NVIC_EnableIRQ(TIM2_IRQn);    /// TIM2
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 



TIM_Cmd(TIM2, ENABLE);                                        //// Çàïóñê òàéìåðà äëÿ ïåðåäà÷è äàííûõ è ïåðåêëþ÷åíèÿ ýëåêòðîäîâ

//////////////////////////////// óñòàíîâèëè îïòðîíû âðåìåííî!

GPIO_SetBits(GPIOA, GPIO_Pin_11);
GPIO_SetBits(GPIOA, GPIO_Pin_12);
///////////////////////////////

SPI_SETUP(9, 224); //ñòàíîâèëè èçëó÷àòåëü â 1,5 Âîëüòà

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
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    // Òàêòèðîâàíèå àëüòåðíàòèâíûõ ôóíêöèé GPIO.







RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  // Âêëþ÷åíèå òàêòèðîâàíèÿ USART1.

RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); // Âêëþ÷åíèå SPI2.

RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // Âêëþ÷åíèå òàêòèðîâàíèÿ ADC1.
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);    // Âêëþ÷åíèå òàêòèðîâàíèÿ ADC2.

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    // Âêëþ÷åíèå òàêòèðîâàíèÿ TIM3 äëÿ ñðàáàòûâàíèÿ ÀÖÏ.
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);    // Âêëþ÷åíèå òàêòèðîâàíèÿ TIM7 äëÿ ãåíåðàöèè ñèíóñà.
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    // Âêëþ÷åíèå òàêòèðîâàíèÿ TIM2 äëÿ ïåðåäà÷è äàííûõ è ïåðåêëþ÷åíèÿ èçëó÷àþùèõ ýëåêòðîäîâ.
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);    // Âêëþ÷åíèå òàêòèðîâàíèÿ TIM6 äëÿ ôîðìèðîâàíèÿ êîìïåíñàöèîííîãî ñèãíàëà.




RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);        // Âêëþ÷åíèå òàêòèðîâàíèÿ DAC.

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);      // Âêëþ÷åíèå òàêòèðîâàíèÿ DMA.
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);      // Âêëþ÷åíèå òàêòèðîâàíèÿ DMA 2.
  
 
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
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;           // Âêëþ÷åíèå âûõîäà DAC1 è DAC2
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





GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5;                 // ADC2 íå ðàáîòàåò!!!!!!!!!!!!!!!!!!!
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
GPIO_Init(GPIOC,&GPIO_InitStruct);


GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_12;                 // îïòðîí
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOA,&GPIO_InitStruct);


GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_11;                 // îïòðîí
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

  if((COMMAND==102||COMMAND==101||COMMAND==100||COMMAND==99)&&(C==0)){                                   //ñòíàäàðòíûå êîìàíäû ÝÌÈ
    C=0;
    switch(COMMAND){
    case 102:   SINE_2_EMS();  N=0; z=0; ADC_EMS_2_Start(1);  FREQ=2; break;
    case 101:   SINE_8_EMS();  N=0; z=0; ADC_EMS_8_Start(1);  FREQ=8; break;
    case 100:   SINE_4_EMS();  N=0; z=0; ADC_EMS_4_Start(1);  FREQ=4; break;
    case 99: COMP_FLAG=1; break;                                                                 //óñòàíîâèëè ôëàã êîìïåíñàöèè
    };
    
    
    
  }
  
///////////////////////////////////////////////////// 
  
  else { // äîïîëíèòåëüíûå êîìàíäû ÝÌÈ ïðèíèìàåì è ñ÷èòàåì áàéòû
 
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
  
  if(COMP_FLAG==0){         //êîìïåíñàöèè íåò îáû÷íàÿ ðàáîòà 
ADC_ARRAY[N]=ADC_RES;

if(i<(K)){             //îïðåäåëÿåì W ñìåùåíèå íà 90
W=i+(TOCHEK-K);
  }
  else {W=i-(K);};

///////////////////////////////////////////////////////////////////// âûðàáîòêà êîìïåíñèðóþùåãî ñèãíàëà
//COMP=((ADC_ARRAY[W]>>16))/4+((ADC_ARRAY[N]>>16))/4;
int16_t QQ=(ADC_ARRAY[N]>>16)-VREF;
int16_t QQQ=(ADC_ARRAY[W]>>16)-VREF;
COMP=(((QQ)*((K1))))+(((QQQ)*K2))+VREF;


DAC_SetChannel2Data(DAC_Align_12b_R, COMP); 
///////////////////////////////////////////////////////////////

N=N+1;




if(N==TOCHEK){                                             //îáðàáîòêà ïîñëå çàïîëíåíèÿ ìàññèâà îòñ÷åòàìè(òî÷êè)
N=0;
 

EMS_MATH_1(); //ôóíêöèÿ äåòåêòîðà ðàñ÷èòûâàåò Udsin Udcos íè÷åãî íå âîçâðàùàåò, ñîõðàíÿåò èõ êàê ãëîáàëüíûå ïåðåìåííûå

                                                                 ///Ôèëüòð Í×



z=z+1;                                                                        /// ñ÷èòàåì z

Sum_fsin+=Udsin;
Sum_fcos+=Udcos;  

if(z==300) {           //ôèëüòð ÔÍ×
  
z=0;



Ufsin=(Sum_fsin>>8)/128;
Ufcos=(Sum_fcos>>8)/128;

Sum_fsin=0;
Sum_fcos=0;

/*
if(COMP_FLAG){
COMP_FLAG=0;  
K1=Ufsin/11000; ///11000 ìàêñèìàëüíûé îòêëèê ïðè âõîäíîì ñèãíàëå àíàëîãè÷íîì îïîðíîìó äîáàâèòü â êîìïåíñàöèþ!!!!!!!
K2=Ufcos/11000;
};
*/

};
};
  }else{     //åñëè ôëàã êîìïåíñàöèè 
  
 ADC_ARRAY[N]=ADC_RES;
 N++;
 
if(N==ARU_SAMPLES){ //ñîáðàëè ìàññèâ èç 240 òî÷åê
 N=0; 
for(i=0; i<ARU_SAMPLES; i++) {
ADC_ARRAY[i]=(ADC_ARRAY[i] & 0xFFFF);   //signal        âûäåëèëè 2-îé àöï èç îáùèõ äàííûõ ///
};

MINIMUM=ADC_ARRAY[0];

for(i=1;i<ARU_SAMPLES;i++){          ///íàøëè ìèíèìóì ñèíóñîèäû ( ïî ñóòè ìàêñèìàëüíîå çíà÷åíèå îòðèöàòåëüíîé àìïëèòóäû)
if((MINIMUM)>(ADC_ARRAY[i])){
MINIMUM=ADC_ARRAY[i];  
};
};

if(MINIMUM<VOLTAGE_1){ // îò 1 Â ñíèçó 1Â -1241
RESISTOR=SPI_MORE(1);  //óìåíüøàåì êîýôôèöèåíò óñèëåíèÿ ïðèåìíûõ ÎÓ
if(RESISTOR==1023){
COMP_FLAG=0;            // ìèíèìàëüíîå óñèëåíèå, ñáðîñèëè ôëàã  
};
}else{
COMP_FLAG=0;            // àðó ãîòîâ, ñáðîñèëè ôëàã
};
   
 };
    
  };


}

void TIM2_IRQHandler (void){ ///////////òàéìåð ïåðåäà÷è äàííûõ
  
 TIM2->SR &= ~TIM_SR_UIF;                                                    //////// ñáðîñ ôëàãà ïðåðûâàíèÿ 

 
 
if(!COMP_FLAG){ //ïåðåäà÷à òîëüêî ïðè îòñóòñâèè êîìïåíñàöèè
  
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
