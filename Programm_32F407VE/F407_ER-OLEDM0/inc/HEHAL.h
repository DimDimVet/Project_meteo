#ifndef HEHAL_H_
#define HEHAL_H_

/*includes*/
#include "stdlib.h"
#include "math.h"

/*defaines*/
#define ENABLE_BIT(reg, bit) ((reg) |= (bit)) 
#define DISABLE_BIT(reg, bit) ((reg) &= ~(bit)) 
#define Read_BIT(reg, bit)    ((reg) & (bit)) 
#define Write_REG(REG, VAL)   ((REG) = (VAL))
#define Read_REG(REG)         ((REG))
#define Clear_REG(REG)        ((REG) = (0x0))

/*макс число*/
#define MAX_DELAY      0xFFFFFFFF
#define assert_param(expr) ((void)0U)
	
/*var*/

/*structures*/
typedef enum
{
	TRUE = 1,
	FALSE = 0,
}Status_t;

/*func*/
/*аналог sprintf(buff, "ABSD: %d",(int)digital);*/
void Сonverter_Int_Str(char *str, int val);
void Sprintf_M(char *buff, const char *prefix, int val);
void Sprintf_str_to_str(int size_buf,char *buff, char *str1, char *str2, char *str3,char *str4,char *str5,char *str6);
float_t customPow(float_t x, float_t y);

void formatDate(char* buff, int day, int month, int year);
void myStrnCpy(char* dest, const char* src, size_t n);
size_t myStrLen(const char* str);
	
#endif


/*includes*/

/*defaines*/

/*var*/

/*structures*/

/*func*/