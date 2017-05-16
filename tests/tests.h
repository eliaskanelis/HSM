/******************************************************************************
	About
******************************************************************************/

/**
 * \file tests.h
 *
 * \brief	Summary of all tests.
 *
 * Created:			18/11/2016
 *
 * \author	Elias Kanelis	hkanelhs@yahoo.gr
 */

/**
* \defgroup	Tests	Tests
*
* \code	#include <tests.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef TESTS_H_ONLY_ONE_INCLUDE_SAFETY
#define TESTS_H_ONLY_ONE_INCLUDE_SAFETY

/******************************************************************************
	Include files
******************************************************************************/

#include "hsm/unit_test_hsm_module.h"

/******************************************************************************
	Function declarations
******************************************************************************/


/**
 * \brief Runs all tests.
 * 
 * \return Will always return 0.
 */
static inline int run_unit_tests( void )
{
	RUN_TEST( test_hsm_module );
	
	/* Exit with success */
	return 0;
}

#endif /* TESTS_H_ONLY_ONE_INCLUDE_SAFETY */
