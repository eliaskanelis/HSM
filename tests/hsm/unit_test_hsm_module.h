/******************************************************************************
	About
******************************************************************************/

/**
 * \file unit_test_hsm_module.h
 *
 * \brief	Test cases for HSM module.
 *
 * Created:			18/11/2016
 *
 * \author	Elias Kanelis	hkanelhs@yahoo.gr
 */

/**
* \defgroup	Tests	Tests
*
* \code	#include <unit_test_hsm_module.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef UNIT_TEST_HSM_MODULE_H_ONLY_ONE_INCLUDE_SAFETY
#define UNIT_TEST_HSM_MODULE_H_ONLY_ONE_INCLUDE_SAFETY

/******************************************************************************
	Include files
******************************************************************************/

#include "test_state.h"
#include "test_state_transitions.h"

#include "unit.h"

/******************************************************************************
	Function declarations
******************************************************************************/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/*
 *	Tests
 */
START_TEST( test_hsm_module )
{
	/* Test state */
	ATTACH_SUBTEST( test_hsm_state_reset );
	
	/* Test state transitions */
	ATTACH_SUBTEST( test_hsm_state_onEntry );
	ATTACH_SUBTEST( test_hsm_state_during );
	ATTACH_SUBTEST( test_hsm_state_transition );
}
END_TEST
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* UNIT_TEST_HSM_MODULE_H_ONLY_ONE_INCLUDE_SAFETY */
