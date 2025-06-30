#include "init_I2C.h"

uint32_t dataBufRx[1];

void Init_I2C(I2C_TypeDef *instance)
{
  Enable_RCC_I2C();
  Config_GPIO_I2C();
  Config_I2C(instance);
}

void Enable_RCC_I2C()
{
	ENABLE_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOBEN);
	ENABLE_BIT(RCC->APB1ENR,RCC_APB1ENR_I2C1EN);
	//ENABLE_BIT(RCC->AHB1ENR,RCC_AHB1ENR_DMA1EN);
}

void Config_GPIO_I2C()
{
	/* PB6 (SCL), PB7 (SDA) */
  GPIOB->MODER |= 0 << GPIO_MODER_MODE6_Pos;       // Очистка режима для PB6
  GPIOB->MODER |= 2 << GPIO_MODER_MODE6_Pos;       // Альтернативная функция для PB6 (SCL)
  GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT6_Pos;       // открытый коллектор
  GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED6_Pos; // скорость

  GPIOB->MODER |= 0 << GPIO_MODER_MODE7_Pos;       // Очистка режима для PB7
  GPIOB->MODER |= 2 << GPIO_MODER_MODE7_Pos;       // Альтернативная функция для PB7 (SDA)
  GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT7_Pos;       // открытый коллектор
  GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED7_Pos; // скорость

  GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL6_Pos; // AF4 для I2C PB6 (SCL)
  GPIOB->AFR[0] |= 4 << GPIO_AFRL_AFSEL7_Pos; // AF4 для I2C PB7 (SDA)
}

void Config_I2C(I2C_TypeDef *instance)
{
  uint32_t freqrange;
  uint32_t pclk1;

  /*Reset I2C*/
  ENABLE_BIT(instance->CR1, I2C_CR1_SWRST);
  DISABLE_BIT(instance->CR1, I2C_CR1_SWRST);

  /* Get PCLK1 frequency */
  pclk1 = RCC_GetPCLK1Freq();
  freqrange = pclk1 / 1000000;

  /* Configure I2Cx: Frequency range */
  ENABLE_BIT(instance->CR2, (freqrange << I2C_CR2_FREQ_Pos));

  /* Configure I2Cx: Rise Time */
  ENABLE_BIT(instance->TRISE, (I2C_Rise_Time(freqrange, CLOCK_SPEED) << I2C_TRISE_TRISE_Pos));

  /* Configure I2Cx: Speed */
  ENABLE_BIT(instance->CCR, I2C_Speed(pclk1, CLOCK_SPEED, 0));

  /* Configure I2Cx: Generalcall and NoStretch mode */
  ENABLE_BIT(instance->CR1, ((0 | 0) << (I2C_CR1_ENGC_Pos | I2C_CR1_NOSTRETCH_Pos)));

  /* Configure I2Cx: Own Address1 and addressing mode */
  ENABLE_BIT(instance->OAR1, ((I2C_ADDRESSINGMODE_7BIT | 0) << (I2C_OAR1_ADDMODE_Pos | I2C_OAR1_ADD0_Pos)));

  /* Configure I2Cx: Dual mode and Own Address2 */
  ENABLE_BIT(instance->OAR2, ((0 | 0) << (I2C_OAR2_ENDUAL_Pos | I2C_OAR2_ADD2_Pos)));

  /* Enable the selected I2C peripheral */
	ENABLE_BIT(instance->CR1,I2C_CR1_PE);

  NVIC_EnableIRQ(I2C1_EV_IRQn);
}

uint32_t RCC_GetPCLK1Freq(void)
{
  uint32_t temp = SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]; // 10U=RCC_CFGR_PPRE1_Pos
  return temp;
}

