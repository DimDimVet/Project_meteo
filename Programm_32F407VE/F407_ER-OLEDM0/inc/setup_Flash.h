#ifndef SETUP_FLASH_H_
#define SETUP_FLASH_H_

/*includes*/
#include "flash_F407.h"
#include "clock_RTC.h"
#include "delay.h"

/*defaines*/
#define ADD_HOURS 0x08010000
#define ADD_MINUTES 0x08020000
#define ADD_DATE 0x08030000
#define ADD_MONTH 0x08040000
#define ADD_YEAR 0x08050000
#define SIZE 5
/*var*/
static uint32_t addr_time_date[SIZE] = {ADD_HOURS,ADD_MINUTES,ADD_DATE,ADD_MONTH,ADD_YEAR};
static uint8_t arr_time_date[SIZE];
/*structures*/

/*func*/
void Set_Flash_Data(RTC_t *time_date);
void Get_Flash_Data(RTC_t *time_date);

#endif
