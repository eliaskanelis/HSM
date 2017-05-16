/******************************************************************************
	About
******************************************************************************/

/**
 * \file test_state_transitions.h
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
* \code	#include <test_state_transitions.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef TEST_STATE_TRANSITIONS_H_ONLY_ONE_INCLUDE_SAFETY
#define TEST_STATE_TRANSITIONS_H_ONLY_ONE_INCLUDE_SAFETY

/******************************************************************************
	Include files
******************************************************************************/

#include "unit.h"

#include "hsm.h"

#include <stdint.h>		/* uint8_t */
#include <string.h>		/* strcmp */

/******************************************************************************
	Test state variables
******************************************************************************/

static void onEntry( state_t* me, hsm_event_t* event )
{
//#ifdef HSM_DEBUG
//	console_t serial = console_build();
//	serial.puts( me->itsName );
//	serial.puts( ":\tonEntry\n" );
//#endif
};

static void during( state_t* me, hsm_event_t* event )
{
//#ifdef HSM_DEBUG
//	console_t serial = console_build();
//	serial.puts( me->itsName );
//	serial.puts( ":\tduring\n" );
//#endif
};

static void onExit( state_t* me, hsm_event_t* event )
{
//#ifdef HSM_DEBUG
//	console_t serial = console_build();
//	serial.puts( me->itsName );
//	serial.puts( ":\tonExit\n" );
//#endif
};

static bool guard( state_t* me, hsm_event_t* event )
{
	if ( strcmp( event->data, "1" ) == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
};

static void action( state_t* me, hsm_event_t* event )
{
	console_t serial = console_build();
	serial.puts( me->itsName );
	serial.puts( ":\tAction\n" );
};

static state_t test_superState;
static state_t test_onState;
static state_t test_offState;
static state_t test_outState;

static state_t test_superState = 
{
	.itsName = "SUPER",
	.itsInitialState = &test_onState,
	.itsParentState = NULL,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 0,
	.itsTransition = NULL,
	
	.itsHistoryState = &test_onState,
};

static state_t test_onState = 
{
	.itsName = "ON   ",
	.itsInitialState = NULL,
	.itsParentState = &test_superState,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 1,
	.itsTransition = (transition_t[]){
		{ &guard, &action, &test_offState }
	},
	
	.itsHistoryState = NULL,
};

static state_t test_offState = 
{
	.itsName = "OFF  ",
	.itsInitialState = NULL,
	.itsParentState = &test_superState,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 1,
	.itsTransition = (transition_t[]){
		{ &guard, &action, &test_outState }
	},
	
	.itsHistoryState = NULL,
};

static state_t test_outState = 
{
	.itsName = "OUT  ",
	.itsInitialState = NULL,
	.itsParentState = NULL,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 1,
	.itsTransition = (transition_t[]){
		{ &guard, &action, &test_onState }
	},
	
	.itsHistoryState = NULL,
};

/******************************************************************************
	Tests
******************************************************************************/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/*
 *	[FACT]	HSM should transition handles events correctly.
 */
START_SUBTEST( test_hsm_state_onEntry )
{
	/*
	 *	[CASE]	superState.
	 */
	{
		state_t* test = &test_superState;
		
		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 2, "Superstate onEntry" );
		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 1, "On onEntry" );
//		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
//		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		
		hsm_state_reset( &test_superState );
		hsm_state_reset( &test_onState );
		hsm_state_reset( &test_offState );
		hsm_state_reset( &test_outState );
	}
	
	/*
	 *	[CASE]	onState.
	 */
	{
		state_t* test = &test_onState;
		
		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 1, "On onEntry" );
		//CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		//CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		
		hsm_state_reset( &test_superState );
		hsm_state_reset( &test_onState );
		hsm_state_reset( &test_offState );
		hsm_state_reset( &test_outState );
	}
	
	/*
	 *	[CASE]	offState.
	 */
	{
		state_t* test = &test_offState;
		
		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 1, "Off onEntry" );
		//CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		//CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		
		hsm_state_reset( &test_superState );
		hsm_state_reset( &test_onState );
		hsm_state_reset( &test_offState );
		hsm_state_reset( &test_outState );
	}
	
	/*
	 *	[CASE]	outState.
	 */
	{
		state_t* test = &test_outState;
		
		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 1, "Off outEntry" );
//		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
//		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		
		hsm_state_reset( &test_superState );
		hsm_state_reset( &test_onState );
		hsm_state_reset( &test_offState );
		hsm_state_reset( &test_outState );
	}
}
END_SUBTEST
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/*
 *	[FACT]	HSM should transition handles events correctly.
 */
START_SUBTEST( test_hsm_state_during )
{
	/*
	 *	[CASE]	superState.
	 */
	{
		state_t* test = &test_superState;
		
		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 2, "Superstate onEntry" );
		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 1, "On onEntry" );