uint32_t I2C_Speed(uint32_t pclk, uint32_t speed, uint32_t dutyCycle)
{
  uint32_t timing = 0;

  if (dutyCycle == I2C_CCR_DUTY)
  {
    // Формула для коэффициента заполнения 16/9
    timing = (pclk / (speed * 9)) - 1;
  }
  else
  {
    // Формула для коэффициента заполнения 1/2
    timing = (pclk / (speed * 2)) - 1;
  }

  return timing;
}

uint32_t I2C_Rise_Time(uint32_t freqrange, uint32_t clockSpeed)
{
  if (clockSpeed <= 100000)
  {
    freqrange = freqrange + 1;
  }
  else
  {
    freqrange = ((freqrange * 300) / 1000) + 1;
  }
  return freqrange;
}

uint8_t I2C_WaitOnFlagUntilTimeout(I2C_TypeDef *instance, uint32_t flag, FlagStatus Status, uint32_t Timeout)
{
  uint32_t Time_count;
  /* Wait until flag is set */
  while (I2C_GET_FLAG(instance, flag) == Status)
  {
    /* Check for the Timeout */
    if (Timeout != MAX_DELAY)
    {
			/* Check for the Timeout */
			if (Timeout != MAX_DELAY)
			{

			}
    }
  }
  return 0;
}

/**
 * @brief Эта функция обрабатывает тайм-аут связи I2C для определенного использования флага TXE.
 * @retval
 */
uint8_t I2C_WaitOnTXEFlagUntilTimeout(I2C_TypeDef *instance)
{
  while (!(Read_BIT(instance->SR1, I2C_SR1_TXE)))
  {
  }
  return 0;
}

uint8_t I2C_WaitOnRXNEFlagUntilTimeout(I2C_TypeDef *instance)
{
  // ждем прием данных
  while (!(instance->SR1 & I2C_SR1_RXNE))
  {
		
  }
  return 0;
}

/**
 * @brief  This function handles I2C Communication Timeout for specific usage of BTF flag.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  Timeout Timeout duration
 * @param  Tickstart Tick start value
 * @retval HAL status
 */
uint8_t I2C_WaitOnBTFFlagUntilTimeout(I2C_TypeDef *instance, uint32_t Timeout)
{
  while (I2C_GET_FLAG(instance, I2C_FLAG_BTF) == RESET)
  {
    /* Check if a NACK is detected */
    if (I2C_IsAcknowledgeFailed(instance) != 0)
    {
      return 1;
    }

    /* Check for the Timeout */
    if (Timeout != MAX_DELAY)
    {

    }
  }
  return 0;
}

/**
 * @brief Эта функция обрабатывает тайм-аут связи I2C для фазы адресации мастера.
 * @retval
 */

uint8_t I2C_WaitOnMasterAddressFlagUntilTimeout(I2C_TypeDef *instance, uint32_t Flag, uint32_t Timeout)
{
  uint32_t Time_count;

  while (!(Read_BIT(instance->SR1, Flag)))
  {
    /* Check for the Timeout */
    if (Timeout != MAX_DELAY)
    {

    }
  }
  return 0;
}

/**
 * @brief  Master sends target device address followed by internal memory address for read request.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *         the configuration information for I2C module
 * @param  DevAddress Target device address: The device 7 bits address value
 *         in datasheet must be shifted to the left before calling the interface
 * @param  MemAddress Internal memory address
 * @param  MemAddSize Size of internal memory address
 * @param  Timeout Timeout duration
 * @param  Tickstart Tick start value
 * @retval HAL status
 */

