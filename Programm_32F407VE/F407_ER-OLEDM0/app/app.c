#include "app.h"

RTC_t rtc;
uint8_t count_RTC=0;
bmp180_t str_BMP180;
ssd1306_t str_SSD1306;
uint32_t data;

void ExecutorTerminal_USART_Irq(void)
{
    USART1_ReadChar(receivedChar); // Читаем из консоли

    if (count_size_buf >= SIZE_BUF_USART)
    {
        count_size_buf = 0;

        __disable_irq();

        while (I2C_Master_Transmit(I2C1, I2C_ADDRESS, (uint8_t*)rezultReadConsol, BUFFER_SIZE_I2C, 10) != 0)
        {
            Error_Handler();
        }


        while (I2C_Master_Receive(I2C1, I2C_ADDRESS, (uint8_t*)rezultReadI2C, BUFFER_SIZE_I2C, 10) != 0)
        {
            Error_Handler();
        }

        __enable_irq();

        USART1_SetString(rezultReadI2C);

    }
    else
    {
        rezultReadConsol[count_size_buf] = receivedChar_;
        count_size_buf++;
    }
}

/*main*/

int main()
{
    Init_LED();
		Init_Button(2);
    Init_USART1(BAUND_RATE);
    Init_I2C(I2C1);

    /* */

    str_BMP180.adress_I2C = BMP180_I2C_ADDR;
    str_BMP180.instance = I2C1;
		str_BMP180.count=0;

    str_SSD1306.adress_I2C = SSD1306_I2C_ADDR;
    str_SSD1306.instance = I2C1;

    /* */
    int count = 0;
    /* */
    BMP180_Init(&str_BMP180);

    SSD1306_Init(&str_SSD1306);


    SSD1306_Fill(SSD1306_COLOR_WHITE);
    /* */
    SSD1306_GotoXY(&str_SSD1306, 20, 0);
    SSD1306_Puts(&str_SSD1306, "DZ", &Font_11x18, 1);
    SSD1306_GotoXY(&str_SSD1306, 10, 30);
    SSD1306_Puts(&str_SSD1306, "xxx", &Font_16x26, 1);
    SSD1306_UpdateScreen(&str_SSD1306);
    SSD1306_Clear(&str_SSD1306);

    /*------------*/
		
	  Get_Flash_Data(&rtc);

    RTC_INIT(&rtc);
    /*------------*/

			char buff_str[20];
    while (1)
    {
			if(Update_Time_Date(&rtc,&str_SSD1306,&current_time_date))
			{
				Execute_Termo_Press();
			}

    }
		
    return 0;
}

void Execute_Termo_Press()
{
    if(count_RTC > TIME_UPDATE_TERMO_PRESS)
    {
        count_RTC=0;
				
				if(Update_Termo_Press(&str_BMP180,date_termo_press) == 0)
				{
					Update_SSD_Termo_Press(&str_SSD1306,date_termo_press);
				}
    }
		
		if(date_termo_press[0].temperature == 0)
		{
				if(Update_Termo_Press(&str_BMP180,date_termo_press) == 0)
				{
					Update_SSD_Termo_Press(&str_SSD1306,date_termo_press);
				}
		}
		
		count_RTC++;
}

uint8_t Update_Termo_Press(bmp180_t* str_BMP180, bmp180_t *date_termo_press)
{
    BMP180_GetTemp(str_BMP180);
    BMP180_GetPress(str_BMP180);
    BMP180_GetAlt(str_BMP180);
		
		if(date_termo_press[0].temperature == 0)
		{
			date_termo_press[str_BMP180->count] = *str_BMP180;
			return 0;
		}
		
		date_termo_press[str_BMP180->count] = *str_BMP180;
	
		str_BMP180->count ++;
		
		if((str_BMP180->count) > (SIZE_DATE_TERMO_PRESS-1))
		{
			str_BMP180->count = 0;
			
			uint32_t temp_termo=0;
			uint32_t temp_press=0;
			uint32_t temp_alt=0;
			
			for(uint8_t i=0; i < SIZE_DATE_TERMO_PRESS; i++)
			{
				temp_termo += date_termo_press[i].temperature;
				temp_press += date_termo_press[i].pressure_RTUT_STOLB;
				temp_alt += date_termo_press[i].altitude;
			}
			
			date_termo_press[0].temperature = temp_termo / SIZE_DATE_TERMO_PRESS;
			date_termo_press[0].pressure_RTUT_STOLB = temp_press / SIZE_DATE_TERMO_PRESS;
			date_termo_press[0].altitude = temp_alt / SIZE_DATE_TERMO_PRESS;
			
			return 0;
		}
		
		return 1;
}

void Update_SSD_Termo_Press(ssd1306_t *str_SSD1306, bmp180_t *date_termo_press)
{
	char buff_str_temp[20];
	char buff_str_press[20];
	char buff_str_alt[20];

	      Sprintf_M(buff_str_temp, "Temp: ", date_termo_press[0].temperature);
        SSD1306_GotoXY(str_SSD1306, 0, 20);
        SSD1306_Puts(str_SSD1306, buff_str_temp, &Font_7x10, 1);

        Sprintf_M(buff_str_press, "Press: ", date_termo_press[0].pressure_RTUT_STOLB);
        SSD1306_GotoXY(str_SSD1306, 0, 30);
        SSD1306_Puts(str_SSD1306, buff_str_press, &Font_7x10, 1);

        Sprintf_M(buff_str_alt, "Alt: ", date_termo_press[0].altitude);
        SSD1306_GotoXY(str_SSD1306, 0, 40);
        SSD1306_Puts(str_SSD1306, buff_str_alt, &Font_7x10, 1);
				
				Write_Terminal_USART(buff_str_temp,buff_str_press,buff_str_alt);
				
        SSD1306_UpdateScreen(str_SSD1306);
}

Status_t Write_Terminal_USART(char *str1, char *str2, char *str3)
{
	char buff_str[SIZE_BUFF_USART];
	
	USART1_SetString(NEW_STRING_CONSOLE);
	
	Sprintf_str_to_str(SIZE_BUFF_USART,buff_str,str1," ",str2," ",str3," /stop");
	
	USART1_SetString(buff_str);
	
	return TRUE;
}

