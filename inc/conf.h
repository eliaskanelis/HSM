/******************************************************************************
	About
******************************************************************************/

/**
 * \file conf.h
 *
 * \brief	Board specific Configurations
 *
 * Created:			09/10/2017
 *
 * \author	Ilias Kanelis	hkanelhs@yahoo.gr
 */

/**
* \defgroup	Ungrouped	Ungrouped
*
* \code	#include <conf.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef CONF_H_ONLY_ONE_INCLUDE_SAFETY
#define CONF_H_ONLY_ONE_INCLUDE_SAFETY

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
	Include files
******************************************************************************/

#include "drivers/peripherals/mcp3421.h"	/* MCP3421_ADDRESS_A1 */
#include "drivers/peripherals/mma8452q.h"	/* MMA8452Q_ADDR_SA0_0 */

/******************************************************************************
	Custom definitions
******************************************************************************/

/* LED */
#define LED_GPIO_PORT				GP_GPIOA
#define LED_GPIO_PIN				5

/* BUTTON */
#define BUTTON_GPIO_PORT			GP_GPIOC
#define BUTTON_GPIO_PIN				13

/* CONSOLE */
#define CONSOLE_UART_PORT			SP_UART2
#define CONSOLE_UART_BAUDRATE		115200U

#define CONSOLE_RX_GPIO_PORT		GP_GPIOA
#define CONSOLE_RX_GPIO_PIN			3
#define CONSOLE_RX_GPIO_ALTFUN		GM_AF1_USART2

#define CONSOLE_TX_GPIO_PORT		GP_GPIOA
#define CONSOLE_TX_GPIO_PIN			2
#define CONSOLE_TX_GPIO_ALTFUN		GM_AF1_USART2

/* I2C 1 */
#define I2C1_SDA_GPIO_PORT			GP_GPIOB
#define I2C1_SDA_GPIO_PIN			9
#define I2C1_SDA_GPIO_ALTFUN		GM_AF1_I2C1

#define I2C1_SCL_GPIO_PORT			GP_GPIOB
#define I2C1_SCL_GPIO_PIN			8
#define I2C1_SCL_GPIO_ALTFUN		GM_AF1_I2C1

#define WEIGHT_SENSOR_ADDRESS		MCP3421_ADDR_A1

#define ACCELEROMETER_ADDRESS		MMA8452Q_ADDR_SA0_0

#ifdef __cplusplus
}
#endif

#endif /* CONF_H_ONLY_ONE_INCLUDE_SAFETY */