uint8_t I2C_RequestMemoryRead(I2C_TypeDef *instance, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout)
{
  /* Enable Acknowledge */
  ENABLE_BIT(instance->CR1, I2C_CR1_ACK);

  /* Generate Start */
  ENABLE_BIT(instance->CR1, I2C_CR1_START);

  /* Wait until SB flag is set */
  if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_SB, RESET, Timeout) != 0)
  {
    if (Read_BIT(instance->CR1, I2C_CR1_START) == I2C_CR1_START)
    {
    }

    return 3;
  }

  /* Send slave address */
  instance->DR = I2C_7BIT_ADD_Write(DevAddress);

  /* Wait until ADDR flag is set */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(instance, I2C_SR1_ADDR, Timeout) != 0)
  {
    //    return HAL_ERROR;
    return 1;
  }

  /* Clear ADDR flag */
  I2C_CLEAR_ADDRFLAG(instance);

  /* Wait until TXE flag is set */
  if (I2C_WaitOnTXEFlagUntilTimeout(instance) != 0)
  {
    // откл проверку адреса
    ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
    return 1;
  }

  /* If Memory address size is 8Bit */
  if (MemAddSize == I2C_MEMADD_SIZE_8BIT)
  {
    /* Send Memory Address */
    Write_REG(instance->DR, I2C_MEM_ADD_LSB(MemAddress));
  }
  /* If Memory address size is 16Bit */
  else
  {
    /* Send MSB of Memory Address */
    Write_REG(instance->DR, I2C_MEM_ADD_MSB(MemAddress));

    /* Wait until TXE flag is set */
    if (I2C_WaitOnTXEFlagUntilTimeout(instance) != 0)
    {
      // откл проверку адреса
      ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
      return 1;
    }

    /* Send LSB of Memory Address */
    Write_REG(instance->DR, I2C_MEM_ADD_LSB(MemAddress));
  }

  /* Wait until TXE flag is set */
  if (I2C_WaitOnTXEFlagUntilTimeout(instance) != 0)
  {
    // откл проверку адреса
    ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
    return 1;
  }

  /* Generate Restart */
  ENABLE_BIT(instance->CR1, I2C_CR1_START);

  /* Wait until SB flag is set */
  if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_SB, RESET, Timeout) != 0)
  {
    if (Read_BIT(instance->CR1, I2C_CR1_START) == I2C_CR1_START)
    {
    }
    // return HAL_TIMEOUT;
    return 3;
  }

  /* Send slave address */
  Write_REG(instance->DR, (uint8_t)(DevAddress | I2C_OAR1_ADD0));

  /* Wait until ADDR flag is set */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(instance, I2C_SR1_ADDR, Timeout) != 0)
  {
    return 1;
  }

  return 0;
}

/**
 * @brief  Master sends target device address followed by internal memory address for write request.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *         the configuration information for I2C module
 * @param  DevAddress Target device address: The device 7 bits address value
 *         in datasheet must be shifted to the left before calling the interface
 * @param  MemAddress Internal memory address
 * @param  MemAddSize Size of internal memory address
 * @param  Timeout Timeout duration
 * @param  Tickstart Tick start value
 * @retval HAL status
 */

uint8_t I2C_RequestMemoryWrite(I2C_TypeDef *instance, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout)
{
  /* Generate Start */
  ENABLE_BIT(instance->CR1, I2C_CR1_START);

  /* Wait until SB flag is set */
  if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_SB, RESET, Timeout) != 0)
  {
    if (Read_BIT(instance->CR1, I2C_CR1_START) == I2C_CR1_START)
    {

    }
    // return HAL_TIMEOUT;
    return 3;
  }
	
  /* Send slave address */
  instance->DR = I2C_7BIT_ADD_Write(DevAddress);

  /* Wait until ADDR flag is set */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(instance, I2C_SR1_ADDR, 10) != 0)
  {
    //    return HAL_ERROR;
    return 1;
  }

  /* Clear ADDR flag */
  I2C_CLEAR_ADDRFLAG(instance);

  /* Wait until TXE flag is set */
  if (I2C_WaitOnTXEFlagUntilTimeout(instance) != 0)
  {
    // откл проверку адреса
    ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
    return 1;
  }

  /* If Memory address size is 8Bit */
  if (MemAddSize == I2C_MEMADD_SIZE_8BIT)
  {
    /* Send Memory Address */
    Write_REG(instance->DR, I2C_MEM_ADD_LSB(MemAddress));
  }
  /* If Memory address size is 16Bit */
  else
  {
    /* Send MSB of Memory Address */
    Write_REG(instance->DR, I2C_MEM_ADD_MSB(MemAddress));
		
    /* Wait until TXE flag is set */
    if (I2C_WaitOnTXEFlagUntilTimeout(instance) != 0)
    {
      // откл проверку адреса
      ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
      return 1;
    }

    /* Send LSB of Memory Address */
    Write_REG(instance->DR, I2C_MEM_ADD_LSB(MemAddress));
  }

  return 0;
}

