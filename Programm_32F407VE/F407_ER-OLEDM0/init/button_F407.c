#include "button_F407.h"

Status_t Init_Button(uint16_t _return_Time)
{
		if(Enable_RCC_Button() == TRUE)
		{
			return_Time = _return_Time;
			
			Config_Button();
		}

    return FALSE;
}

Status_t Enable_RCC_Button()
{
    ENABLE_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOEEN);

    if(READ_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOEEN))
    {
        return TRUE;
    }

		return FALSE;
}

Status_t Config_Button()
{ /*K1=3 K0=4*/
		DISABLE_BIT(GPIOE->MODER,(GPIO_MODER_MODER3|GPIO_MODER_MODER4));
		ENABLE_BIT(GPIOE->PUPDR,(GPIO_PUPDR_PUPD3_0|GPIO_PUPDR_PUPD4_0));
		ENABLE_BIT(GPIOE->ODR,(GPIO_ODR_OD3|GPIO_ODR_OD4));

		return TRUE;
}

Status_t Press_Button_K1()
{
			if((!READ_BIT(GPIOE->IDR,GPIO_IDR_ID3)) & (trig_status_Button_K1 == FALSE))
			{
				trig_status_Button_K1 = TRUE;
			}
			
			if((READ_BIT(GPIOE->IDR,GPIO_IDR_ID3)))
			{
				if(trig_status_Button_K1 == TRUE)
				{
					is_Iteration_Button_K1++;
					
						if(is_Iteration_Button_K1 > return_Time)
						{
							trig_status_Button_K1 = FALSE;
							is_Iteration_Button_K1 = FALSE;
						}
				}
					
			}

	return trig_status_Button_K1;
}

Status_t Press_Button_K0()
{
			if((!READ_BIT(GPIOE->IDR,GPIO_IDR_ID4)) & (trig_status_Button_K0 == FALSE))
			{
				trig_status_Button_K0 = TRUE;
			}
			
			if((READ_BIT(GPIOE->IDR,GPIO_IDR_ID4)))
			{
				if(trig_status_Button_K0 == TRUE)
				{
					is_Iteration_Button_K0++;
					
						if(is_Iteration_Button_K0 > return_Time)
						{
							trig_status_Button_K0 = FALSE;
							is_Iteration_Button_K0 = FALSE;
						}
				}
					
			}

	return trig_status_Button_K0;

}
