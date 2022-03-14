/*
 * RTC.c
 *
 *  Created on: Feb 15, 2022
 *      Author: chevillard
 */


/*
 * INCLUDE FILES
 */
#include "RTC.h"

/*
 * PRIVATE CONSTANTS
 */

/*
 * PRIVATE GLOBAL VARIABLES
 */


/*
 * PRIVATE FUNCTION PROTOTYPES
 */
uint8_t bcd2bin(uint8_t value);
uint8_t bin2bcd(uint8_t value);


/***************************************************************************************/
/*
 * RTC_Init
 * @brief
 * Initialize the RTC module
 * @param
 * RTC_Date		:	Structure of RTC date
 * squareWave	:	Shape of the SWQ/OUT pin of the RTC
 * @return
 * None
 */
HAL_StatusTypeDef RTC_Init(RTC_Date_t RTC_Date, SQW_t squareWave)
{
	HAL_StatusTypeDef state;
	uint8_t buf[2];

	buf[0] = 0x00;

	//Enable Clock
 	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, &buf[0], 1, HAL_MAX_DELAY);
	state = HAL_I2C_Master_Receive(&hi2c1, RTC_addr, &buf[1], 1, HAL_MAX_DELAY);
	buf[1] &= 0x7F;
	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, buf, 2, HAL_MAX_DELAY);

	//Setup the squarewave output
	buf[0] = 0x07;
	buf[1] = (uint8_t)squareWave;
	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, buf, 2, HAL_MAX_DELAY);

	//Setup the RTC date
	RTC_setDate(RTC_Date);

	return state;
}

HAL_StatusTypeDef RTC_changeHourMode(void)
{
	HAL_StatusTypeDef state;
	uint8_t tmp[3];
	state = RTC_getHour(&tmp[0], &tmp[1], &tmp[2]);

	if(tmp[1])
	{
		//passage en mode 12H
		if(tmp[0] > 12)
		{
			tmp[2] = PM_12H;
		}
		else
		{
			tmp[2] = AM_12H;
		}
		tmp[0] %= 12;
		tmp[1] = HOUR_TYPE_12H;
	}
	else
	{
		if(tmp[2] == PM_12H)
		{
			tmp[0] = bin2bcd(bcd2bin(tmp[0])+12);
		}
		tmp[2] = AM_PM_NONE;
		tmp[1] = HOUR_TYPE_24H;
	}

	state = RTC_setHour(tmp[0], tmp[1], tmp[2]);
	return state;
}

/***************************************************************************************/
/************************************** GETTERS ****************************************/
/***************************************************************************************/
HAL_StatusTypeDef RTC_getDate(RTC_Date_t * RTC_Date)
{
	HAL_StatusTypeDef state;
	state = RTC_getSeconds(&RTC_Date->seconds);
	state = RTC_getMinutes(&RTC_Date->minutes);
	state = RTC_getHour(&RTC_Date->hour, &RTC_Date->hourMode, &RTC_Date->timeMode);
	state = RTC_getDay(&RTC_Date->day);
	state = RCT_getDateNumber(&RTC_Date->dateNumber);
	state = RTC_getMonth(&RTC_Date->month);
	state = RTC_getYear(&RTC_Date->year);
	return state;
}

HAL_StatusTypeDef RTC_getYear(uint8_t * year)
{
	HAL_StatusTypeDef state;
	*year = 0x06;
	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, year, 1, HAL_MAX_DELAY);
	state = HAL_I2C_Master_Receive(&hi2c1, RTC_addr, year, 1, HAL_MAX_DELAY);
	*year = bcd2bin(*year);
	return state;
}

HAL_StatusTypeDef RTC_getMonth(uint8_t * month)
{
	HAL_StatusTypeDef state;
	*month = 0x05;
	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, month, 1, HAL_MAX_DELAY);
	state = HAL_I2C_Master_Receive(&hi2c1, RTC_addr, month, 1, HAL_MAX_DELAY);
	*month = bcd2bin(*month);
	return state;
}

HAL_StatusTypeDef RCT_getDateNumber(uint8_t * dateNumber)
{
	HAL_StatusTypeDef state;
	*dateNumber = 0x04;
	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, dateNumber, 1, HAL_MAX_DELAY);
	state = HAL_I2C_Master_Receive(&hi2c1, RTC_addr, dateNumber, 1, HAL_MAX_DELAY);
	*dateNumber = bcd2bin(*dateNumber);
	return state;
}

HAL_StatusTypeDef RTC_getDay(uint8_t * day)
{
	HAL_StatusTypeDef state;
	*day = 0x03;
	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, day, 1, HAL_MAX_DELAY);
	state = HAL_I2C_Master_Receive(&hi2c1, RTC_addr, day, 1, HAL_MAX_DELAY);
	return state;
}

HAL_StatusTypeDef RTC_getHour(uint8_t * hour, uint8_t * hourMode, TIME_12H_t * time)
{
	HAL_StatusTypeDef state;
	*hour = 0x02;
	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, hour, 1, HAL_MAX_DELAY);
	state = HAL_I2C_Master_Receive(&hi2c1, RTC_addr, hour, 1, HAL_MAX_DELAY);
	*hourMode = (*hour & 0x40)>>6;
	if(*hourMode)
	{
		//12H
		*time = (*hour & 0x20)>>5;
		*hour = bcd2bin((*hour & 0x1F));
	}
	else
	{
		//24H
		*time = AM_PM_NONE;
		*hour = bcd2bin((*hour & 0x3F));
	}
	return state;
}

