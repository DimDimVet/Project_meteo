#ifndef FLASH_F407_H_
#define FLASH_F407_H_

/*includes*/
#include "stm32f4xx.h"

/*defaines*/
#define FLASH_SECTOR_0     ((uint16_t)0x0000) /*!< Sector Number 0   */
#define FLASH_SECTOR_1     ((uint16_t)0x0008) /*!< Sector Number 1   */
#define FLASH_SECTOR_2     ((uint16_t)0x0010) /*!< Sector Number 2   */
#define FLASH_SECTOR_3     ((uint16_t)0x0018) /*!< Sector Number 3   */
#define FLASH_SECTOR_4     ((uint16_t)0x0020) /*!< Sector Number 4   */
#define FLASH_SECTOR_5     ((uint16_t)0x0028) /*!< Sector Number 5   */
#define FLASH_SECTOR_6     ((uint16_t)0x0030) /*!< Sector Number 6   */
#define FLASH_SECTOR_7     ((uint16_t)0x0038) /*!< Sector Number 7   */

#define FLASH_SECTOR_8     ((uint16_t)0x0040) /*!< Sector Number 8   */
#define FLASH_SECTOR_9     ((uint16_t)0x0048) /*!< Sector Number 9   */
#define FLASH_SECTOR_10    ((uint16_t)0x0050) /*!< Sector Number 10  */
#define FLASH_SECTOR_11    ((uint16_t)0x0058) /*!< Sector Number 11  */
#define FLASH_SECTOR_12    ((uint16_t)0x0080) /*!< Sector Number 12  */
#define FLASH_SECTOR_13    ((uint16_t)0x0088) /*!< Sector Number 13  */
#define FLASH_SECTOR_14    ((uint16_t)0x0090) /*!< Sector Number 14  */
#define FLASH_SECTOR_15    ((uint16_t)0x0098) /*!< Sector Number 15  */
#define FLASH_SECTOR_16    ((uint16_t)0x00A0) /*!< Sector Number 16  */
#define FLASH_SECTOR_17    ((uint16_t)0x00A8) /*!< Sector Number 17  */
#define FLASH_SECTOR_18    ((uint16_t)0x00B0) /*!< Sector Number 18  */
#define FLASH_SECTOR_19    ((uint16_t)0x00B8) /*!< Sector Number 19  */
#define FLASH_SECTOR_20    ((uint16_t)0x00C0) /*!< Sector Number 20  */
#define FLASH_SECTOR_21    ((uint16_t)0x00C8) /*!< Sector Number 21  */
#define FLASH_SECTOR_22    ((uint16_t)0x00D0) /*!< Sector Number 22  */
#define FLASH_SECTOR_23    ((uint16_t)0x00D8) /*!< Sector Number 23  */

#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

#define SECTOR_MASK               ((uint32_t)0xFFFFFF07)

#define FLASH_FLAG_EOP                 ((uint32_t)0x00000001)  /*!< FLASH End of Operation flag               */
#define FLASH_FLAG_OPERR               ((uint32_t)0x00000002)  /*!< FLASH operation Error flag                */
#define FLASH_FLAG_WRPERR              ((uint32_t)0x00000010)  /*!< FLASH Write protected error flag          */
#define FLASH_FLAG_PGAERR              ((uint32_t)0x00000020)  /*!< FLASH Programming Alignment error flag    */
#define FLASH_FLAG_PGPERR              ((uint32_t)0x00000040)  /*!< FLASH Programming Parallelism error flag  */
#define FLASH_FLAG_PGSERR              ((uint32_t)0x00000080)  /*!< FLASH Programming Sequence error flag     */
#define FLASH_FLAG_RDERR               ((uint32_t)0x00000100)  /*!< Read Protection error flag (PCROP)        */
#define FLASH_FLAG_BSY                 ((uint32_t)0x00010000)  /*!< FLASH Busy flag */

/** @defgroup FLASH_Voltage_Range 
  * @{
  */ 
#define VoltageRange_1        ((uint8_t)0x00)  /*!< Device operating range: 1.8V to 2.1V */
#define VoltageRange_2        ((uint8_t)0x01)  /*!<Device operating range: 2.1V to 2.7V */
#define VoltageRange_3        ((uint8_t)0x02)  /*!<Device operating range: 2.7V to 3.6V */
#define VoltageRange_4        ((uint8_t)0x03)  /*!<Device operating range: 2.7V to 3.6V + External Vpp */

/** @defgroup FLASH_Program_Parallelism   
  * @{
  */
#define FLASH_PSIZE_BYTE           ((uint32_t)0x00000000)
#define FLASH_PSIZE_HALF_WORD      ((uint32_t)0x00000100)
#define FLASH_PSIZE_WORD           ((uint32_t)0x00000200)
#define FLASH_PSIZE_DOUBLE_WORD    ((uint32_t)0x00000300)
#define CR_PSIZE_MASK              ((uint32_t)0xFFFFFCFF)

/*var*/

/*structures*/
typedef enum
{ 
  FLASH_BUSY_ = 1,
  FLASH_ERROR_RD_ = 2,
  FLASH_ERROR_PGS_ = 3,
  FLASH_ERROR_PGP_ = 4,
  FLASH_ERROR_PGA_ = 5,
  FLASH_ERROR_WRP_ = 6,
  FLASH_ERROR_PROGRAM_ = 7,
  FLASH_ERROR_OPERATION_ = 8,
  FLASH_COMPLETE_ = 9
}FLASH_Status;

/*func*/
FLASH_Status FLASH_GetStatusR(void);
FLASH_Status FLASH_WaitForLastOperation(void);
FLASH_Status FLASH_EraseSector(uint32_t FLASH_Sector, uint8_t VoltageRange);
void FLASH_Unlock(void);
void FLASH_Lock(void);
uint32_t FLASH_Read(uint32_t address);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
uint32_t GetSector(uint32_t address);

#endif