#include "init_LED.h"

void Init_LED()
{
    Enable_RCC_LED();
    Config_LED();
}

void Enable_RCC_LED()
{
    RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOAEN_Pos;
}

void Config_LED()
{
    GPIOA->MODER |= 0 << GPIO_MODER_MODER6_Pos;      // сброс
    GPIOA->MODER |= 1 << GPIO_MODER_MODER6_Pos;      // режим на выход
    GPIOA->OSPEEDR |= 0 << GPIO_OSPEEDR_OSPEED6_Pos; // малая скорость
    GPIOA->PUPDR |= 1 << GPIO_PUPDR_PUPD6_Pos;       // установка подтяжки к + (1)
    GPIOA->OTYPER |= 1 << GPIO_OTYPER_OT6_Pos;       // Выход с открытым стоком

    GPIOA->MODER |= 0 << GPIO_MODER_MODER7_Pos;      // сброс
    GPIOA->MODER |= 1 << GPIO_MODER_MODER7_Pos;      // режим на выход
    GPIOA->OSPEEDR |= 0 << GPIO_OSPEEDR_OSPEED7_Pos; // малая скорость
    GPIOA->PUPDR |= 1 << GPIO_PUPDR_PUPD7_Pos;       // установка подтяжки к + (1)
    GPIOA->OTYPER |= 1 << GPIO_OTYPER_OT7_Pos;       // Выход с открытым стоком
}

void LED7(Status_t isStatus)
{
	if(isStatus == FALSE)
	{
		ENABLE_BIT(GPIOA->ODR,GPIO_ODR_OD7);
		//GPIOA->ODR |= 0 << GPIO_ODR_OD7_Pos;
	}
	else 
	{
		DISABLE_BIT(GPIOA->ODR,GPIO_ODR_OD7);
		//GPIOA->ODR |= 1 << GPIO_ODR_OD7_Pos;
	}
}

void LED6(Status_t isStatus)
{
	if(isStatus == FALSE)
	{
		ENABLE_BIT(GPIOA->ODR,GPIO_ODR_OD6);
		//GPIOA->ODR |= 0 << GPIO_ODR_OD6_Pos;
	}
	else 
	{
		DISABLE_BIT(GPIOA->ODR,GPIO_ODR_OD6);
		//GPIOA->ODR |= 1 << GPIO_ODR_OD6_Pos;
	}

}