/**
 * @brief  This function handles Acknowledge failed detection during an I2C Communication.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @retval HAL status
 */
uint8_t I2C_IsAcknowledgeFailed(I2C_TypeDef *instance)
{
  if (I2C_GET_FLAG(instance, I2C_FLAG_AF) == SET)
  {
    /* Clear NACKF Flag */
    I2C_CLEAR_FLAG(instance, I2C_FLAG_AF);
		
    return 1;
  }
  return 0;
}

uint8_t I2C_IsDeviceReady(I2C_TypeDef *instance, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
  uint32_t I2C_Trials = 0;
  FlagStatus tmp1;
  FlagStatus tmp2;

  /* Wait until BUSY flag is reset */
  if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG) != 0)
  {
    // return HAL_BUSY;
    return 2;
  }
	ENABLE_BIT(instance->CR1,I2C_CR1_PE);

  /* Disable Pos */
  DISABLE_BIT(instance->CR1, I2C_CR1_POS);

  do
  {
    /* Generate Start */
    ENABLE_BIT(instance->CR1, I2C_CR1_START);

    /* Wait until SB flag is set */
    if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_SB, RESET, Timeout) != 0)
    {
      if (Read_BIT(instance->CR1, I2C_CR1_START) == I2C_CR1_START)
      {

      }
      // return HAL_TIMEOUT;
      return 3;
    }

    /* Send slave address */
    instance->DR = I2C_7BIT_ADD_Write(DevAddress);

    tmp1 = I2C_GET_FLAG(instance, I2C_FLAG_ADDR);
    tmp2 = I2C_GET_FLAG(instance, I2C_FLAG_AF);

    while ((tmp1 == RESET) && (tmp2 == RESET))
    {
      tmp1 = I2C_GET_FLAG(instance, I2C_FLAG_ADDR);
      tmp2 = I2C_GET_FLAG(instance, I2C_FLAG_AF);
    }

    /* Check if the ADDR flag has been set */
    if (I2C_GET_FLAG(instance, I2C_FLAG_ADDR) == SET)
    {
      /* Generate Stop */
      ENABLE_BIT(instance->CR1, I2C_CR1_STOP);

      /* Clear ADDR Flag */
      I2C_CLEAR_ADDRFLAG(instance);

      /* Wait until BUSY flag is reset */
      if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG) != 0)
      {
        // return HAL_ERROR;
        return 1;
      }

      // return HAL_OK;
      return 0;
    }
    else
    {
      /* Generate Stop */
      ENABLE_BIT(instance->CR1, I2C_CR1_STOP);

      /* Clear AF Flag */
      I2C_CLEAR_FLAG(instance, I2C_FLAG_AF);

      /* Wait until BUSY flag is reset */
      if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG) != 0)
      {
        return 1;
      }
    }

    /* Increment Trials */
    I2C_Trials++;
  } while (I2C_Trials < Trials);
	
  return 1;
}

uint8_t I2C_MasterRequestRead(I2C_TypeDef *instance, uint16_t DevAddress, uint32_t Timeout)
{
  ENABLE_BIT(instance->CR1, I2C_CR1_ACK);

  ENABLE_BIT(instance->CR1, I2C_CR1_START);

  // I2C_StartBit_SetTime();
  if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_SB, RESET, Timeout) != 0)
  {
    // return HAL_TIMEOUT;
    return 3;
  }

  Write_REG(instance->DR, (uint8_t)(DevAddress | I2C_OAR1_ADD0));

  if (I2C_WaitOnMasterAddressFlagUntilTimeout(instance, I2C_SR1_ADDR, 10) != 0)
  {
    //    return HAL_ERROR;
    return 1;
  }
  return 0;
}

