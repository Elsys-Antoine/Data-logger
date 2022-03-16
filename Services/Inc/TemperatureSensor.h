/*
 * TemperatureSensor.h
 *
 *  Created on: Feb 14, 2022
 *      Author: chevillard
 */

#ifndef INC_TEMPERATURESENSOR_H_
#define INC_TEMPERATURESENSOR_H_

/*
 * INCLUDE FILES
 */
#include "main.h"
#include "stm32f3xx_hal_adc_ex.h"
#include "stm32f3xx_hal_gpio.h"
/*
 * PUBLIC CONSTANT
 */

/*
 * PUBLIC GLOBAL VARIABLE
 */

/*
 * PUBLIC FUNCTION PROTOTYPES
 */
uint32_t TS_getTemperature(void);

#endif /* INC_TEMPERATURESENSOR_H_ */
