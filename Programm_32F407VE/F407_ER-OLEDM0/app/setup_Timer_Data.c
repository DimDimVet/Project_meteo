#include "setup_Timer_Data.h"

Status_t isRun_setup = FALSE;
uint8_t dec_max_count = FALSE;
uint8_t dec_count = FALSE;
uint16_t count_update = FALSE;

int isK0 = FALSE;
int isK1 = FALSE;

Switch_t sw_date_time = sw_Norm;

void UpDates_Button()
{
				if (Press_Button_K1() != FALSE)
				{
					if(isK1 == FALSE)
					{
						isK1 = TRUE;

						sw_date_time++;
						
						if(isRun_setup == TRUE)
						{
							isRun_setup = FALSE;
						}
						
						if(sw_date_time > sw_count)
						{
							sw_date_time = sw_Norm;
							isRun_setup = TRUE;
						}
						
					}

					LED6(TRUE);
				}
				else
				{
					if(isK1 == TRUE)
					{
						isK1 = FALSE;
					}
					
					LED6(FALSE);
				}
				
				if (Press_Button_K0() != FALSE)
				{
					if(isK0 == FALSE)
					{
						isK0 = TRUE;
						
						dec_count++;
						
						if(dec_count > (dec_max_count))
						{
							dec_count = FALSE;
						}
					}
					
					LED7(TRUE);
				}
				else
				{
					if(isK0 == TRUE)
					{
						isK0 = FALSE;
					}
					
					LED7(FALSE);
				}
}

Status_t Update_Time_Date(RTC_t *rtc, ssd1306_t *str_SSD1306, RTC_t *current_time_date)
{
	UpDates_Button();
	
	Status_t temp_status;
	
	RTC_GetDate(rtc);
	
	RTC_GetTime(rtc);
	
	Update_SSD_Time_Date(str_SSD1306,rtc,10);
	
	Update_RTC(rtc, &sw_date_time);
	
	temp_status = TRUE;

	return temp_status;
}

void Update_RTC(RTC_t *current_time_date, Switch_t *sw_date_time)
{
	
	switch(*sw_date_time)
								{
										case sw_Norm: 
												
												if(isRun_setup == TRUE)
												{
													isRun_setup = FALSE;
													
													Set_Flash_Data(current_time_date);
												}
		
												break;
												
										case sw_Hours: 
											
												dec_max_count = 23;
												
												if(isRun_setup == FALSE)
												{
													isRun_setup = TRUE;
													dec_count = current_time_date->time.Hours;
												}

												if(current_time_date->time.Hours != dec_count)
												{
													current_time_date->time.Hours = dec_count;
													RTC_INIT(current_time_date);
												}
										
												break;
												
										case sw_Minutes: 
											
												dec_max_count = 60;
												
												if(isRun_setup == FALSE)
												{
													isRun_setup = TRUE;
													dec_count = current_time_date->time.Minutes;
												}

												if(current_time_date->time.Minutes != dec_count)
												{
													current_time_date->time.Minutes = dec_count;
													RTC_INIT(current_time_date);
												}

												break;
										case sw_Date: 
												dec_max_count = 31;
												
												if(isRun_setup == FALSE)
												{
													isRun_setup = TRUE;
													dec_count = current_time_date->date.Date;
												}

												if(current_time_date->date.Date != dec_count)
												{
													current_time_date->date.Date = dec_count;
													RTC_INIT(current_time_date);
												}
												
												break;
												
										case sw_Month: 
												dec_max_count = 12;
												
												if(isRun_setup == FALSE)
												{
													isRun_setup = TRUE;
													dec_count = current_time_date->date.Month;
												}

												if(current_time_date->date.Month != dec_count)
												{
													current_time_date->date.Month = dec_count;
													RTC_INIT(current_time_date);
												}
										
												break;
										
										case sw_Year: 
												dec_max_count = 35;
												
												if(isRun_setup == FALSE)
												{
													isRun_setup = TRUE;
													dec_count = current_time_date->date.Year;
												}

												if(current_time_date->date.Year != dec_count)
												{
													current_time_date->date.Year = dec_count;
													RTC_INIT(current_time_date);
												}
												
												break;
																
										default: 

												break;
								}
}

