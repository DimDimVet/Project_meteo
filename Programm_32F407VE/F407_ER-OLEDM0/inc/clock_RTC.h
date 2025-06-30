#ifndef CLOCK_RTC_H_
#define CLOCK_RTC_H_

/*includes*/
#include "stm32f4xx.h"
#include "HEHAL.h"

/*defaines*/

#define RCC_PERIPHCLK_I2S             0x00000001U
#define RCC_PERIPHCLK_RTC             0x00000002U
#define RCC_PERIPHCLK_PLLI2S          0x00000004U
#define RCC_GET_FLAG(__FLAG__) (((((((__FLAG__) >> 5U) == 1U)? RCC->CR :((((__FLAG__) >> 5U) == 2U) ? RCC->BDCR :((((__FLAG__) >> 5U) == 3U)? RCC->CSR :RCC->CIR))) & (1U << ((__FLAG__) & RCC_FLAG_MASK)))!= 0U)? 1U : 0U)
#define RCC_FLAG_PLLI2SRDY               ((uint8_t)0x3B)
#define RCC_FLAG_MASK  ((uint8_t)0x1FU)

#define RTC_ALARMMASK_NONE                0x00000000U
#define RTC_ALARMMASK_DATEWEEKDAY         RTC_ALRMAR_MSK4
#define RTC_ALARMMASK_HOURS               RTC_ALRMAR_MSK3
#define RTC_ALARMMASK_MINUTES             RTC_ALRMAR_MSK2
#define RTC_ALARMMASK_SECONDS             RTC_ALRMAR_MSK1
#define RTC_ALARMMASK_ALL                 (RTC_ALARMMASK_DATEWEEKDAY | \
                                           RTC_ALARMMASK_HOURS       | \
                                           RTC_ALARMMASK_MINUTES     | \
                                           RTC_ALARMMASK_SECONDS)

#define RTC_ALARM_GET_FLAG(__HANDLE__, __FLAG__)                (((((__HANDLE__)->instance->ISR) & (__FLAG__)) != 0U) ? 1U : 0U)
#define RCC_FLAG_LSERDY                  ((uint8_t)0x41)

/*var*/

/*structures*/
typedef struct
{
    uint32_t PeriphClockSelection; /*!< The Extended Clock to be configured.
                                      This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */

    uint32_t PLLI2SN;    /*!< Specifies the multiplication factor for PLLI2S VCO output clock.
                            This parameter must be a number between Min_Data = 50 and Max_Data = 432
                            Except for STM32F411xE devices where the Min_Data = 192.
                            This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */

    uint32_t PLLI2SR;    /*!< Specifies the division factor for I2S clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 7.
                            This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */


    uint32_t RTCClockSelection;      /*!< Specifies RTC Clock Prescalers Selection.
                                       This parameter can be a value of @ref RCC_RTC_Clock_Source */

} RCC_PeriphCLKInit;

typedef struct
{
  uint8_t WeekDay;  /*!< Specifies the RTC Date WeekDay.
                         This parameter can be a value of @ref RTC_WeekDay_Definitions */

  uint8_t Month;    /*!< Specifies the RTC Date Month (in BCD format).
                         This parameter can be a value of @ref RTC_Month_Date_Definitions */

  uint8_t Date;     /*!< Specifies the RTC Date.
                         This parameter must be a number between Min_Data = 1 and Max_Data = 31 */

  uint8_t Year;     /*!< Specifies the RTC Date Year.
                         This parameter must be a number between Min_Data = 0 and Max_Data = 99 */

} RTC_Date_t;

typedef struct
{
  uint8_t Hours;            /*!< Specifies the RTC Time Hour.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected */

  uint8_t Minutes;          /*!< Specifies the RTC Time Minutes.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

  uint8_t Seconds;          /*!< Specifies the RTC Time Seconds.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

  uint8_t TimeFormat;       /*!< Specifies the RTC AM/PM Time.
                                 This parameter can be a value of @ref RTC_AM_PM_Definitions */

  uint32_t SubSeconds;      /*!< Specifies the RTC_SSR RTC Sub Second register content.
                                 This parameter corresponds to a time unit range between [0-1] Second
                                 with [1 Sec / SecondFraction +1] granularity */

  uint32_t SecondFraction;  /*!< Specifies the range or granularity of Sub Second register content
                                 corresponding to Synchronous prescaler factor value (PREDIV_S)
                                 This parameter corresponds to a time unit range between [0-1] Second
                                 with [1 Sec / SecondFraction +1] granularity.
                                 This field will be used only by HAL_RTC_GetTime function */

  uint32_t DayLightSaving;  /*!< This interface is deprecated. To manage Daylight
                                 Saving Time, please use HAL_RTC_DST_xxx functions */

  uint32_t StoreOperation;  /*!< This interface is deprecated. To manage Daylight
                                 Saving Time, please use HAL_RTC_DST_xxx functions */
} RTC_Time_t;

