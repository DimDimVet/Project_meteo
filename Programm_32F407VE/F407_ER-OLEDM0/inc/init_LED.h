#ifndef INIT_LED
#define INIT_LED

#include "stm32f4xx.h"
#include "HEHAL.h"

void Init_LED(void);
void Enable_RCC_LED(void);
void Config_LED(void);
void LED6(Status_t isStatus);
void LED7(Status_t isStatus);

#endif