/**
 * @brief Эта функция ожидание передачи данных связи I2C.
 * @retval
 */
uint8_t I2C_MasterRequestWrite(I2C_TypeDef *instance, uint16_t DevAddress, uint32_t Timeout)
{
  ENABLE_BIT(instance->CR1, I2C_CR1_START);

  if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_SB, RESET, Timeout) != 0)
  {
    // return HAL_TIMEOUT;
    return 3;
  }

  Write_REG(instance->DR, (uint8_t)(DevAddress & (~I2C_OAR1_ADD0)));

  if (I2C_WaitOnMasterAddressFlagUntilTimeout(instance, I2C_SR1_ADDR, Timeout) != 0)
  {
    //    return HAL_ERROR;
    return 1;
  }

  return 0;
}

uint8_t I2C_GET_FLAG(I2C_TypeDef *instance, uint32_t flag)
{
  if ((flag >> 16) == 1)
  {
    if (((instance->SR1) & (flag & I2C_FLAG_MASK)) == (flag & I2C_FLAG_MASK))
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    if (((instance->SR2) & (flag & I2C_FLAG_MASK)) == (flag & I2C_FLAG_MASK))
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

void I2C_CLEAR_ADDRFLAG(I2C_TypeDef *instance)
{
    instance->SR1;
    instance->SR2;
}

void I2C_CLEAR_FLAG(I2C_TypeDef *instance, uint32_t flag)
{
  instance->SR1 = ~(flag & I2C_FLAG_MASK);
}

uint8_t I2C_7BIT_ADD_Write(uint8_t DevAddress)
{
  return (uint8_t)(DevAddress & (~I2C_OAR1_ADD0));
}

uint8_t I2C_Master_Receive(I2C_TypeDef *instance, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{

  /* Wait until BUSY flag is reset */
  if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG) != 0)
  {
    // return HAL_BUSY;
    return 2;
  }

  /* Disable Pos */
  DISABLE_BIT(instance->CR1, I2C_CR1_POS);

  /* Send Slave Address */
  if (I2C_MasterRequestRead(instance, DevAddress, Timeout) != 0)
  {
    return 1;
  }

  if (Size == 0U)
  {
    // сброс флага адреса
    I2C_CLEAR_ADDRFLAG(instance);

    /* Generate Stop */
    ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
  }
  else if (Size == 1U)
  {
    // откл проверку адреса
    DISABLE_BIT(instance->CR1, I2C_CR1_ACK);
		
    // сброс флага адреса
    I2C_CLEAR_ADDRFLAG(instance);

    /* Generate Stop */
    ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
  }
  else if (Size == 2U)
  {

    // откл проверку адреса
    DISABLE_BIT(instance->CR1, I2C_CR1_ACK);

    /* Enable Pos */
    ENABLE_BIT(instance->CR1, I2C_CR1_POS);

    // сброс флага адреса
    I2C_CLEAR_ADDRFLAG(instance);
  }
  else
  {

    // вкл проверку адреса
    ENABLE_BIT(instance->CR1, I2C_CR1_ACK);

    // сброс флага адреса
    I2C_CLEAR_ADDRFLAG(instance);
  }

  while (Size > 0U)
  {
    if (Size <= 3U)
    {
      /* One byte */
      if (Size == 1U)
      {
        I2C_WaitOnRXNEFlagUntilTimeout(instance);

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;
      }
      /* Two bytes */
      else if (Size == 2U)
      {

        // I2C_BTFBit_SetTime();
        if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BTF, RESET, Timeout) != 0)
        {
          // return HAL_ERROR;
          return 1;
        }

        /* Generate Stop */
        ENABLE_BIT(instance->CR1, I2C_CR1_STOP);

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;
      }
      /* 3 Last bytes */
      else
      {
        if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BTF, RESET, Timeout) != 0)
        {
          // return HAL_ERROR;
          return 1;
        }

        // откл проверку адреса
        DISABLE_BIT(instance->CR1, I2C_CR1_ACK);

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;

        if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BTF, RESET, Timeout) != 0)
        {
          // return HAL_ERROR;
          return 1;
        }

        /* Generate Stop */
        ENABLE_BIT(instance->CR1, I2C_CR1_STOP);

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;
      }
    }
    else
    {
      I2C_WaitOnRXNEFlagUntilTimeout(instance);
      /* Read data from DR */
      *pData = Read_REG(instance->DR);
      pData++;
      Size--;

      if (I2C_GET_FLAG(instance, I2C_FLAG_BTF) == SET)
      {
        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;
      }
    }
  }

  return 0;
}