typedef struct
{
  RTC_Time_t AlarmTime;     /*!< Specifies the RTC Alarm Time members */

  uint32_t AlarmMask;            /*!< Specifies the RTC Alarm Masks.
                                      This parameter can be a value of @ref RTC_AlarmMask_Definitions */

  uint32_t AlarmSubSecondMask;   /*!< Specifies the RTC Alarm SubSeconds Masks.
                                      This parameter can be a value of @ref RTC_Alarm_Sub_Seconds_Masks_Definitions */

  uint32_t AlarmDateWeekDaySel;  /*!< Specifies the RTC Alarm is on Date or WeekDay.
                                      This parameter can be a value of @ref RTC_AlarmDateWeekDay_Definitions */

  uint8_t AlarmDateWeekDay;      /*!< Specifies the RTC Alarm Date/WeekDay.
                                      If the Alarm Date is selected, this parameter must be set to a value in the 1-31 range.
                                      If the Alarm WeekDay is selected, this parameter can be a value of @ref RTC_WeekDay_Definitions */

  uint32_t Alarm;                /*!< Specifies the alarm .
                                      This parameter can be a value of @ref RTC_Alarms_Definitions */
} RTC_Alarm_t;

typedef struct
{
  uint32_t HourFormat;      /*!< Specifies the RTC Hour Format.
                                 This parameter can be a value of @ref RTC_Hour_Formats */

  uint32_t AsynchPrediv;    /*!< Specifies the RTC Asynchronous Predivider value.
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x7F */

  uint32_t SynchPrediv;     /*!< Specifies the RTC Synchronous Predivider value.
                                 This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x7FFF */

  uint32_t OutPut;          /*!< Specifies which signal will be routed to the RTC output.
                                 This parameter can be a value of @ref RTC_Output_selection_Definitions */

  uint32_t OutPutPolarity;  /*!< Specifies the polarity of the output signal.
                                 This parameter can be a value of @ref RTC_Output_Polarity_Definitions */

  uint32_t OutPutType;      /*!< Specifies the RTC Output Pin mode.
                                 This parameter can be a value of @ref RTC_Output_Type_ALARM_OUT */
} RTC_Init_t;

typedef struct
{
  RTC_TypeDef *instance;  /*!< Register base address    */
  RTC_Init_t init;       /*!< RTC required parameters  */
	
	RTC_Alarm_t alarm;
	RTC_Time_t time;
	RTC_Date_t date;

} RTC_t;


/*func*/
void RTC_INIT(RTC_t *rtc);

void RTC_Init_Alarm(RTC_t *rtc);

uint8_t RCC_LSI_Enable(void);

uint8_t RTC_Init(RTC_t *rtc);

void RTC_MspInit(RTC_t *rtc);

uint8_t RTC_ExitInitMode(RTC_t *rtc);

uint8_t RTC_WaitForSynchro(RTC_t *rtc);

uint8_t RTC_EnterInitMode(RTC_t *rtc);

uint8_t RTC_ByteToBcd2(uint8_t number);

uint8_t RTC_Bcd2ToByte(uint8_t number);

uint8_t RCCEx_PeriphCLKConfig(RCC_PeriphCLKInit* periphClkStructure);

uint8_t RTC_SetTime(RTC_t *rtc);

uint8_t RTC_SetDate(RTC_t *rtc);

uint8_t RTC_GetTime(RTC_t *rtc);

uint8_t RTC_GetDate(RTC_t *rtc);

uint8_t SetRTCAlarm(RTC_t *rtc);

uint8_t RTC_SetAlarm_IT(RTC_t *rtc);

void Execute_RTC_IRQ(void);

#endif