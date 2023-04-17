#include "CppUTest/TestHarness.h"

#include "hsm.h"

#include <stdlib.h>

extern state_t firstState;
extern state_t secondState;

/* Initial state */
state_t firstState =
{
	.itsInitialState = NULL,
	.itsParentState = NULL,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = (hsm_transition_t[])
		{
			{ NULL, NULL, &secondState }
		},
	.itsTransitionNum = 1
};

/* Next state */
state_t secondState =
{
	.itsInitialState = NULL,
	.itsParentState = NULL,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = (hsm_transition_t[])
		{
			{ NULL, NULL, &firstState }
		},
	.itsTransitionNum = 1
};

static state_t* stateList[] =
{
	&firstState,
	&secondState
};

TEST_GROUP( hsm_handleEvent )
{
	hsm_t me; //Hierachical state machine under test
	hsm_event_t event;

	void setup()
	{
		/* Build hsm */
		me = hsm_build( &firstState, stateList );
	}

	void teardown()
	{
		//
	}
};

TEST( hsm_handleEvent, Should_GiveError_When_InvalidHandleAndNoEvent )
{
	/* Handle event */
	const int err = hsm_handleEvent( NULL, &event );

	/* Check */
	CHECK_EQUAL( -1, err );
}

TEST( hsm_handleEvent, Should_Know_When_NoEventProvided )
{
	/* Handle event */
	const int err = hsm_handleEvent( &me, NULL );

	/* Check */
	CHECK_EQUAL( 1, err );
}
