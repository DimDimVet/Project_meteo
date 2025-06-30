#include "HEHAL.h"

void Sprintf_M(char *buff, const char *prefix, int val) 
{
    int i = 0;
    
    while (*prefix) 
		{
        buff[i++] = *prefix++;
    }
    
    Сonverter_Int_Str(buff + i,val);
}

void Sprintf_str_to_str(int size_buf,char *buff, char *str1, char *str2, char *str3,char *str4,char *str5,char *str6) 
{
    int i = 0;
    
    while (*str1) 
		{
				if(i < size_buf)
				{
					buff[i++] = *str1++;
				}
				else
				{
					break;
				}
    }
		
		while (*str2) 
		{
        if(i < size_buf)
				{
					buff[i++] = *str2++;
				}
				else
				{
					break;
				}
    }
		
		while (*str3) 
		{
        if(i < size_buf)
				{
					buff[i++] = *str3++;
				}
				else
				{
					break;
				}
    }
		
		while (*str4) 
		{
        if(i < size_buf)
				{
					buff[i++] = *str4++;
				}
				else
				{
					break;
				}
    }
		
		while (*str5) 
		{
        if(i < size_buf)
				{
					buff[i++] = *str5++;
				}
				else
				{
					break;
				}
    }
		
		while (*str6) 
		{
        if(i < size_buf)
				{
					buff[i++] = *str6++;
				}
				else
				{
					break;
				}
    }

}


void Сonverter_Int_Str(char *out_str, int in_digital)
{
	int countInt = 0; 
		
		if (in_digital < 0) {
        out_str[countInt++] = '-';
        in_digital = -in_digital;
    }

    int start = countInt;
    do {
        out_str[countInt++] = (in_digital % 10) + '0';
        in_digital /= 10;
    } while (in_digital > 0);

    // Ревер
    for (int j = start; j < start + (countInt - start) / 2; j++) {
        char temp = out_str[j];
        out_str[j] = out_str[countInt - j + start - 1];
        out_str[countInt - j + start - 1] = temp;
    }
    out_str[countInt] = '\0'; 
}

float_t customPow(float_t x, float_t y) 
	{
    if (y == 0) return 1; // любое число в нулевой степени равно 1
    if (y < 0) return 1 / customPow(x, -y); // обработка отрицательной степени

    float_t result = 1;
    for (int i = 0; i < y; i++) {
        result *= x; // умножаем x на себя y раз
    }
    return result;
}
	
void formatDate(char* buff, int day, int month, int year)
	{
    // Форматируем день
    if (day < 10) 
		{
        buff[0] = '0';
        buff[1] = '0' + day;
    } 
		else 
		{
        buff[0] = '0' + day / 10;
        buff[1] = '0' + day % 10;
    }

    // Форматируем месяц
    buff[3] = ':';
    if (month < 10) {
        buff[4] = '0';
        buff[5] = '0' + month;
    } else {
        buff[4] = '0' + month / 10;
        buff[5] = '0' + month % 10;
    }

    // Форматируем год
    buff[7] = ':';
    buff[8] = '0' + year / 10;
    buff[9] = '0' + year % 10;

//    // Добавляем префикс "Date: "
//    const char* prefix = "Date: ";
//    myStrnCpy(buff + 10, prefix, myStrLen(prefix));
    //myStrnCpy(buff, prefix);
}

void myStrnCpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; ++i) {
        dest[i] = src[i];
    }
    // Если n больше длины src, добавим '\0' в конец
    for (; i < n; ++i) {
        dest[i] = '\0';
    }
}

size_t myStrLen(const char* str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}