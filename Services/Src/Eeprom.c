/*
 * Eeprom.c
 *
 *  Created on: Mar 7, 2022
 *      Author: chevillard
 */

/*
 * INCLUDE FILES
 */
#include "Eeprom.h"

/*
 * PRIVATE CONSTANTS
 */

/*
 * PRIVATE GLOBAL VARIABLES
 */


/*
 * PRIVATE FUNCTION PROTOTYPES
 */
void EE_setSlaveSelect();
void EE_resetSlaveSelect();

/***************************************************************************************/

HAL_StatusTypeDef EE_Init(uint16_t status)
{
	HAL_StatusTypeDef state;

	HAL_SPIEx_FlushRxFifo(&hspi2);

	while(EE_isEEPROMBusy());

	state = EE_WriteStatusRegister(status);

	return state;
}

HAL_StatusTypeDef EE_SetWriteEnable()
{
	HAL_StatusTypeDef state;
	uint8_t tmp = WREN;


	state = HAL_SPI_Transmit(&hspi2, &tmp, 1, HAL_MAX_DELAY);

	EE_SPI_Disable();

	return state;
}

HAL_StatusTypeDef EE_ResetWriteEnable()
{
	HAL_StatusTypeDef state;
	uint8_t tmp = WRDI;

	while(EE_isEEPROMBusy());

	state = HAL_SPI_Transmit(&hspi2, &tmp, 1, HAL_MAX_DELAY);

	EE_SPI_Disable();

	return state;
}

HAL_StatusTypeDef EE_WriteStatusRegister(uint16_t status)
{
	HAL_StatusTypeDef state;
	uint8_t buf[3];

	buf[0] = WRSR;
	buf[1] = status&0xFF;
	buf[2] = (status >> 8)&0xFF;

	while(EE_isEEPROMBusy());

	EE_SetWriteEnable();

	state = HAL_SPI_Transmit(&hspi2, buf, 3, HAL_MAX_DELAY);

	EE_SPI_Disable();

	return state;
}

HAL_StatusTypeDef EE_ReadStatusRegister(uint16_t * status)
{
	HAL_StatusTypeDef state;
	uint8_t send = RDSR;
	uint8_t receive[2];

	state = HAL_SPI_Transmit(&hspi2, &send, 1, HAL_MAX_DELAY);
	state = HAL_SPI_Receive(&hspi2, receive, 2, HAL_MAX_DELAY);

	EE_SPI_Disable();

	*status = (receive[1]<<8) + receive[0];

	return state;
}

HAL_StatusTypeDef EE_Write(uint32_t addr, uint8_t * data, uint8_t length)
{
	HAL_StatusTypeDef state;
	uint8_t buf[length+4];


	if((addr&0xFF) + length >= EE_SIZE_PAGE)
	{
		//Ecriture au dela de la fin de la page
		buf[0] = WRITE;

		//address setting 24 bits of address
		buf[1] = (addr >> 16) & 0x07;
		buf[2] = (addr >> 8) & 0xFF;
		buf[3] = addr & 0xFF;

		for(uint8_t i = 0; i<(EE_SIZE_PAGE - (addr&0xFF)); i++)
		{
			buf[4+i] = data[i];
		}

		while(EE_isEEPROMBusy());

		state = EE_SetWriteEnable();

		state = HAL_SPI_Transmit(&hspi2, buf, (EE_SIZE_PAGE - (addr&0xFF)) + 4, HAL_MAX_DELAY);

		EE_SPI_Disable();

		//Changement de page
		uint16_t page = (addr >> 8) & 0x7FF;
		page++;

		if(page > EE_LAST_PAGE)
		{
			//Revenir sur la première page
			page = 0x00;
		}

		buf[1] = (page >> 8) & 0x0F;
		buf[2] = page & 0xFF;
		buf[3] = 0x00;

		for(uint8_t i=0; i<length - (EE_SIZE_PAGE - (addr&0xFF)); i++)
		{
			buf[4+i] = data[(EE_SIZE_PAGE - (addr&0xFF))+i];
		}

		while(EE_isEEPROMBusy());

		state = EE_SetWriteEnable();

		state = HAL_SPI_Transmit(&hspi2, buf, length - (EE_SIZE_PAGE - (addr&0xFF)) + 4, HAL_MAX_DELAY);

		EE_SPI_Disable();
		while(EE_isEEPROMBusy());

	}
	else
	{
		//Ecriture sur une page
		buf[0] = WRITE;

		//address setting 24 bits of address
		buf[1] = (addr >> 16) & 0x0F;
		buf[2] = (addr >> 8) & 0xFF;
		buf[3] = addr & 0xFF;

		for(uint8_t i = 0; i<length; i++)
		{
			buf[4+i] = data[i];
		}

		while(EE_isEEPROMBusy());

		state = EE_SetWriteEnable();

		state = HAL_SPI_Transmit(&hspi2, buf, length+4, HAL_MAX_DELAY);

		EE_SPI_Disable();

		while(EE_isEEPROMBusy());
	}


	return state;
}

HAL_StatusTypeDef EE_Read(uint32_t addr, uint8_t * data, uint8_t length)
{
	HAL_StatusTypeDef state;
	uint8_t send[4], receive[length];
	//Lecture sur une page
	send[0] = READ;

	//address setting 24 bits of address
	send[1] = (addr >> 16) & 0x0F;
	send[2] = (addr >> 8) & 0xFF;
	send[3] = addr & 0xFF;

	state = HAL_SPI_Transmit(&hspi2, send, 4, HAL_MAX_DELAY);
	state = HAL_SPI_Receive(&hspi2, receive, length, HAL_MAX_DELAY);

	EE_SPI_Disable();

	for(uint8_t i = 0; i<length; i++)
	{
		data[i] = receive[i];
	}

	return state;
}


uint8_t EE_isEEPROMBusy()
{
	uint8_t state;
	uint8_t status = 0;

	uint8_t buf = WRBP;

	HAL_SPI_Transmit(&hspi2, &buf, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi2, &status, 1, HAL_MAX_DELAY);
	EE_SPI_Disable();

	if(status)
	{
		state = EEPROM_BUSY;
	}
	else
	{
		state = EEPROM_NOT_BUSY;
	}

	return state;
}


void EE_getID(uint8_t * id)
{
	uint8_t buf = SPID;

	HAL_SPI_Transmit(&hspi2, &buf, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi2, id, 5, HAL_MAX_DELAY);

	EE_SPI_Disable();
}


void EE_SPI_Disable()
{
	//Check if FTLVL is equal to 0
	while((hspi2.Instance->SR & SPI_SR_FTLVL) != SPI_FTLVL_EMPTY);

	while((hspi2.Instance->SR & SPI_SR_BSY) == SPI_SR_BSY);

	/* Disable the SPI to let the NSS lign */
	if ((hspi2.Instance->CR1 & SPI_CR1_SPE) == SPI_CR1_SPE)
	{
		/* Disable SPI peripheral */
		__HAL_SPI_DISABLE(&hspi2);
	}

	HAL_SPIEx_FlushRxFifo(&hspi2);

}
