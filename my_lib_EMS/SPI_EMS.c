#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "SPI_EMS.h"



uint16_t Value=0; 
uint16_t Value_1=0; 
uint16_t Value_2=0; 
uint16_t Value_3=0; 
uint16_t Value_4=0;
uint16_t Value_9=0;
uint16_t A; 





void SPI_EMS_Init(void){
  
  
 GPIO_InitTypeDef GPIO_InitStructure;             //SPI2





GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                    ////////Chip Select CS9
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOB, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                    ///CS1
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOA, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                    ///CS4
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOC, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                    ///CS3
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOC, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                    ///CS2
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOC, &GPIO_InitStructure);



GPIO_SetBits( GPIOB, GPIO_Pin_12);            /// Начальный высокий уровень(сброс кристалла)
GPIO_SetBits( GPIOA, GPIO_Pin_8);
GPIO_SetBits( GPIOC, GPIO_Pin_7); 
GPIO_SetBits( GPIOC, GPIO_Pin_8); 
GPIO_SetBits( GPIOC, GPIO_Pin_9);

SPI_I2S_DeInit(SPI2); // Resets SPI2  
SPI_InitTypeDef SPI_InitStructure;
SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;             //  clock polarity
SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;          // clock phase
SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
SPI_InitStructure.SPI_CRCPolynomial = 7;
SPI_Init(SPI2, &SPI_InitStructure);




/* Enable SPI_MASTER NSS output for master mode */
SPI_SSOutputCmd(SPI2, DISABLE);

// Enable the SPI 
SPI_Cmd(SPI2, ENABLE);

  
}




uint16_t SPI_MORE(uint16_t CS){

__IO uint32_t nCount;

switch (CS){
case 1: Value_1=Value_1+1; Value=Value_1; break; //независимая регулировка
case 2: Value_2=Value_2+1; Value=Value_2; break;
case 3: Value_3=Value_3+1; Value=Value_3; break;
case 4: Value_4=Value_4+1; Value=Value_4; break;
case 9: Value_9=Value_9+1; Value=Value_9; break;
default: ;
}

 
 switch (CS){
 case 0:
 case 1: GPIO_ResetBits( GPIOA, GPIO_Pin_8); break;
 case 2: GPIO_ResetBits( GPIOC, GPIO_Pin_9); break;
 case 3: GPIO_ResetBits( GPIOC, GPIO_Pin_8); break;
 case 4: GPIO_ResetBits( GPIOC, GPIO_Pin_7); break;
 case 5:
 case 6:
 case 7:
 case 8:
 case 9: GPIO_ResetBits( GPIOB, GPIO_Pin_12); break;
 default: ;
 }
 
 nCount=1000;
for(; nCount != 0; nCount--);





 


while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);    //посылаем прини предыдущее значение
SPI_I2S_SendData(SPI2, 0x00);
while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
A= SPI_I2S_ReceiveData(SPI2);



while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);    //посылаем прини предыдущее значение
SPI_I2S_SendData(SPI2, ((Value>>2)&0xFF));
while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
A= SPI_I2S_ReceiveData(SPI2);




while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);    //посылаем прини предыдущее значение
SPI_I2S_SendData(SPI2, ((Value<<6)&0xFF));
while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
A= SPI_I2S_ReceiveData(SPI2);






nCount=1000;
for(; nCount != 0; nCount--);



  
switch (CS){
 case 0:
 case 1: GPIO_SetBits( GPIOA, GPIO_Pin_8); break;
 case 2: GPIO_SetBits( GPIOC, GPIO_Pin_9); break;
 case 3: GPIO_SetBits( GPIOC, GPIO_Pin_8); break;
 case 4: GPIO_SetBits( GPIOC, GPIO_Pin_7); break;
 case 5:
 case 6:
 case 7:
 case 8:
 case 9: GPIO_SetBits( GPIOB, GPIO_Pin_12); break;
 default: ;
 

 }  
  return Value; 
}









