/******************************************************************************
	About
******************************************************************************/

/**
 * \file console.h
 *
 * \brief       Console low level driver.
 *
 * It uses the Proxy design pattern.
 *
 * Created:     30/04/2017
 *
 * \author      Elias Kanelis    hkanelhs@yahoo.gr
 */

/**
 * \defgroup    Drivers    Drivers
 *
 * \code #include <console.h> \endcode
 */

/******************************************************************************
	Code
******************************************************************************/

#ifndef CONSOLE_H_ONLY_ONE_INCLUDE_SAFETY
#define CONSOLE_H_ONLY_ONE_INCLUDE_SAFETY

/******************************************************************************
	Include files
******************************************************************************/

#include <stdint.h>

/******************************************************************************
	Type definitions
******************************************************************************/

/**
 * \brief The console.
 */
typedef struct console
{
	char *( *gets )( void ); /**< Pointer to \ref gets function. */
	void ( *puts )( const char *s ); /**< Pointer to \ref puts function. */
	void ( *putNum )( const int32_t num ); /**< Pointer to \ref putNum function. */
} console_t;

/******************************************************************************
	Function declarations
******************************************************************************/

console_t console_build( void );

#endif /* CONSOLE_H_ONLY_ONE_INCLUDE_SAFETY */
