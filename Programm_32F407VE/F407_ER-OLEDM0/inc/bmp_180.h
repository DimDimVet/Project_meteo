#ifndef BMP_180_H_
#define BMP_180_H_

/*includes*/
//#include "math.h"
#include "stm32f4xx.h"
#include "HEHAL.h"
#include "init_I2C.h"
#include "delay.h"

/*defaines*/
#define CALLIBR_READ_MEM_ADRESS 0xAA
#define DATA_READ_MEM_ADRESS 0xF6
#define DATA_WRITE_MEM_ADRESS 0xF4
#define MEM_ADD_SIZE 1
#define SIZE_DATA_WRITE 1
#define TIME_OUT 1000

#define ATM_PRESS 101325 // Pa
#define k_RTUT_STOLB 0.75006
/*var*/

/*structures*/

typedef struct
{
    I2C_TypeDef *instance;
    uint16_t adress_I2C;
    float temperature;
    float pressure_PA;
		float pressure_RTUT_STOLB;
    float altitude;

		uint16_t count;

} bmp180_t;

typedef struct
{
    short AC1;
    short AC2;
    short AC3;
    unsigned short AC4;
    unsigned short AC5;
    unsigned short AC6;
    short B1;
    short B2;
    short MB;
    short MC;
    short MD;

    long UT;
    short oss;
    long UP;
    long X1;
    long X2;
    long X3;
    long B3;
    long B5;
    unsigned long B4;
    long B6;
    unsigned long B7;
		
} callibr_t;

/*func*/

void BMP180_Init(bmp180_t *str);

void Read_Callibration(bmp180_t *str,callibr_t *call);

uint16_t Get_UTemp(bmp180_t *str);

void BMP180_GetTemp(bmp180_t *str);

uint32_t Get_UPress(bmp180_t *str);

void BMP180_GetPress(bmp180_t *str);

void BMP180_GetAlt(bmp180_t *str);


#endif