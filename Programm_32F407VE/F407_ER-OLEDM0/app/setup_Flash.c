#include "setup_Flash.h"

void Set_Flash_Data(RTC_t *time_date)
{
	FLASH_Unlock();
	
	arr_time_date[0] = time_date->time.Hours;
	arr_time_date[1] = time_date->time.Minutes;
	arr_time_date[2] = time_date->date.Date;
	arr_time_date[3] = time_date->date.Month;
	arr_time_date[4] = time_date->date.Year;
	
	uint8_t size_temp = sizeof(addr_time_date)/sizeof(addr_time_date[0]);
	
//	FLASH_EraseSector( GetSector( addr_time_date[0] ), VoltageRange_3 );
	
	
	for(uint8_t i = 0; i <  size_temp; i++)
	{
		FLASH_EraseSector( GetSector( addr_time_date[i] ), VoltageRange_3 );
		delay_ms(50);
	}
	
	for(uint8_t i = 0; i <  size_temp; i++)
	{
		FLASH_ProgramWord(addr_time_date[i], arr_time_date[i]);
		delay_ms(50);
	}

	FLASH_Lock();
}

void Get_Flash_Data(RTC_t *time_date)
{
	FLASH_Unlock();

	uint8_t size_temp = sizeof(addr_time_date)/sizeof(addr_time_date[0]);
	
	for(uint8_t i = 0; i < size_temp; i++)
	{
		arr_time_date[i] = FLASH_Read(addr_time_date[i]);
	}
	
	FLASH_Lock();
	
	time_date->time.Hours = arr_time_date[0];
	time_date->time.Minutes = arr_time_date[1];
	time_date->date.Date = arr_time_date[2];
	time_date->date.Month = arr_time_date[3];
	time_date->date.Year = arr_time_date[4];
}

//    uint32_t addr = 0x08010000;
//    /*------------*/

//    FLASH_Unlock();
//    // Адрес для записи
//    FLASH_EraseSector( GetSector( addr ), VoltageRange_3 );;
//    FLASH_ProgramWord(addr, 0xDEADBACF);

//    data = FLASH_Read(addr);
//    FLASH_Lock();