uint16_t SPI_LESS(uint16_t CS){
  
  


__IO uint32_t nCount;


switch (CS){
case 1: Value_1=Value_1-1; Value=Value_1; break;
case 2: Value_2=Value_2-1; Value=Value_2; break;
case 3: Value_3=Value_3-1; Value=Value_3; break;
case 4: Value_4=Value_4-1; Value=Value_4; break;
case 9: Value_9=Value_9-1; Value=Value_9; break;
default: ;
}



 
 switch (CS){
 case 0:
 case 1:  GPIO_ResetBits( GPIOA, GPIO_Pin_8); break;
 case 2: GPIO_ResetBits( GPIOC, GPIO_Pin_9); break;
 case 3: GPIO_ResetBits( GPIOC, GPIO_Pin_8); break;
 case 4: GPIO_ResetBits( GPIOC, GPIO_Pin_7); break;
 case 5:
 case 6:
 case 7:
 case 8:
 case 9: GPIO_ResetBits( GPIOB, GPIO_Pin_12); break;
 default: ;
 }
 
 
for(nCount=0; nCount <1000; nCount++);





 


while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);    //посылаем прини предыдущее значение
SPI_I2S_SendData(SPI2, 0x00);
while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
A= SPI_I2S_ReceiveData(SPI2);



while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);    //посылаем прини предыдущее значение
SPI_I2S_SendData(SPI2, ((Value>>2)&0xFF));
while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
A= SPI_I2S_ReceiveData(SPI2);




while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);    //посылаем прини предыдущее значение
SPI_I2S_SendData(SPI2, ((Value<<6)&0xFF));
while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
A= SPI_I2S_ReceiveData(SPI2);







for(nCount=0; nCount <1000; nCount++);



  
switch (CS){
 case 0:
 case 1: GPIO_SetBits( GPIOA, GPIO_Pin_8); break;
 case 2: GPIO_SetBits( GPIOC, GPIO_Pin_9); break;
 case 3:GPIO_SetBits( GPIOC, GPIO_Pin_8); break;
 case 4: GPIO_SetBits( GPIOC, GPIO_Pin_7); break;
 case 5:
 case 6:
 case 7:
 case 8:
 case 9: GPIO_SetBits( GPIOB, GPIO_Pin_12); break;
 default: ;
 
 }  


 return  Value; 
}




void SPI_SETUP(uint16_t CS, uint16_t DIGIT){
  
 uint32_t nCount;



 
 switch (CS){
 case 0:
 case 1:  GPIO_ResetBits( GPIOA, GPIO_Pin_8); break;
 case 2: GPIO_ResetBits( GPIOC, GPIO_Pin_9); break;
 case 3: GPIO_ResetBits( GPIOC, GPIO_Pin_8); break;
 case 4: GPIO_ResetBits( GPIOC, GPIO_Pin_7); break;
 case 5:
 case 6:
 case 7:
 case 8:
 case 9: GPIO_ResetBits( GPIOB, GPIO_Pin_12); break;
 default: ;
 };
 
 
for(nCount=0; nCount <1000; nCount++);





 


while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);    //посылаем прини предыдущее значение
SPI_I2S_SendData(SPI2, 0x00);
while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
A= SPI_I2S_ReceiveData(SPI2);



while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);    //посылаем прини предыдущее значение
SPI_I2S_SendData(SPI2, ((DIGIT>>2)&0xFF));
while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
A= SPI_I2S_ReceiveData(SPI2);




while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);    //посылаем прини предыдущее значение
SPI_I2S_SendData(SPI2, ((DIGIT<<6)&0xFF));
while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
A= SPI_I2S_ReceiveData(SPI2);







for(nCount=0; nCount <1000; nCount++);



  
switch (CS){
 case 0:
 case 1: GPIO_SetBits( GPIOA, GPIO_Pin_8); break;
 case 2: GPIO_SetBits( GPIOC, GPIO_Pin_9); break;
 case 3:GPIO_SetBits( GPIOC, GPIO_Pin_8); break;
 case 4: GPIO_SetBits( GPIOC, GPIO_Pin_7); break;
 case 5:
 case 6:
 case 7:
 case 8:
 case 9: GPIO_SetBits( GPIOB, GPIO_Pin_12); break;
 default: ;

}; 
  
  
  
  
}