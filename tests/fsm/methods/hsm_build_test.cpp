#include "CppUTest/TestHarness.h"

#include "hsm.h"

#include <stdlib.h>	/* NULL */

extern state_t parentState;
extern state_t childState;

state_t parentState = 
{
	.itsInitialState = NULL,
	.itsParentState = NULL,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = NULL,
	.itsTransitionNum = 0
};

/* Initial state */
state_t childState = 
{
	.itsInitialState = NULL,
	.itsParentState = &parentState,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = NULL,
	.itsTransitionNum = 0
};

static state_t* stateList[] = 
{
	&parentState,
	&childState 
};

TEST_GROUP( hsm_build )
{
	hsm_t me; //Hierachical state machine under test
	uint32_t expectedStateNum;

	void setup()
	{
		expectedStateNum = sizeof(stateList)/sizeof(stateList[0]);
	}

	void teardown()
	{
		//
	}
	
};

TEST( hsm_build, Should_InitializeValidHsmHandle_When_ValidInitialStateGiven )
{
	/* Build hsm */
	me = hsm_build( &parentState, stateList );

	/* Check  correct build */
	POINTERS_EQUAL( &parentState, me.itsInitialState );
	POINTERS_EQUAL( &parentState, me.itsCurrentState );
	POINTERS_EQUAL( stateList, me.allStates );
	LONGS_EQUAL( expectedStateNum, me.allStatesSize );
}

TEST( hsm_build, Should_InitializeInvalidHsmHandle_When_NoTopStateGiven )
{
	/* Build hsm */
	me = hsm_build( &childState, stateList );

	/* Check  correct build */
	POINTERS_EQUAL( NULL, me.itsInitialState );
	POINTERS_EQUAL( NULL, me.itsCurrentState );
	POINTERS_EQUAL( NULL, me.allStates );
	LONGS_EQUAL( 0, me.allStatesSize );
}

TEST( hsm_build, Should_InitializeInvalidHsmHandle_When_NullInitialStateGiven )
{
	/* Build hsm */
	me = hsm_build( NULL, stateList );

	/* Check  correct build */
	POINTERS_EQUAL( NULL, me.itsInitialState );
	POINTERS_EQUAL( NULL, me.itsCurrentState );
	POINTERS_EQUAL( NULL, me.allStates );
	LONGS_EQUAL( 0, me.allStatesSize );
}

TEST( hsm_build, Should_InitializeInvalidHsmHandle_When_NoStateListGiven )
{
	/* Build hsm */
	me = _hsm_build( &parentState, NULL, 2U );

	/* Check  correct build */
	POINTERS_EQUAL( NULL, me.itsInitialState );
	POINTERS_EQUAL( NULL, me.itsCurrentState );
	POINTERS_EQUAL( NULL, me.allStates );
	LONGS_EQUAL( 0, me.allStatesSize );
}

TEST( hsm_build, Should_InitializeInvalidHsmHandle_When_NoNumberOfStateListGiven )
{
	/* Build hsm */
	me = _hsm_build( &parentState, stateList, 0U );

	/* Check  correct build */
	POINTERS_EQUAL( NULL, me.itsInitialState );
	POINTERS_EQUAL( NULL, me.itsCurrentState );
	POINTERS_EQUAL( NULL, me.allStates );
	LONGS_EQUAL( 0, me.allStatesSize );
}

TEST( hsm_build, Should_InitializeInvalidHsmHandle_When_NoStateListAndNoNumberOfStateListGiven )
{
	/* Build hsm */
	me = _hsm_build( &parentState, NULL, 0U );

	/* Check  correct build */
	POINTERS_EQUAL( NULL, me.itsInitialState );
	POINTERS_EQUAL( NULL, me.itsCurrentState );
	POINTERS_EQUAL( NULL, me.allStates );
	LONGS_EQUAL( 0, me.allStatesSize );
}

TEST( hsm_build, Should_InitializeInvalidHsmHandle_When_NothingGivenGiven )
{
	/* Build hsm */
	me = _hsm_build( NULL, NULL, 0U );

	/* Check  correct build */
	POINTERS_EQUAL( NULL, me.itsInitialState );
	POINTERS_EQUAL( NULL, me.itsCurrentState );
	POINTERS_EQUAL( NULL, me.allStates );
	LONGS_EQUAL( 0, me.allStatesSize );
}