uint8_t I2C_Master_Transmit(I2C_TypeDef *instance, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	ENABLE_BIT(instance->CR1,I2C_CR1_PE);

  /* Disable Pos */
  DISABLE_BIT(instance->CR1, I2C_CR1_POS);

  /* Send Slave Address */
  if (I2C_MasterRequestWrite(instance, DevAddress, Timeout) != 0)
  {
    return 1;
  }

  // сброс флага адреса
  I2C_CLEAR_ADDRFLAG(instance);

  while (Size > 0U)
  {
    /* Write data to DR */
    Write_REG(instance->DR, *pData);
    /* Increment Buffer pointer */
    pData++;
    Size--;

    // проверим данные
    if (I2C_WaitOnTXEFlagUntilTimeout(instance) != 0)
    {
      // откл проверку адреса
      DISABLE_BIT(instance->CR1, I2C_CR1_ACK);
      return 1;
    }
  }

  /* Generate Stop */
  ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
  return 0;
}

/**
 * @brief  Write an amount of data in blocking mode to a specific memory address
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  DevAddress Target device address: The device 7 bits address value
 *         in datasheet must be shifted to the left before calling the interface
 * @param  MemAddress Internal memory address
 * @param  MemAddSize Size of internal memory address
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @param  Timeout Timeout duration
 * @retval HAL status
 */

uint8_t I2C_Mem_Write(I2C_TypeDef *instance, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  /* Check the parameters */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));

  /* Wait until BUSY flag is reset */
  if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG) != 0)
  {
    // return HAL_BUSY;
    return 2;
  }

  /* Check if the I2C is already enabled */
	ENABLE_BIT(instance->CR1,I2C_CR1_PE);

  /* Disable Pos */
  DISABLE_BIT(instance->CR1, I2C_CR1_POS);

  /* Send Slave Address and Memory Address */
  if (I2C_RequestMemoryWrite(instance, DevAddress, MemAddress, MemAddSize, Timeout) != 0)
  {
    // return HAL_ERROR;
    return 1;
  }

  while (Size > 0U)
  {
    /* Wait until TXE flag is set */
    if (I2C_WaitOnTXEFlagUntilTimeout(instance) != 0)
    {
      // откл проверку адреса
      ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
      return 1;
    }

    /* Write data to DR */
    Write_REG(instance->DR, *pData);
    /* Increment Buffer pointer */
    pData++;
    Size--;

    if (I2C_GET_FLAG(instance, I2C_FLAG_BTF) == SET)
    {
      /* Write data to DR */
      Write_REG(instance->DR, *pData);
      /* Increment Buffer pointer */
      pData++;
      Size--;
    }

  }

  /* Wait until BTF flag is set */
  if (I2C_WaitOnBTFFlagUntilTimeout(instance, Timeout) != 0)
  {
    return 1;
  }

  /* Generate Stop */
  ENABLE_BIT(instance->CR1, I2C_CR1_STOP);

  return 0;
}

