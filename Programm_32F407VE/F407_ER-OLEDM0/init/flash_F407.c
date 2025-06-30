#include "flash_F407.h"

FLASH_Status FLASH_GetStatusR(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE_;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY_;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_WRPERR) != (uint32_t)0x00)
    { 
      flashstatus = FLASH_ERROR_WRP_;
    }
    else
    {
      if((FLASH->SR & FLASH_FLAG_RDERR) != (uint32_t)0x00)
      { 
        flashstatus = FLASH_ERROR_RD_;
      } 
      else 
      {
        if((FLASH->SR & (uint32_t)0xE0) != (uint32_t)0x00)
        {
          flashstatus = FLASH_ERROR_PROGRAM_; 
        }
        else
        {
          if((FLASH->SR & FLASH_FLAG_OPERR) != (uint32_t)0x00)
          {
            flashstatus = FLASH_ERROR_OPERATION_;
          }
          else
          {
            flashstatus = FLASH_COMPLETE_;
          }
        }
      }
    }
  }

  return flashstatus;
}


FLASH_Status FLASH_WaitForLastOperation(void)
{ 
	
  FLASH_Status status = FLASH_COMPLETE_;

  status = FLASH_GetStatusR();

  while(status == FLASH_BUSY_)
  {
    status = FLASH_GetStatusR();
  }

  return status;
}

void FLASH_Unlock(void)
{
    if((FLASH->CR & FLASH_CR_LOCK) != 0)
    {
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
}

void FLASH_Lock(void)
{
  FLASH->CR |= FLASH_CR_LOCK;
}

uint32_t FLASH_Read(uint32_t address)
{
    return (*(uint32_t*)address);
}

FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Status status = FLASH_COMPLETE_;
    FLASH->CR &= FLASH_CR_PG;

    status = FLASH_WaitForLastOperation();

    if(status == FLASH_COMPLETE_)
    {
        FLASH->CR &= CR_PSIZE_MASK;
        FLASH->CR |= FLASH_PSIZE_WORD;
        FLASH->CR |= FLASH_CR_PG;

        *(uint32_t*)Address = Data;

        status = FLASH_WaitForLastOperation();

        FLASH->CR &= (~FLASH_CR_PG);
    }

    return status;
}

FLASH_Status FLASH_EraseSector(uint32_t FLASH_Sector, uint8_t VoltageRange)
{
  uint32_t tmp_psize = 0x0;
  FLASH_Status status = FLASH_COMPLETE_;
  
  if(VoltageRange == VoltageRange_1)
  {
     tmp_psize = FLASH_PSIZE_BYTE;
  }
  else if(VoltageRange == VoltageRange_2)
  {
    tmp_psize = FLASH_PSIZE_HALF_WORD;
  }
  else if(VoltageRange == VoltageRange_3)
  {
    tmp_psize = FLASH_PSIZE_WORD;
  }
  else
  {
    tmp_psize = FLASH_PSIZE_DOUBLE_WORD;
  }

  status = FLASH_WaitForLastOperation();
  
  if(status == FLASH_COMPLETE_)
  { 

    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= tmp_psize;
    FLASH->CR &= SECTOR_MASK;
    FLASH->CR |= FLASH_CR_SER | FLASH_Sector;
    FLASH->CR |= FLASH_CR_STRT;

    status = FLASH_WaitForLastOperation();

    FLASH->CR &= (~FLASH_CR_SER);
    FLASH->CR &= SECTOR_MASK; 
  }

  return status;
}

uint32_t GetSector(uint32_t address)
{
    if (address < 0x08004000) return FLASH_SECTOR_0; // Адрес 0x08000000 - 0x08003FFF
    else if (address < 0x08008000) return FLASH_SECTOR_1; // 0x08004000 - 0x08007FFF
    else if (address < 0x0800C000) return FLASH_SECTOR_2; // 0x08008000 - 0x0800BFFF
    else if (address < 0x08010000) return FLASH_SECTOR_3; // 0x0800C000 - 0x0800FFFF
    else if (address < 0x08020000) return FLASH_SECTOR_4; // 0x08010000 - 0x0801FFFF
    else if (address < 0x08040000) return FLASH_SECTOR_5; // 0x08020000 - 0x0803FFFF
    else if (address < 0x08060000) return FLASH_SECTOR_6; // 0x08040000 - 0x0805FFFF
    else return FLASH_SECTOR_7; // 0x08060000 - 0x080FFFFF
}