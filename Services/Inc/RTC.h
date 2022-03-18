/*
 * RTC.h
 *
 *  Created on: Feb 15, 2022
 *      Author: chevillard
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

/*
 * INCLUDE FILES
 */
#include "main.h"
#include "stm32f3xx_hal_i2c.h"


/*
 * PUBLIC CONSTANT
 */
#define RTC_addr	(0x68&0xFF)<<1

#define HOUR_TYPE_24H	0
#define HOUR_TYPE_12H	1


/*
 * PUBLIC TYPE DEFINITION
 */
typedef enum
{
	SQW_OFF_0 	= 0x00,			// Signal squarewave is off and the lign is low
	SQW_OFF_1 	= 0x80,			// Signal squarewave is off and the lign is high
	SQW_1Hz		= 0x10,			// Signal squarewave at a frequency of 1Hz
	SQW_4kHz	= 0x11,			// Signal squarewave at a frequency of 4kHz
	SQW_8kHz	= 0x12,			// Signal squarewave at a frequency of 8kHz
	SQW_32kHz	= 0x13			// Signal squarewave at a frequency of 32kHz
}SQW_t;

typedef enum
{
	AM_PM_NONE 	= 0,
	AM_12H		= 1,
	PM_12H		= 2
}TIME_12H_t;

/*
 * RTC_Date_t definition
 * year		: Years to set on the RTC
 * month	: Month to set on the RTC
 * day		: Day to set on the RTC
 * hour		: Hour to set on the RTC
 * minutes	: Minute to set on the RTC
 * seconds	: Second to set on the RTC
 */
typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t dateNumber;
	uint8_t day;
	uint8_t hour;
	uint8_t minutes;
	uint8_t seconds;

	uint8_t hourMode;
	TIME_12H_t timeMode;
} RTC_Date_t;


/*
 * ASSERTION
 */
#define IS_SECONDS_MINUTES(val)		((val) >= 0 && (val) <= 59)
#define IS_12HOUR(val)				((val) >= 1 && (val) <= 12)
#define IS_24HOUR(val)				((val) >= 0 && (val) <= 23)
#define IS_DAY(val)					((val) >= 1 && (val) <= 7)
#define IS_DATE(val)				((val) >= 1 && (val) <= 31)
#define IS_MONTH(val)				((val) >= 1 && (val) <= 12)
#define IS_YEAR(val)				((val) >= 0 && (val) <= 99)
#define IS_HOUR_MODE(val)			((val) == HOUR_TYPE_24H || (val) == HOUR_TYPE_12H)
#define IS_TIME(val)				((val) == AM_PM_NONE ||\
									(val) == AM_12H ||\
									(val) == PM_12H)

/*
 * PUBLIC GLOBAL VARIABLE
 */

/*
 * PUBLIC FUNCTION PROTOTYPES
 */
HAL_StatusTypeDef RTC_Init(RTC_Date_t RTC_Date, SQW_t squareWave);

HAL_StatusTypeDef RTC_changeHourMode(void);

/***************************************************************************************/
/************************************** GETTERS ****************************************/
/***************************************************************************************/
HAL_StatusTypeDef RTC_getDate(RTC_Date_t * RTC_Date);
HAL_StatusTypeDef RTC_getYear(uint8_t * year);
HAL_StatusTypeDef RTC_getMonth(uint8_t * month);
HAL_StatusTypeDef RCT_getDateNumber(uint8_t * dayNumber);
HAL_StatusTypeDef RTC_getDay(uint8_t * day);
HAL_StatusTypeDef RTC_getHour(uint8_t * hour, uint8_t * hourMode, TIME_12H_t * time);
HAL_StatusTypeDef RTC_getMinutes(uint8_t * minutes);
HAL_StatusTypeDef RTC_getSeconds(uint8_t * seconds);

/***************************************************************************************/
/************************************** SETTERS ****************************************/
/***************************************************************************************/
HAL_StatusTypeDef RTC_setDate(RTC_Date_t RTC_Date);
HAL_StatusTypeDef RTC_setYear(uint8_t year);
HAL_StatusTypeDef RTC_setMonth(uint8_t month);
HAL_StatusTypeDef RTC_setDateNumber(uint8_t date);
HAL_StatusTypeDef RTC_setDay(uint8_t day);
HAL_StatusTypeDef RTC_setHour(uint8_t hour, uint8_t hourMode, TIME_12H_t time);
HAL_StatusTypeDef RTC_setMinutes(uint8_t minutes);
HAL_StatusTypeDef RTC_setSeconds(uint8_t seconds);

#endif /* INC_RTC_H_ */