//		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
//		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		
		test = &test_superState;
		
		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 2, "Superstate during" );
		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 1, "On during" );
//		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No during action" );
//		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No during action" );
		
		test = &test_superState;
		uint32_t id;
		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 2, "Superstate guard" );
		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 1, "On guard" );
//		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 3, "No guard action" );
//		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 3, "No guard action" );
		
		hsm_state_reset( &test_superState );
		hsm_state_reset( &test_onState );
		hsm_state_reset( &test_offState );
		hsm_state_reset( &test_outState );
	}
	
	/*
	 *	[CASE]	onState.
	 */
	{
		state_t* test = &test_onState;
		
		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 1, "On onEntry" );
		//CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		//CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		
		test = &test_onState;
		
		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 1, "On during" );
//		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No during action" );
//		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No during action" );
		
		test = &test_onState;
		uint32_t id = 0;
		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 1, "On guard" );
//		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 3, "No guard action" );
//		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 3, "No guard action" );
		
		hsm_state_reset( &test_superState );
		hsm_state_reset( &test_onState );
		hsm_state_reset( &test_offState );
		hsm_state_reset( &test_outState );
	}
	
	/*
	 *	[CASE]	offState.
	 */
	{
		state_t* test = &test_offState;
		
		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 1, "Off onEntry" );
		//CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		//CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		
		test = &test_offState;
		
		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 1, "Off during" );
//		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No during action" );
//		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No during action" );
		
		test = &test_offState;
		uint32_t id = 0;
		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 1, "Off guard" );
//		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 3, "No guard action" );
//		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 3, "No guard action" );
		
		hsm_state_reset( &test_superState );
		hsm_state_reset( &test_onState );
		hsm_state_reset( &test_offState );
		hsm_state_reset( &test_outState );
	}
	
	/*
	 *	[CASE]	outState.
	 */
	{
		state_t* test = &test_outState;
		
		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 1, "Out outEntry" );
//		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
//		CHECK( hsm_state_onEntry( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No entry action" );
		
		test = &test_outState;
		
		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 1, "Out during" );
//		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No during action" );
//		CHECK( hsm_state_during( &test, &(hsm_event_t){ 0, "0" } ) == 0, "No during action" );
		
		test = &test_outState;
		uint32_t id = 0;
		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 1, "Out guard" );
//		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 3, "No guard action" );
//		CHECK( hsm_state_guard( &test, &(hsm_event_t){ 0, "0" }, &id ) == 3, "No guard action" );
		
		hsm_state_reset( &test_superState );
		hsm_state_reset( &test_onState );
		hsm_state_reset( &test_offState );
		hsm_state_reset( &test_outState );
	}
}
END_SUBTEST
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/*
 *	[FACT]	HSM should transition handle events correctly.
 */
START_SUBTEST( test_hsm_state_transition )
{
	/*
	 *	[CASE]	HSM trasitions.
	 */
	{
		hsm_t testSys = hsm_build( &test_superState );
		
		CHECK( testSys.itsInitialState == &test_superState, "HSM Initial state" );
		CHECK( testSys.itsCurrentState == &test_superState, "HSM Current state" );
		CHECK( testSys.itsNextState == NULL, "HSM Next state" );
		CHECK( testSys.itsMode == HSM_ON_ENTRY, "HSM mode" );
		
//		/* SuperState onEntry */
//		hsm_handleEvent( &testSys, &(hsm_event_t){ 0, "0" } );
//		CHECK( testSys.itsCurrentState == &test_superState, "SuperState" );
//		CHECK( testSys.itsCurrentState->itsMode == HSM_DURING, "Next mode: During" );
//		
//		/* onState entry */
//		hsm_handleEvent( &testSys, &(hsm_event_t){ 0, "0" } );
//		CHECK( testSys.itsCurrentState == &test_onState, "onState" );
//		CHECK( testSys.itsCurrentState->itsMode == HSM_DURING, "Next mode: During" );
//		
//		/* superState during */
//		hsm_handleEvent( &testSys, &(hsm_event_t){ 0, "0" } );
//		CHECK( testSys.itsCurrentState == &test_superState, "SuperState" );
//		CHECK( testSys.itsCurrentState->itsMode == HSM_DURING, "During" );
//		
//		/* onState during */
//		hsm_handleEvent( &testSys, &(hsm_event_t){ 0, "0" } );
//		CHECK( testSys.itsCurrentState == &test_onState, "onState" );
//		CHECK( testSys.itsCurrentState->itsMode == HSM_DURING, "During" );
	}
}
END_SUBTEST
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif /* TEST_STATE_TRANSITIONS_H_ONLY_ONE_INCLUDE_SAFETY */
