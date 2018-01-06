#include "CppUTest/TestHarness.h"

#include "hsm.h"

#include <stdlib.h>	/* NULL */

/*
digraph G {
	graph [fontsize=10 fontname="Verdana" compound=true rankdir=LR];
	node [shape=record fontsize=10 fontname="Verdana"];

	// Edges between nodes render fine
	"on" -> "off";
	"off" -> "on";
}
*/

/*
on.on_entry
on.during
on.checking_guard
on.on_exit

off.on_entry
off.during
off.checking_guard
off.on_exit

...
*/

extern state_t onState;
extern state_t offState;

/* Initial state */
state_t onState = 
{
	.itsInitialState = NULL,
	.itsParentState = NULL,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = (hsm_transition_t[])
		{
			{ NULL, NULL, &offState }
		},
	.itsTransitionNum = 1
};

/* Next state */
state_t offState = 
{
	.itsInitialState = NULL,
	.itsParentState = NULL,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = (hsm_transition_t[])
		{
			{ NULL, NULL, &onState }
		},
	.itsTransitionNum = 1
};

static state_t* stateList[] = 
{
	&onState,
	&offState
};

TEST_GROUP( hsm_1_layer_simple )
{
	hsm_t me; //Hierachical state machine under test
	hsm_event_t event;

	void setup()
	{
		/* Build hsm */
		me = hsm_build( &onState, stateList );

		/* Check state before event handle */
		POINTERS_EQUAL( &onState, me.itsInitialState );
		POINTERS_EQUAL( &onState, me.itsCurrentState );
		LONGS_EQUAL( HSM_ST_M_ON_ENTRY, me.itsCurrentState->itsMode );
	}

	void teardown()
	{
		//
	}
	
};

static void CHECK_HSM_NO_EVENT( hsm_t* me, state_t* fromState, hsm_st_mode_t fromMode, state_t* toState, hsm_st_mode_t toMode )
{
	/* From */
	POINTERS_EQUAL( fromState, me->itsCurrentState );
	LONGS_EQUAL( fromMode, me->itsCurrentState->itsMode );

	const int err = hsm_handleEvent( me, NULL );
	CHECK_EQUAL( 1, err );
	
	/* To */
	POINTERS_EQUAL( toState, me->itsCurrentState );
	LONGS_EQUAL( toMode, me->itsCurrentState->itsMode );
}

TEST( hsm_1_layer_simple, Should_HandleSimpleOnOffHSMWithNoEvents )
{
	/* ON.on_entry -- > ON.during */
	CHECK_HSM_NO_EVENT( &me, &onState, HSM_ST_M_ON_ENTRY, &onState, HSM_ST_M_DURING );

	/* ON.during -- > ON.checking_quard */
	CHECK_HSM_NO_EVENT( &me, &onState ,HSM_ST_M_DURING, &onState, HSM_ST_M_CHECKING_GUARD );

	/* ON.checking_quard -- > ON.on_exit */
	CHECK_HSM_NO_EVENT( &me, &onState, HSM_ST_M_CHECKING_GUARD, &onState, HSM_ST_M_ON_EXIT );

	/* ON.on_exit -- > OFF.on_entry */
	CHECK_HSM_NO_EVENT( &me, &onState, HSM_ST_M_ON_EXIT, &offState, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, onState.itsMode );

	/* OFF.on_entry -- > OFF.during */
	CHECK_HSM_NO_EVENT( &me, &offState, HSM_ST_M_ON_ENTRY, &offState, HSM_ST_M_DURING );

	/* OFF.during -- > OFF.checking_quard */
	CHECK_HSM_NO_EVENT( &me, &offState ,HSM_ST_M_DURING, &offState, HSM_ST_M_CHECKING_GUARD );

	/* OFF.checking_quard -- > OFF.on_exit */
	CHECK_HSM_NO_EVENT( &me, &offState, HSM_ST_M_CHECKING_GUARD, &offState, HSM_ST_M_ON_EXIT );

	/* OFF.on_exit -- > ON.on_entry */
	CHECK_HSM_NO_EVENT( &me, &offState, HSM_ST_M_ON_EXIT, &onState, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, offState.itsMode );
}

TEST( hsm_1_layer_simple, Should_HandleSimpleOnOffHSMWithNoEvents_old )
{

	/* ON.on_entry -- > ON.during */
	{
		const int err = hsm_handleEvent( &me, NULL );
		POINTERS_EQUAL( &onState, me.itsCurrentState );
		LONGS_EQUAL( HSM_ST_M_DURING, me.itsCurrentState->itsMode );
		CHECK_EQUAL( 1, err );
	}

	/* ON.during -- > ON.checking_quard */
	{
		const int err = hsm_handleEvent( &me, NULL );
		POINTERS_EQUAL( &onState, me.itsCurrentState );
		LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, me.itsCurrentState->itsMode );
		CHECK_EQUAL( 1, err );
	}

	/* ON.checking_quard -- > ON.on_exit */
	{
		const int err = hsm_handleEvent( &me, NULL );
		POINTERS_EQUAL( &onState, me.itsCurrentState );
		LONGS_EQUAL( HSM_ST_M_ON_EXIT, me.itsCurrentState->itsMode );
		CHECK_EQUAL( 1, err );
	}

	/* ON.on_exit -- > OFF.on_entry */
	{
		const int err = hsm_handleEvent( &me, NULL );
		LONGS_EQUAL( HSM_ST_M_ON_ENTRY, onState.itsMode );
		POINTERS_EQUAL( &offState, me.itsCurrentState );
		LONGS_EQUAL( HSM_ST_M_ON_ENTRY, me.itsCurrentState->itsMode );
		CHECK_EQUAL( 1, err );
	}

	/* OFF.on_entry -- > OFF.during */
	{
		const int err = hsm_handleEvent( &me, NULL );
		POINTERS_EQUAL( &offState, me.itsCurrentState );
		LONGS_EQUAL( HSM_ST_M_DURING, me.itsCurrentState->itsMode );
		CHECK_EQUAL( 1, err );
	}

	/* OFF.during -- > OFF.checking_guards */
	{
		const int err = hsm_handleEvent( &me, NULL );
		POINTERS_EQUAL( &offState, me.itsCurrentState );
		LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, me.itsCurrentState->itsMode );
		CHECK_EQUAL( 1, err );
	}

	/* OFF.checking_quard -- > OFF.on_exit */
	{
		const int err = hsm_handleEvent( &me, NULL );
		POINTERS_EQUAL( &offState, me.itsCurrentState );
		LONGS_EQUAL( HSM_ST_M_ON_EXIT, me.itsCurrentState->itsMode );
		CHECK_EQUAL( 1, err );
	}

	/* OFF.on_exit -- > ON.on_entry */
	{
		const int err = hsm_handleEvent( &me, NULL );
		LONGS_EQUAL( HSM_ST_M_ON_ENTRY, offState.itsMode );
		POINTERS_EQUAL( &onState, me.itsCurrentState );
		LONGS_EQUAL( HSM_ST_M_ON_ENTRY, me.itsCurrentState->itsMode );
		CHECK_EQUAL( 1, err );
	}
}