/*---------------------------------------------*/
/**
 * @brief  Read an amount of data in blocking mode from a specific memory address
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  DevAddress Target device address: The device 7 bits address value
 *         in datasheet must be shifted to the left before calling the interface
 * @param  MemAddress Internal memory address
 * @param  MemAddSize Size of internal memory address
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @param  Timeout Timeout duration
 * @retval HAL status
 */

uint8_t I2C_Mem_Read(I2C_TypeDef *instance, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  /* Check the parameters */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));

  /* Wait until BUSY flag is reset */
  if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG) != 0)
  {
    // return HAL_BUSY;
    return 2;
  }

  /* Enable I2C peripheral */
	ENABLE_BIT(instance->CR1,I2C_CR1_PE);


  /* Disable Pos */
  DISABLE_BIT(instance->CR1, I2C_CR1_POS);

  /* Send Slave Address and Memory Address */
  if (I2C_RequestMemoryRead(instance, DevAddress, MemAddress, MemAddSize, Timeout) != 0)
  {
    // return HAL_ERROR;
    return 1;
  }

  if (Size == 0U)
  {
    /* Clear ADDR flag */
    I2C_CLEAR_ADDRFLAG(instance);

    /* Generate Stop */
    ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
  }
  else if (Size == 1U)
  {
    /* Disable Acknowledge */
    DISABLE_BIT(instance->CR1, I2C_CR1_ACK);

    /* Clear ADDR flag */
    I2C_CLEAR_ADDRFLAG(instance);

    /* Generate Stop */
    ENABLE_BIT(instance->CR1, I2C_CR1_STOP);
  }
  else if (Size == 2U)
  {
    /* Disable Acknowledge */
    DISABLE_BIT(instance->CR1, I2C_CR1_ACK);

    /* Enable Pos */
    ENABLE_BIT(instance->CR1, I2C_CR1_POS);

    /* Clear ADDR flag */
    I2C_CLEAR_ADDRFLAG(instance);
  }
  else
  {
    /* Clear ADDR flag */
    I2C_CLEAR_ADDRFLAG(instance);
  }

  while (Size > 0U)
  {
    if (Size <= 3U)
    {
      /* One byte */
      if (Size == 1U)
      {
        /* Wait until RXNE flag is set */
        I2C_WaitOnRXNEFlagUntilTimeout(instance);

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;
      }
      /* Two bytes */
      else if (Size == 2U)
      {
        /* Wait until BTF flag is set */
        if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BTF, RESET, Timeout) != 0)
        {
          // return HAL_ERROR;
          return 1;
        }

        /* Generate Stop */
        ENABLE_BIT(instance->CR1, I2C_CR1_STOP);

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;
      }
      /* 3 Last bytes */
      else
      {
        /* Wait until BTF flag is set */
        if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BTF, RESET, Timeout) != 0)
        {
          // return HAL_ERROR;
          return 1;
        }

        /* Disable Acknowledge */
        DISABLE_BIT(instance->CR1, I2C_CR1_ACK);

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;

        /* Wait until BTF flag is set */
        if (I2C_WaitOnFlagUntilTimeout(instance, I2C_FLAG_BTF, RESET, Timeout) != 0)
        {
          // return HAL_ERROR;
          return 1;
        }

        /* Generate Stop */
        ENABLE_BIT(instance->CR1, I2C_CR1_STOP);

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;

        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;
      }
    }
    else
    {
      /* Wait until RXNE flag is set */
      I2C_WaitOnRXNEFlagUntilTimeout(instance);

      /* Read data from DR */
      *pData = Read_REG(instance->DR);
      pData++;
      Size--;

      if (I2C_GET_FLAG(instance, I2C_FLAG_BTF) == SET)
      {
        /* Read data from DR */
        *pData = Read_REG(instance->DR);
        pData++;
        Size--;
      }
    }
  }

  return 0;
}

////IRQ
void Error_Handler(void)
{
  //    __disable_irq();
  //    while (1)
  //    {
  //    }
}
