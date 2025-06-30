#include "clock_RTC.h"

void RTC_INIT(RTC_t *rtc)
{
    RCC_LSI_Enable();
		
		rtc->instance = RTC;
		
		if (RTC_Init(rtc) != 0)
    {
    }
		
		if (RTC_SetTime(rtc) != 0)
    {
    }
		
		if (RTC_SetDate(rtc) != 0)
    {
    }
		
//		if (SetRTCAlarm(rtc) != 0) //запускать при наличии настроек
//    {
//    }

}

void RTC_Init_Alarm(RTC_t *rtc)
{
		RCC_LSI_Enable();
		if (SetRTCAlarm(rtc) != 0)
    {
    }
}

uint8_t RCC_LSI_Enable()
{
    RCC->CSR |= RCC_CSR_LSION;

    while ((RCC->CSR & RCC_CSR_LSIRDY) == 0)
    {
    }
    return 0;
}

uint8_t RTC_Init(RTC_t *rtc)
{
    uint8_t status = 1;

    RTC_MspInit(rtc);

    rtc->instance->WPR = 0xCAU;
    rtc->instance->WPR = 0x53U;

    /* Clear RTC_CR FMT, OSEL and POL Bits */
    rtc->instance->CR &= ((uint32_t)~(RTC_CR_FMT | RTC_CR_OSEL | RTC_CR_POL));

    /* Set RTC_CR register */
    rtc->instance->CR |= (uint32_t)(rtc->init.HourFormat | rtc->init.OutPut | rtc->init.OutPutPolarity);

    /* Configure the RTC PRER */
    rtc->instance->PRER = (uint32_t)(rtc->init.SynchPrediv);
    rtc->instance->PRER |= (uint32_t)(rtc->init.AsynchPrediv << RTC_PRER_PREDIV_A_Pos);

    /* Exit Initialization mode */
    status = RTC_ExitInitMode(rtc);

    rtc->instance->TAFCR &= (uint32_t)~RTC_TAFCR_ALARMOUTTYPE;
    rtc->instance->TAFCR |= (uint32_t)(rtc->init.OutPutType);

    /* Enable the write protection for RTC registers */
    rtc->instance->WPR = 0xFFU;

    return  status;
}

void RTC_MspInit(RTC_t *rtc)
{
    RCC_PeriphCLKInit PeriphClkInitStruct = { 0 };

    if (rtc->instance == RTC)
    {
        /** Initializes the peripherals clock
        */
        PeriphClkInitStruct.PeriphClockSelection = 0x00000002;//RCC_PERIPHCLK_RTC=0x00000002
        PeriphClkInitStruct.RTCClockSelection = 0x00000200;//RCC_RTCCLKSOURCE_LSI=0x00000200

        if (RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != 0)
        {
        }

        NVIC_EnableIRQ(RTC_Alarm_IRQn);
    }
}

uint8_t RTC_ExitInitMode(RTC_t *rtc)
{
    uint8_t status = 0;

    /* Clear INIT bit to exit Initialization mode */
    DISABLE_BIT(rtc->instance->ISR, RTC_ISR_INIT);

    /* If CR_BYPSHAD bit = 0, wait for synchro */
    if (Read_BIT(rtc->instance->CR, RTC_CR_BYPSHAD) == 0U)
    {
        if (RTC_WaitForSynchro(rtc) != 0)
        {
            /* Set RTC state */
            status = 0x01U;//HAL_ERROR=0x01U;
        }
    }

    return status;
}

uint8_t RTC_WaitForSynchro(RTC_t *rtc)
{
    /* Clear RSF flag */
    rtc->instance->ISR &= (uint32_t)~(RTC_ISR_INIT | RTC_ISR_RSF);

    /* Wait the registers to be synchronised */
    while ((rtc->instance->ISR & RTC_ISR_RSF) == 0U)
    {
    }

    return 0;
}

