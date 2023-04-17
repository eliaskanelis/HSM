/******************************************************************************
	Code
******************************************************************************/

/******************************************************************************
	Include files
******************************************************************************/

#include "console.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/******************************************************************************
	Definitions
******************************************************************************/

static char g_str[ 1000 ];

/******************************************************************************
	Local function declarations
******************************************************************************/
/**
 * \brief Waits until it reads a string from the console.
 *
 * \return The string.
 */
static char *getString( void )
{
	if( scanf( "%s", g_str ) != 1 )
	{
		/* It failed make it empty*/
		strcpy( g_str, "" );//TODO: check if robust
	}

	return g_str;
}

/**
 * \brief Writes a string to the console
 *
 * \param[in] s The string
 */
static void putString( const char *s )
{
	printf( "%s", s );
}

/**
 * \brief Writes an integer number to the console.
 *
 * \param[in] num The number.
 */
static void putNum( const int32_t num )
{
	printf( "%i", num );
}

/**
 * \brief Initializes the console.
 *
 * \param[in] obj The console object.
 */
static void console_init( console_t *obj )
{
	obj->gets = getString;
	obj->puts = putString;
	obj->putNum = putNum;
}

/******************************************************************************
	Function definitions
******************************************************************************/

/**
 * \brief Builds the console and provides a handler.
 *
 * \return The console object.
 */
console_t console_build( void )
{
	console_t aux;
	console_init( &aux );
	return aux;
}