void Update_SSD_Time_Date(ssd1306_t *str_SSD1306, RTC_t *current_time_date, uint16_t time)
{
	char buff_str[20];
	
				  switch(sw_date_time)
								{
										case sw_Norm: 
											
												        sprintf(buff_str, "Time: %02d:%02d:%02d", current_time_date->time.Hours, current_time_date->time.Minutes, current_time_date->time.Seconds);
																SSD1306_GotoXY(str_SSD1306, 0, 00);
																SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
										
																sprintf(buff_str, "Date: %02d:%02d:%02d", current_time_date->date.Date, current_time_date->date.Month, current_time_date->date.Year);
																SSD1306_GotoXY(str_SSD1306, 0, 10);
																SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
															
												break;
												
										case sw_Hours: 
																
																count_update++;
										
																if(count_update < time)
																{
																	sprintf(buff_str, "Time:   :%02d:%02d", current_time_date->time.Minutes, current_time_date->time.Seconds);
																	SSD1306_GotoXY(str_SSD1306, 0, 00);
																	SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
																}
																else if((count_update < (time * 2)) & (count_update > time))
																{
																	sprintf(buff_str, "Time: %02d:%02d:%02d", current_time_date->time.Hours, current_time_date->time.Minutes, current_time_date->time.Seconds);
																	SSD1306_GotoXY(str_SSD1306, 0, 00);
																	SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);

																}
																else if(count_update > (time * 2))
																{
																	count_update = 0;
																}

												break;
												
										case sw_Minutes: 
										
																count_update++;
										
																if(count_update < time)
																{
																	sprintf(buff_str, "Time: %02d:  :%02d", current_time_date->time.Hours, current_time_date->time.Seconds);
																	SSD1306_GotoXY(str_SSD1306, 0, 00);
																	SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
																}
																else if((count_update < (time * 2)) & (count_update > time))
																{
																	sprintf(buff_str, "Time: %02d:%02d:%02d", current_time_date->time.Hours, current_time_date->time.Minutes, current_time_date->time.Seconds);
																	SSD1306_GotoXY(str_SSD1306, 0, 00);
																	SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
																}
																else if(count_update > (time * 2))
																{
																	count_update = 0;
																}

												break;
										case sw_Date: 
											
																count_update++;
																if(count_update < time)
																{
																	sprintf(buff_str, "Date:   :%02d:%02d", current_time_date->date.Month, current_time_date->date.Year);
																	SSD1306_GotoXY(str_SSD1306, 0, 10);
																	SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
																}
																else if((count_update < (time * 2)) & (count_update > time))
																{
																	sprintf(buff_str, "Date: %02d:%02d:%02d", current_time_date->date.Date, current_time_date->date.Month, current_time_date->date.Year);
																	SSD1306_GotoXY(str_SSD1306, 0, 10);
																	SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
																}
																else if(count_update > (time * 2))
																{
																	count_update = 0;
																}
															
												break;
												
										case sw_Month: 
										
																count_update++;
																if(count_update < time)
																{
																	sprintf(buff_str, "Date: %02d:  :%02d", current_time_date->date.Date, current_time_date->date.Year);
																	SSD1306_GotoXY(str_SSD1306, 0, 10);
																	SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
																}
																else if((count_update < (time * 2)) & (count_update > time))
																{
																	sprintf(buff_str, "Date: %02d:%02d:%02d", current_time_date->date.Date, current_time_date->date.Month, current_time_date->date.Year);
																	SSD1306_GotoXY(str_SSD1306, 0, 10);
																	SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
																}
																else if(count_update > (time * 2))
																{
																	count_update = 0;
																}
												break;
										
										case sw_Year: 
										
																count_update++;
																if(count_update < time)
																{
																	sprintf(buff_str, "Date: %02d:%02d:  ", current_time_date->date.Date, current_time_date->date.Month);
																	SSD1306_GotoXY(str_SSD1306, 0, 10);
																	SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
																}
																else if((count_update < (time * 2)) & (count_update > time))
																{
																	sprintf(buff_str, "Date: %02d:%02d:%02d", current_time_date->date.Date, current_time_date->date.Month, current_time_date->date.Year);
																	SSD1306_GotoXY(str_SSD1306, 0, 10);
																	SSD1306_Puts(str_SSD1306, buff_str, &Font_7x10, 1);
																}
																else if(count_update > (time * 2))
																{
																	count_update = 0;
																}
												break;
																
										default: 

																SSD1306_GotoXY(str_SSD1306, 0, 00);
																SSD1306_Puts(str_SSD1306, "Time:", &Font_7x10, 1);
										
																SSD1306_GotoXY(str_SSD1306, 0, 10);
																SSD1306_Puts(str_SSD1306, "Date:", &Font_7x10, 1);

												break;
								}

	SSD1306_UpdateScreen(str_SSD1306);

}
