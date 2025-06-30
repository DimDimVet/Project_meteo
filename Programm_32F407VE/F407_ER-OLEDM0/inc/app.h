#ifndef APP_H_
#define APP_H_

/*includes*/
#include "delay.h"
#include "init_LED.h"
#include "init_I2C.h"
#include "init_USART.h"
#include "ssd1306.h"
#include "bmp_180.h"
#include "clock_RTC.h"
#include "button_F407.h"
#include "setup_Timer_Data.h"

/*defaines*/
/*Период измерений температуры-давления в сек*/
#define TIME_UPDATE_TERMO_PRESS 3
/*Размер буфера*/
#define SIZE_DATE_TERMO_PRESS 3
/*Скорость USART*/
#define BAUND_RATE 9600
/*Размер буфера USART*/
#define SIZE_BUFF_USART 36
/*адрес I2C датчика*/
#define BMP180_I2C_ADDR 0xEE
/*адрес I2C экрана*/
#define SSD1306_I2C_ADDR 0x78
/*адрес I2C slave устройства*/
#define I2C_ADDRESS 0x68
/*новая строка*/
#define NEW_STRING_CONSOLE "\r"
/*символ очистки консоли*/
#define CLEAR_CONSOLE "\r\033[K"
/*var*/
static bmp180_t date_termo_press[SIZE_DATE_TERMO_PRESS];
static RTC_t current_time_date;

static char receivedChar_;
static char *receivedChar = &receivedChar_;

static char rezultReadConsol[SIZE_BUF_USART];
static char rezultReadI2C[SIZE_BUF_USART];
/*structures*/


/*func*/
void Execute_Termo_Press(void);
uint8_t Update_Termo_Press(bmp180_t* str_BMP180, bmp180_t *date_termo_press);
void Update_SSD_Termo_Press(ssd1306_t *str_SSD1306, bmp180_t *date_termo_press);
Status_t Write_Terminal_USART(char *str1, char *str2, char *str3);

#endif