HAL_StatusTypeDef RTC_getMinutes(uint8_t * minutes)
{
	HAL_StatusTypeDef state;
	*minutes = 0x01;
	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, minutes, 1, HAL_MAX_DELAY);
	state = HAL_I2C_Master_Receive(&hi2c1, RTC_addr, minutes, 1, HAL_MAX_DELAY);
	*minutes = bcd2bin(*minutes);
	return state;
}

HAL_StatusTypeDef RTC_getSeconds(uint8_t * seconds)
{
	HAL_StatusTypeDef state;
	*seconds = 0x00;
	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, seconds, 1, HAL_MAX_DELAY);
	state = HAL_I2C_Master_Receive(&hi2c1, RTC_addr, seconds, 1, HAL_MAX_DELAY);
	*seconds = bcd2bin(*seconds);
	return state;
}


/***************************************************************************************/
/************************************** SETTERS ****************************************/
/***************************************************************************************/
HAL_StatusTypeDef RTC_setDate(RTC_Date_t RTC_Date)
{
	HAL_StatusTypeDef state;
	state = RTC_setSeconds(RTC_Date.seconds);
	state = RTC_setMinutes(RTC_Date.minutes);
	state = RTC_setHour(RTC_Date.hour, RTC_Date.hourMode, RTC_Date.timeMode);
	state = RTC_setDay(RTC_Date.day);
	state = RTC_setDateNumber(RTC_Date.dateNumber);
	state = RTC_setMonth(RTC_Date.month);
	state = RTC_setYear(RTC_Date.year);
	return state;
}

HAL_StatusTypeDef RTC_setYear(uint8_t year)
{
	assert_param(IS_YEAR(year));

	HAL_StatusTypeDef state;
	uint8_t buf[2];
	buf[0] = 0x06;
	buf[1] = bin2bcd(year);

	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, buf, 2, HAL_MAX_DELAY);
	return state;
}

HAL_StatusTypeDef RTC_setMonth(uint8_t month)
{
	assert_param(IS_MONTH(month));

	HAL_StatusTypeDef state;
	uint8_t buf[2];
	buf[0] = 0x05;
	buf[1] = bin2bcd(month);

	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, buf, 2, HAL_MAX_DELAY);
	return state;
}

HAL_StatusTypeDef RTC_setDateNumber(uint8_t date)
{
	assert_param(IS_DATE(date));

	HAL_StatusTypeDef state;
	uint8_t buf[2];
	buf[0] = 0x04;
	buf[1] = bin2bcd(date);

	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, buf, 2, HAL_MAX_DELAY);
	return state;
}

HAL_StatusTypeDef RTC_setDay(uint8_t day)
{
	assert_param(IS_DAY(day));

	HAL_StatusTypeDef state;
	uint8_t buf[2];
	buf[0] = 0x03;
	buf[1] = bin2bcd(day);

	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, buf, 2, HAL_MAX_DELAY);
	return state;
}

HAL_StatusTypeDef RTC_setHour(uint8_t hour, uint8_t hourMode, TIME_12H_t time)
{
	assert_param(IS_HOUR_MODE(hourMode));
	assert_param(IS_TIME(time));


	HAL_StatusTypeDef state;
	uint8_t buf[2];
	buf[0] = 0x02;
	buf[1] = bin2bcd(hour);

	if(hourMode)
	{
		//12H
		assert_param(IS_12HOUR(hour));

		buf[1] |= 0x40;
		switch (time) {
			case PM_12H:
				buf[1] |= 0x20;
				break;
			case AM_12H:
				buf[1] &= 0x5F;
				break;
			default:
				return HAL_ERROR;
		}
	}
	else
	{
		//24H
		assert_param(IS_24HOUR(hour));

		buf[1] &= 0x3F;
	}

	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, buf, 2, HAL_MAX_DELAY);
	return state;
}

HAL_StatusTypeDef RTC_setMinutes(uint8_t minutes)
{
	assert_param(IS_SECONDS_MINUTES(minutes));

	HAL_StatusTypeDef state;
	uint8_t buf[2];
	buf[0] = 0x01;
	buf[1] = bin2bcd(minutes);

	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, buf, 2, HAL_MAX_DELAY);
	return state;
}

HAL_StatusTypeDef RTC_setSeconds(uint8_t seconds)
{
	assert_param(IS_SECONDS_MINUTES(seconds));

	HAL_StatusTypeDef state;
	uint8_t buf[2];
	buf[0] = 0x00;

	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, buf, 1, HAL_MAX_DELAY);
	state = HAL_I2C_Master_Receive(&hi2c1, RTC_addr, &buf[1], 1, HAL_MAX_DELAY);
	if((buf[1]&0x80)>>7)
	{
		buf[1] = bin2bcd(seconds)|0x80;
	}
	else
	{
		buf[1] = bin2bcd(seconds)&0x7F;
	}
	state = HAL_I2C_Master_Transmit(&hi2c1, RTC_addr, buf, 2, HAL_MAX_DELAY);
	return state;
}

/***************************************************************************************/
/************************************** CONVERSION *************************************/
/***************************************************************************************/
uint8_t bcd2bin(uint8_t value)
{
	return value - 6 * (value >> 4);
}

uint8_t bin2bcd(uint8_t value)
{
	return value + 6 * (value / 10);
}

