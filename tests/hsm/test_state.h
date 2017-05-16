/******************************************************************************
	About
******************************************************************************/

/**
 * \file test_state.h
 *
 * \brief	Test cases for hsm states.
 *
 * Created:			18/11/2016
 *
 * \author	Elias Kanelis	hkanelhs@yahoo.gr
 */

/**
* \defgroup	Tests	Tests
*
* \code	#include <test_state.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef TEST_STATE_H_ONLY_ONE_INCLUDE_SAFETY
#define TEST_STATE_H_ONLY_ONE_INCLUDE_SAFETY

/******************************************************************************
	Include files
******************************************************************************/

#include "unit.h"

#include "hsm.h"

#include <stdint.h>		/* uint8_t */

/******************************************************************************
	Tests
******************************************************************************/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/*
 *	[FACT]	initString initializes any sString to "".
 */
START_SUBTEST( test_hsm_state_reset )
{
	/*
	 *	[CASE]	State should reset.
	 */
	{
		state_t kidState;
		state_t testState = 
		{
			.itsInitialState = &kidState
		};
		
		hsm_state_reset( &testState );
		
		//TODO: reset states
		
		CHECK( testState.itsInitialState == &kidState, "Initial state" );
		CHECK( testState.itsHistoryState == &kidState, "History state" );
	}
}
END_SUBTEST
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* TEST_STATE_H_ONLY_ONE_INCLUDE_SAFETY */
