#ifndef SETUP_TIMER_DATA_H_
#define SETUP_TIMER_DATA_H_

/*includes*/
#include "stdio.h"
#include "HEHAL.h"
#include "init_LED.h"
#include "button_F407.h"
#include "clock_RTC.h"
#include "ssd1306.h"
#include "setup_Flash.h"

/*defaines*/

/*var*/

/*structures*/
typedef enum
{
	sw_Norm = 0,
	sw_Hours = 1,
	sw_Minutes = 2,
	sw_Date = 3,
	sw_Month = 4,
	sw_Year = 5,
	sw_count = sw_Year
}Switch_t;

/*func*/
void UpDates_Button(void);

void Update_RTC(RTC_t *current_time_date, Switch_t *sw_date_time);
Status_t Update_Time_Date(RTC_t *rtc, ssd1306_t *str_SSD1306, RTC_t *current_time_date);
void Update_SSD_Time_Date(ssd1306_t *str_SSD1306, RTC_t *current_time_date, uint16_t time);

#endif
