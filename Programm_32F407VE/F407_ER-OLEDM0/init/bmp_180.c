#include "bmp_180.h"

callibr_t callibr;

/**
 * @brief  Инициализация
 * @param  *str: структура экземпляра bmp180
 * @retval None
 */
void BMP180_Init(bmp180_t *str)
{
    Read_Callibration(str,&callibr);
}

/**
 * @brief  Получение из памяти bmp180 данных
 * @param  *str: структура экземпляра bmp180
 * @param  *call: структура экземпляра каллибровочных данных bmp180
 * @retval None
 */
void Read_Callibration(bmp180_t *str,callibr_t *call)
{
    uint8_t Callib_Data[22] = {0};

    I2C_Mem_Read(str->instance, str->adress_I2C, CALLIBR_READ_MEM_ADRESS, 1, Callib_Data, 22, MAX_DELAY);

    call -> AC1 = ((Callib_Data[0] << 8) | Callib_Data[1]);
    call -> AC2 = ((Callib_Data[2] << 8) | Callib_Data[3]);
    call -> AC3 = ((Callib_Data[4] << 8) | Callib_Data[5]);
    call -> AC4 = ((Callib_Data[6] << 8) | Callib_Data[7]);
    call -> AC5 = ((Callib_Data[8] << 8) | Callib_Data[9]);
    call -> AC6 = ((Callib_Data[10] << 8) | Callib_Data[11]);
    call -> B1 = ((Callib_Data[12] << 8) | Callib_Data[13]);
    call -> B2 = ((Callib_Data[14] << 8) | Callib_Data[15]);
    call -> MB = ((Callib_Data[16] << 8) | Callib_Data[17]);
    call -> MC = ((Callib_Data[18] << 8) | Callib_Data[19]);
    call -> MD = ((Callib_Data[20] << 8) | Callib_Data[21]);
}

/**
 * @brief  Для получения необработанных данных температуры датчика.
 * @param  *str: структура экземпляра bmp180
 * @retval None
 */
uint16_t Get_UTemp(bmp180_t *str)
{
    uint8_t datatowrite = 0x2E;
    uint8_t Temp_RAW[2] = {0};
		
    I2C_Mem_Write(str->instance, str->adress_I2C, DATA_WRITE_MEM_ADRESS, MEM_ADD_SIZE, &datatowrite, SIZE_DATA_WRITE, TIME_OUT);

		delay_ms(5);
		
    I2C_Mem_Read(str->instance, str->adress_I2C, DATA_READ_MEM_ADRESS, MEM_ADD_SIZE, Temp_RAW, 2, TIME_OUT);
		
    return ((Temp_RAW[0] << 8) + Temp_RAW[1]);
}

/**
 * @brief  Для получения значения температуры датчика.
 * @param  *str: структура экземпляра bmp180
 * @retval None
 */
void BMP180_GetTemp(bmp180_t *str)
{
    callibr.UT = Get_UTemp(str);
    callibr.X1 = ((callibr.UT - callibr.AC6) * (callibr.AC5 /(customPow(2, 15))));

    callibr.X2 = ((callibr.MC * (customPow(2, 11))) / (callibr.X1 + callibr.MD));

    callibr.B5 = callibr.X1 + callibr.X2;
		
    str->temperature = (callibr.B5 + 8) / (customPow(2, 4)) / 10.0;
}

/**
 * @brief  Для получения необработанных данных давления датчика.
 * @param  *str: структура экземпляра bmp180
 * @retval None
 */
uint32_t Get_UPress(bmp180_t *str)
{	
		uint8_t size = 1;
    uint8_t datatowrite = 0x34 + (callibr.oss << 6);
    uint8_t Press_RAW[3] = {0};
		
    I2C_Mem_Write(str->instance, str->adress_I2C, DATA_WRITE_MEM_ADRESS, MEM_ADD_SIZE, &datatowrite, SIZE_DATA_WRITE, TIME_OUT);
		
    switch (callibr.oss)
    {
    case (0):
				delay_ms(5);
        break;
    case (1):
				delay_ms(8);
        break;
    case (2):
				delay_ms(14);
        break;
    case (3):
				delay_ms(26);
        break;
    }
		
    I2C_Mem_Read(str->instance, str->adress_I2C, DATA_READ_MEM_ADRESS, MEM_ADD_SIZE, Press_RAW, 3, TIME_OUT);
		
    return (((Press_RAW[0] << 16) + (Press_RAW[1] << 8) + Press_RAW[2]) >> (8 - callibr.oss));
}

/**
 * @brief  Для получения значения давления датчика.
 * @param  *str: структура экземпляра bmp180
 * @retval None
 */
void BMP180_GetPress(bmp180_t *str)
{
    callibr.UP = Get_UPress(str);
    callibr.X1 = ((callibr.UT - callibr.AC6) * (callibr.AC5 / (customPow(2, 15))));
    callibr.X2 = ((callibr.MC * (customPow(2, 11))) / (callibr.X1 + callibr.MD));
    callibr.B5 = callibr.X1 + callibr.X2;
    callibr.B6 = callibr.B5 - 4000;
    callibr.X1 = (callibr.B2 * (callibr.B6 * callibr.B6 / (customPow(2, 12)))) / (customPow(2, 11));
    callibr.X2 = callibr.AC2 * callibr.B6 / (customPow(2, 11));
    callibr.X3 = callibr.X1 + callibr.X2;
    callibr.B3 = (((callibr.AC1 * 4 + callibr.X3) << callibr.oss) + 2) / 4;
    callibr.X1 = callibr.AC3 * callibr.B6 / customPow(2, 13);
    callibr.X2 = (callibr.B1 * (callibr.B6 * callibr.B6 / (customPow(2, 12)))) / (customPow(2, 16));
    callibr.X3 = ((callibr.X1 + callibr.X2) + 2) / customPow(2, 2);
    callibr.B4 = callibr.AC4 * (unsigned long)(callibr.X3 + 32768) / (customPow(2, 15));
    callibr.B7 = ((unsigned long)callibr.UP - callibr.B3) * (50000 >> callibr.oss);
		
    if (callibr.B7 < 0x80000000)
		{
			str->pressure_PA = (callibr.B7 * 2) / callibr.B4;
		}
        
    else
		{
			str->pressure_PA = (callibr.B7 / callibr.B4) * 2;
		}
				
    callibr.X1 = (str->pressure_PA / (customPow(2, 8))) * (str->pressure_PA / (customPow(2, 8)));
    callibr.X1 = (callibr.X1 * 3038) / (customPow(2, 16));
    callibr.X2 = (-7357 * str->pressure_PA) / (customPow(2, 16));
		
    str->pressure_PA = str->pressure_PA + (callibr.X1 + callibr.X2 + 3791) / (customPow(2, 4));
		str->pressure_RTUT_STOLB = str->pressure_PA * k_RTUT_STOLB;
}

/**
 * @brief  Для получения расчетного уровня над уровнем моря.
 * @param  *str: структура экземпляра bmp180
 * @retval None
 */
void BMP180_GetAlt(bmp180_t *str)
{
    //BMP180_GetPress(str);
		
    str->altitude = (44330/5.5) * (1 - (customPow((str->pressure_PA / ATM_PRESS), 0.19029495718)));/*5.5 my koeff*/
}
