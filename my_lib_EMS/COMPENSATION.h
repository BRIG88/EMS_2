#include "stm32f10x.h"

extern int16_t COMP;
extern long float K1;
extern long float K2;
extern char COMP_FLAG;
extern uint16_t MINIMUM;
void COMP_init(void);