uint8_t RTC_EnterInitMode(RTC_t *rtc)
{
    PWR->CR |= PWR_CR_DBP;
    RCC->BDCR |= RCC_BDCR_RTCEN;

    uint8_t status = 0;

    /* Check that Initialization mode is not already set */
    if (Read_BIT(rtc->instance->ISR, RTC_ISR_INITF) == 0U)
    {
        /* Set INIT bit to enter Initialization mode */
        ENABLE_BIT(rtc->instance->ISR, RTC_ISR_INIT);

        /* Wait till RTC is in INIT state and if timeout is reached exit */
        while ((Read_BIT(rtc->instance->ISR, RTC_ISR_INITF) == 0U) /*&& (status != 1)*/)
        {
        }
    }

    return status;
}

uint8_t RTC_ByteToBcd2(uint8_t number)
{
    uint8_t bcdhigh = 0;

    while (number >= 10)
    {
        bcdhigh++;
        number -= 10;
    }

    return ((uint8_t)(bcdhigh << 4) | number);
}

uint8_t RTC_Bcd2ToByte(uint8_t number)
{
    uint8_t tmp = 0;
		
    tmp = ((uint8_t)(number & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
		
    return (tmp + (number & (uint8_t)0x0F));
}

uint8_t RCCEx_PeriphCLKConfig(RCC_PeriphCLKInit* periphClkStructure)
{
    uint32_t tickstart = 0U;
    uint32_t tmpreg1 = 0U;

    /*---------------------------- I2S configuration ---------------------------*/
    if ((((periphClkStructure->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == RCC_PERIPHCLK_I2S) ||
    (((periphClkStructure->PeriphClockSelection) & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S))
    {
        /* Wait till PLLI2S is disabled */
        while (RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY) != 0)
        {
        }

        RCC->PLLI2SCFGR = ((periphClkStructure->PLLI2SN << RCC_PLLI2SCFGR_PLLI2SN_Pos) | (periphClkStructure->PLLI2SR << RCC_PLLI2SCFGR_PLLI2SR_Pos));

        /* Wait till PLLI2S is ready */
        while (RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY) == 0)
        {
        }
    }

    /*---------------------------- RTC configuration ---------------------------*/
    if (((periphClkStructure->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))
    {
        ENABLE_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);

        /* Enable write access to Backup domain */
        PWR->CR |= PWR_CR_DBP;

        while ((PWR->CR & PWR_CR_DBP) == 0)
        {
        }

        /* Reset the Backup domain only if the RTC Clock source selection is modified from reset value */
        tmpreg1 = (RCC->BDCR & RCC_BDCR_RTCSEL);
        if ((tmpreg1 != 0x00000000U) && ((tmpreg1) != (periphClkStructure->RTCClockSelection & RCC_BDCR_RTCSEL)))
        {
            /* Store the content of BDCR register before the reset of Backup Domain */
            tmpreg1 = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));

            /* Restore the Content of BDCR register */
            RCC->BDCR = tmpreg1;

            /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
            if ((RCC->BDCR & RCC_BDCR_LSEON) == RCC_BDCR_LSEON)
            {
                /* Wait till LSE is ready */
                while (RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
                {
                }
            }
        }

        if ((periphClkStructure->RTCClockSelection & RCC_BDCR_RTCSEL) == RCC_BDCR_RTCSEL)
        {
            MODIFY_REG(RCC->CFGR, RCC_CFGR_RTCPRE, (periphClkStructure->RTCClockSelection & 0xFFFFCFFU));
        }
        else
        {
            DISABLE_BIT(RCC->CFGR, RCC_CFGR_RTCPRE);
        }

        RCC->BDCR |= periphClkStructure->RTCClockSelection & 0x00000FFFU;

    }

    return 0;
}

uint8_t RTC_SetTime(RTC_t *rtc)
{
    uint32_t tmpreg = 0U;
    uint8_t status;

        if ((rtc->instance->CR & RTC_CR_FMT) != 0U)
        {
        }
        else
        {
            rtc->time.TimeFormat = 0x00U;
        }

        tmpreg = (uint32_t)(((uint32_t)RTC_ByteToBcd2(rtc->time.Hours) << RTC_TR_HU_Pos) | \
                            ((uint32_t)RTC_ByteToBcd2(rtc->time.Minutes) << RTC_TR_MNU_Pos) | \
                            ((uint32_t)RTC_ByteToBcd2(rtc->time.Seconds)) | \
                            (((uint32_t)rtc->time.TimeFormat) << RTC_TR_PM_Pos));

    /* Disable the write protection for RTC registers */
    rtc->instance->WPR = 0xCAU;
    rtc->instance->WPR = 0x53U;

    /* Enter Initialization mode */
    status = RTC_EnterInitMode(rtc);

    if (status == 0)
    {
        /* Set the RTC_TR register */
        rtc->instance->TR = (uint32_t)(tmpreg & (RTC_TR_HT | RTC_TR_HU | RTC_TR_MNT | RTC_TR_MNU | RTC_TR_ST | RTC_TR_SU | RTC_TR_PM));

        /* Clear the bits to be configured (Deprecated. Use HAL_RTC_DST_xxx functions instead) */
        rtc->instance->CR &= (uint32_t)~RTC_CR_BKP;

        /* Configure the RTC_CR register (Deprecated. Use HAL_RTC_DST_xxx functions instead) */
        rtc->instance->CR |= (uint32_t)(rtc->time.DayLightSaving | rtc->time.StoreOperation);

        /* Exit Initialization mode */
        status = RTC_ExitInitMode(rtc);
    }

    /* Enable the write protection for RTC registers */
    rtc->instance->WPR = 0xFFU;

    return status;
}

uint8_t RTC_SetDate(RTC_t *rtc)
{
    uint32_t datetmpreg = 0U;
    uint8_t status;

    //if ((0 == 0) && ((rtc->date.Month & 0x10U) == 0x10U))
		if (((rtc->date.Month & 0x10U) == 0x10U))
    {
        rtc->date.Month = (uint8_t)((rtc->date.Month & (uint8_t)~(0x10U)) + (uint8_t)0x0AU);
    }

        datetmpreg = (((uint32_t)RTC_ByteToBcd2(rtc->date.Year) << RTC_DR_YU_Pos) | \
                      ((uint32_t)RTC_ByteToBcd2(rtc->date.Month) << RTC_DR_MU_Pos) | \
                      ((uint32_t)RTC_ByteToBcd2(rtc->date.Date)) | \
                      ((uint32_t)rtc->date.WeekDay << RTC_DR_WDU_Pos));


    /* Disable the write protection for RTC registers */
    rtc->instance->WPR = 0xCAU;
    rtc->instance->WPR = 0x53U;

    /* Enter Initialization mode */
    status = RTC_EnterInitMode(rtc);

    if (status == 0)
    {
        /* Set the RTC_DR register */
        rtc->instance->DR = (uint32_t)(datetmpreg & (RTC_DR_YT | RTC_DR_YU | RTC_DR_MT | RTC_DR_MU | RTC_DR_DT | RTC_DR_DU | RTC_DR_WDU));

        /* Exit Initialization mode */
        status = RTC_ExitInitMode(rtc);
    }

    /* Enable the write protection for RTC registers */
    rtc->instance->WPR = 0xFFU;

    return status;
}

uint8_t RTC_GetTime(RTC_t *rtc)
{
    uint32_t tmpreg = 0U;

    /* Get subseconds value from the corresponding register */
    rtc->time.SubSeconds = (uint32_t)(rtc->instance->SSR);

    /* Get SecondFraction structure field from the corresponding register field*/
    rtc->time.SecondFraction = (uint32_t)(rtc->instance->PRER & RTC_PRER_PREDIV_S);

    /* Get the TR register */
    tmpreg = (uint32_t)(rtc->instance->TR & (RTC_TR_HT | RTC_TR_HU | RTC_TR_MNT | RTC_TR_MNU | RTC_TR_ST | RTC_TR_SU | RTC_TR_PM));

    /* Fill the structure fields with the read parameters */
    rtc->time.Hours = (uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> RTC_TR_HU_Pos);
    rtc->time.Minutes = (uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >> RTC_TR_MNU_Pos);
    rtc->time.Seconds = (uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU));
    rtc->time.TimeFormat = (uint8_t)((tmpreg & (RTC_TR_PM)) >> RTC_TR_PM_Pos);

        /* Convert the time structure parameters to Binary format */
        rtc->time.Hours = (uint8_t)RTC_Bcd2ToByte(rtc->time.Hours);
        rtc->time.Minutes = (uint8_t)RTC_Bcd2ToByte(rtc->time.Minutes);
        rtc->time.Seconds = (uint8_t)RTC_Bcd2ToByte(rtc->time.Seconds);

    return 0;
}

uint8_t RTC_GetDate(RTC_t *rtc)
{
    uint32_t datetmpreg = 0U;

    /* Get the DR register */
    datetmpreg = (uint32_t)(rtc->instance->DR & (RTC_DR_YT | RTC_DR_YU | RTC_DR_MT | RTC_DR_MU | RTC_DR_DT | RTC_DR_DU | RTC_DR_WDU));

    /* Fill the structure fields with the read parameters */
    rtc->date.Year = (uint8_t)((datetmpreg & (RTC_DR_YT | RTC_DR_YU)) >> RTC_DR_YU_Pos);
    rtc->date.Month = (uint8_t)((datetmpreg & (RTC_DR_MT | RTC_DR_MU)) >> RTC_DR_MU_Pos);
    rtc->date.Date = (uint8_t)(datetmpreg & (RTC_DR_DT | RTC_DR_DU));
    rtc->date.WeekDay = (uint8_t)((datetmpreg & (RTC_DR_WDU)) >> RTC_DR_WDU_Pos);

        /* Convert the date structure parameters to Binary format */
        rtc->date.Year = (uint8_t)RTC_Bcd2ToByte(rtc->date.Year);
        rtc->date.Month = (uint8_t)RTC_Bcd2ToByte(rtc->date.Month);
        rtc->date.Date = (uint8_t)RTC_Bcd2ToByte(rtc->date.Date);

    return 0;
}

uint8_t SetRTCAlarm(RTC_t *rtc)
{
		uint8_t status=0;
    /* Configure the alarm A */
    rtc->alarm.AlarmTime.SubSeconds = 0;
    rtc->alarm.AlarmTime.DayLightSaving = 0;
    rtc->alarm.AlarmTime.StoreOperation = 0;

    /* Set Alarm to trigger when hours, minutes and seconds match */
    rtc->alarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
    rtc->alarm.AlarmSubSecondMask = 0;
    rtc->alarm.AlarmDateWeekDaySel = 0;
    rtc->alarm.AlarmDateWeekDay = 1;
    rtc->alarm.Alarm = RTC_CR_ALRAE;

    /* Enable Alarm A interrupt */
    NVIC_EnableIRQ(RTC_Alarm_IRQn);
    /* Set and enable the alarm */
    RTC_SetAlarm_IT(rtc);
		
		return status;
}

uint8_t RTC_SetAlarm_IT(RTC_t *rtc)
{
    uint32_t count = 1000 * (SystemCoreClock / 32U / 1000U);
    uint32_t tmpreg = 0U;
    uint32_t subsecondtmpreg = 0U;

    /* Check the data format (binary or BCD) and store the Alarm time and date
       configuration accordingly */

        if ((rtc->instance->CR & RTC_CR_FMT) != 0U)
        {

        }
        else
        {
            rtc->alarm.AlarmTime.TimeFormat = 0x00U;
        }

        tmpreg = (((uint32_t)RTC_ByteToBcd2(rtc->alarm.AlarmTime.Hours) << RTC_ALRMAR_HU_Pos) | \
                  ((uint32_t)RTC_ByteToBcd2(rtc->alarm.AlarmTime.Minutes) << RTC_ALRMAR_MNU_Pos) | \
                  ((uint32_t)RTC_ByteToBcd2(rtc->alarm.AlarmTime.Seconds)) | \
                  ((uint32_t)(rtc->alarm.AlarmTime.TimeFormat) << RTC_TR_PM_Pos) | \
                  ((uint32_t)RTC_ByteToBcd2(rtc->alarm.AlarmDateWeekDay) << RTC_ALRMAR_DU_Pos) | \
                  ((uint32_t)rtc->alarm.AlarmDateWeekDaySel) | \
                  ((uint32_t)rtc->alarm.AlarmMask));

    /* Store the Alarm subseconds configuration */
    subsecondtmpreg = (uint32_t)((uint32_t)(rtc->alarm.AlarmTime.SubSeconds) | \
                                 (uint32_t)(rtc->alarm.AlarmSubSecondMask));

    /* Disable the write protection for RTC registers */
    rtc->instance->WPR = 0xCAU;
    rtc->instance->WPR = 0x53U;

    /* Configure the Alarm register */
    if (rtc->alarm.Alarm == RTC_CR_ALRAE)
    {
        /* Disable the Alarm A */
        rtc->instance->CR &= ~(RTC_CR_ALRAE);

        /* Clear the Alarm flag */
        rtc->instance->ISR = (~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (rtc->instance->ISR & RTC_ISR_INIT));

        /* Wait till RTC ALRAWF flag is set and if timeout is reached exit */
        do
        {
            if (count-- == 0U)
            {
                /* Enable the write protection for RTC registers */
                rtc->instance->WPR = 0xFFU;
                return 0x03;//HAL_TIMEOUT;
            }

        }
        while (RTC_ALARM_GET_FLAG(rtc, RTC_ISR_ALRAWF) == 0U);

        rtc->instance->ALRMAR = (uint32_t)tmpreg;
        /* Configure the Alarm A Subseconds register */
        rtc->instance->ALRMASSR = subsecondtmpreg;
        /* Configure the Alarm state: Enable Alarm */
        rtc->instance->CR |= (RTC_CR_ALRAE);
        /* Configure the Alarm interrupt */
        rtc->instance->CR |= RTC_CR_ALRAIE;
    }
    else
    {
        /* Disable the Alarm B */
        rtc->instance->CR &= ~(RTC_CR_ALRBE);
        /* Clear the Alarm flag */
        rtc->instance->ISR = (~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (rtc->instance->ISR & RTC_ISR_INIT));

        /* Reload the counter */
        count = 1000 * (SystemCoreClock / 32U / 1000U);

        /* Wait till RTC ALRBWF flag is set and if timeout is reached exit */
        do
        {
            if (count-- == 0U)
            {
                /* Enable the write protection for RTC registers */
                rtc->instance->WPR = 0xFFU;
                return 0x03;//HAL_TIMEOUT;
            }
        }
        while (RTC_ALARM_GET_FLAG(rtc, RTC_ISR_ALRBWF) == 0U);

        rtc->instance->ALRMBR = (uint32_t)tmpreg;
        /* Configure the Alarm B Subseconds register */
        rtc->instance->ALRMBSSR = subsecondtmpreg;
        /* Configure the Alarm state: Enable Alarm */
        rtc->instance->CR |= RTC_CR_ALRBE;
        /* Configure the Alarm interrupt */
        rtc->instance->CR |= RTC_CR_ALRAIE;
    }

    /* RTC Alarm Interrupt Configuration: EXTI configuration */
    EXTI->IMR |= EXTI_IMR_MR17;
    EXTI->RTSR |= EXTI_IMR_MR17;

    /* Enable the write protection for RTC registers */
    rtc->instance->WPR = 0xFFU;

    return 0;
}

/*IRQ*/
void RTC_Alarm_IRQHandler()
{
	//Execute_RTC_IRQ();
    if (RTC->ISR & RTC_ISR_ALRAF)
    {
				RTC->ISR &= ~RTC_ISR_ALRAF;  // Очищаем флаг ALRAF
    }
}






















