/*
 * TemperatureSensor.c
 *
 *  Created on: Feb 14, 2022
 *      Author: chevillard
 */


/*
 * INCLUDE FILES
 */
#include "TemperatureSensor.h"

/*
 * PRIVATE CONSTANTS
 */

/*
 * PRIVATE GLOBAL VARIABLES
 */


/*
 * PRIVATE FUNCTION PROTOTYPES
 */


/***************************************************************************************/

/* @function
 * TS_getTemperature
 *
 * @brief
 * This function get the temperature value.
 * First, it starts the ADC, then converts the voltage in a unsigned integer value it gets and then fetch the integer value.
 * Last, it converts again the integer value to celcius degree.
 *
 * @param
 * None
 *
 * @return
 * Value of the temperature
 */
uint32_t TS_getTemperature(void)
{
	uint32_t temp;

	HAL_ADC_Start(&hadc1);

	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

	temp = HAL_ADC_GetValue(&hadc1);

	HAL_ADC_Stop(&hadc1);

	temp = temp / 10 - 50;

	return  temp;
}
