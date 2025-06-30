#ifndef BUTTON_F407_H_
#define BUTTON_F407_H_

/*includes*/
#include "stm32f4xx.h"
#include "HEHAL.h"
#include "delay.h"

/*defaines*/

	
/*var*/
static int is_Iteration_Button_K0=FALSE;
static Status_t trig_status_Button_K0=FALSE;

static int is_Iteration_Button_K1=FALSE;
static Status_t trig_status_Button_K1=FALSE;

static int return_Time = FALSE;
/*structures*/

/*func*/
Status_t Init_Button(uint16_t _return_Time);
Status_t Enable_RCC_Button(void);
Status_t Config_Button(void);
Status_t Press_Button_K1(void);
Status_t Press_Button_K0(void);
	
#endif