/*
 * Eeprom.h
 *
 *  Created on: Mar 7, 2022
 *      Author: chevillard
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

/*
 * INCLUDE FILES
 */
#include "main.h"
#include "stm32f3xx_hal_spi.h"
#include "stm32f3xx_hal_gpio.h"

/*
 * PUBLIC CONSTANT
 */

#define EEPROM_NOT_BUSY			0
#define EEPROM_BUSY				1

#define EEPROM_BUSY_FLAG_MASK	0x0001


#define EEPROM_SPI_DUMMY		0x5555

#define EE_SIZE_PAGE			0x100
#define EE_LAST_PAGE			0x7FF

typedef enum
{
	RDSR	= 0x05,		// Read Status register
	WRBP 	= 0x08,		// Write Ready/Busy Poll
	WREN 	= 0x06,		// Set Write Enable Latch
	WRDI 	= 0x04,		// Reset Write Enable Latch
	WRSR 	= 0x01,		// Write Status register

	READ 	= 0x03,		// Read from EEPROM array
	WRITE 	= 0x02,		// Write to EEPROM array (1 to 256 bytes)
	RDEX	= 0x83,		// Read from security register
	WREX	= 0x82,		// Write to security register
	LOCK	= 0x82,		// Lock the Security Register (permanent)
	CHLK	= 0x83,		// Check Lock Status of Security Register

	RMPR	= 0x31,		// Read Contents of Memory Partition Registers
	PRWE	= 0x07,		// Set Memory Partition Write Enable Latch
	PRWD	= 0x0A,		// Reset Memory Partition Write Enable Latch
	WMPR	= 0x32,		// Write Memory Partition Registers
	PPAB	= 0x34,		// Protect Partition Address Boundaries
	FRZR	= 0x37,		// Freeze Memory Protection Configuration (permanent)

	SPID	= 0x9F, 	// Read SPI Manufacturer ID Data

	SRST	= 0x7C		// Software Device Reset
}EE_opcode_t;
/*
 * PUBLIC TYPE DEFINITION
 */
/*
 * PUBLIC GLOBAL VARIABLE
 */

/*
 * PUBLIC FUNCTION PROTOTYPES
 */
HAL_StatusTypeDef EE_Init(uint16_t status);


HAL_StatusTypeDef EE_WriteStatusRegister(uint16_t status);
HAL_StatusTypeDef EE_ReadStatusRegister(uint16_t * status);
HAL_StatusTypeDef EE_Write(uint32_t addr, uint8_t * data, uint16_t length);
HAL_StatusTypeDef EE_Read(uint32_t addr, uint8_t * data, uint16_t length);

void EE_getID(uint8_t *);



#endif /* INC_EEPROM_H_ */
