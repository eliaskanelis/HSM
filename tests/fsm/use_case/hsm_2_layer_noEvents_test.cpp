#include "CppUTest/TestHarness.h"

#include "hsm.h"

#include <stdlib.h>	/* NULL */

/*
digraph G {
	graph [fontsize=10 fontname="Verdana" compound=true rankdir=LR];
	node [shape=record fontsize=10 fontname="Verdana"];

	subgraph cluster_0 {
		label = "topA";
		"cluster0_dummy" [ label = "", style = invis ];
		"cluster0_dummy" -> "subA1"
		"subA1" -> "subA2"
	}

	subgraph cluster_1 {
		label = "topB";
		"cluster1_dummy" [ label = "", style = invis ];
		"cluster1_dummy" -> "subB1"
	}

	// Edges between nodes render fine
	"subA2" -> "subB1";
	"subB1" -> "subA1";
}

Sequence:

topA.on_entry
	subA1.on_entry
topA.during
	subA1.during
topA.checking_guard
	subA1.checking_guard
	subA1.on_exit
	subA2.on_entry
topA.during
	subA2.during
topA.checking_guard
	subA2.checking_guard
	subA2.on_exit
topA.on_exit
topB.on_entry
	subB1.on_entry
topB.during
	subB1.during
topB.checking_guard
	subB1.checking_guard
	subB1.on_exit
topB.on_exit
...
topA.on_entry
	subA1.on_entry
	...
*/

extern state_t topA;
extern state_t subA1;
extern state_t subA2;
extern state_t topB;
extern state_t subB1;

state_t topA = 
{
	.itsInitialState = &subA1,
	.itsParentState = NULL,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = NULL,
	.itsTransitionNum = 0
};

state_t subA1 = 
{
	.itsInitialState = NULL,
	.itsParentState = &topA,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = (hsm_transition_t[])
		{
			{ NULL, NULL, &subA2 }
		},
	.itsTransitionNum = 1
};

state_t subA2 = 
{
	.itsInitialState = NULL,
	.itsParentState = &topA,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = (hsm_transition_t[])
		{
			{ NULL, NULL, &subB1 }
		},
	.itsTransitionNum = 0
};

state_t topB = 
{
	.itsInitialState = &subB1,
	.itsParentState = NULL,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = NULL,
	.itsTransitionNum = 0
};

state_t subB1 = 
{
	.itsInitialState = NULL,
	.itsParentState = &topB,
	.onEntry = NULL,
	.during = NULL,
	.onExit = NULL,
	.itsTransition = (hsm_transition_t[])
		{
			{ NULL, NULL, &subA1 }
		},
	.itsTransitionNum = 1
};

static state_t* stateList[] = 
{
	&topA,
	&subA1,
	&subA2,
	&topB,
	&subB1
};

TEST_GROUP( hsm_2_layer_noEvents )
{
	hsm_t me; //Hierachical state machine under test
	hsm_event_t event;

	void setup()
	{
		/* Build hsm */
		me = hsm_build( &topA, stateList );

		/* Check state before event handle */
		POINTERS_EQUAL( &topA, me.itsInitialState );
		POINTERS_EQUAL( &topA, me.itsCurrentState );
		LONGS_EQUAL( HSM_ST_M_ON_ENTRY, me.itsCurrentState->itsMode );
	}

	void teardown()
	{
		//
	}
	
};

static void CHECK_HSM( hsm_t* me, state_t* fromState, hsm_st_mode_t fromMode, state_t* toState, hsm_st_mode_t toMode )
{
	/* From */
	POINTERS_EQUAL( fromState, me->itsCurrentState );
	LONGS_EQUAL( fromMode, me->itsCurrentState->itsMode );

	/* Handle Event */
	const int err = hsm_handleEvent( me, NULL );
	CHECK_EQUAL( 1, err );
	
	/* To */
	POINTERS_EQUAL( toState, me->itsCurrentState );
	LONGS_EQUAL( toMode, me->itsCurrentState->itsMode );
}

TEST( hsm_2_layer_noEvents, Should_HandleTransitions )
{
	/* 1) topA.on_entry --> subA1.on_entry */
	CHECK_HSM( &me, &topA, HSM_ST_M_ON_ENTRY, &subA1, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_DURING, topA.itsMode );
	POINTERS_EQUAL( &subA1, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 2) subA1.on_entry --> topA.during */
	CHECK_HSM( &me, &subA1, HSM_ST_M_ON_ENTRY, &topA, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_DURING, subA1.itsMode );
	POINTERS_EQUAL( &subA1, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 3) topA.during --> subA1.during */
	CHECK_HSM( &me, &topA, HSM_ST_M_DURING, &subA1, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, topA.itsMode );
	POINTERS_EQUAL( &subA1, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 4) subA1.during --> topA.checking_guard */
	CHECK_HSM( &me, &subA1, HSM_ST_M_DURING, &topA, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, subA1.itsMode );
	POINTERS_EQUAL( &subA1, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 5) topA.checking_guard --> subA1.checking_guard */
	CHECK_HSM( &me, &topA, HSM_ST_M_CHECKING_GUARD, &subA1, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_DURING, topA.itsMode );
	POINTERS_EQUAL( &subA1, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 6) subA1.checking_guard --> subA1.on_exit */
	CHECK_HSM( &me, &subA1, HSM_ST_M_CHECKING_GUARD, &subA1, HSM_ST_M_ON_EXIT );
	LONGS_EQUAL( HSM_ST_M_ON_EXIT, subA1.itsMode );
	POINTERS_EQUAL( &subA1, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 7) subA1.on_exit --> subA2.on_entry */
	CHECK_HSM( &me, &subA1, HSM_ST_M_ON_EXIT, &subA2, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subA1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 8) subA2.on_entry --> topA.during */
	CHECK_HSM( &me, &subA2, HSM_ST_M_ON_ENTRY, &topA, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_DURING, subA2.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 9) topA.during --> subA2.during */
	CHECK_HSM( &me, &topA, HSM_ST_M_DURING, &subA2, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, topA.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 10) subA2.during --> topA.checking_guard*/
	CHECK_HSM( &me, &subA2, HSM_ST_M_DURING, &topA, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, subA2.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 11) topA.checking_guard --> subA2.checking_guard*/
	CHECK_HSM( &me, &topA, HSM_ST_M_CHECKING_GUARD, &subA2, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_DURING, topA.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 12) subA2.checking_guard --> subA2.on_exit */
	CHECK_HSM( &me, &subA2, HSM_ST_M_CHECKING_GUARD, &subA2, HSM_ST_M_ON_EXIT );
	LONGS_EQUAL( HSM_ST_M_ON_EXIT, subA2.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 13) subA2.on_exit --> topA.on_exit */
	CHECK_HSM( &me, &subA2, HSM_ST_M_ON_EXIT, &topA, HSM_ST_M_ON_EXIT );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subA2.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 14) topA.on_exit --> topB.on_entry */
	CHECK_HSM( &me, &topA, HSM_ST_M_ON_EXIT, &topB, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, topA.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 15) topB.on_entry --> subB1.on_entry */
	CHECK_HSM( &me, &topB, HSM_ST_M_ON_ENTRY, &subB1, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_DURING, topB.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 16) subB1.on_entry --> topB.during */
	CHECK_HSM( &me, &subB1, HSM_ST_M_ON_ENTRY, &topB, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_DURING, subB1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 17) topB.during --> subB1.during */
	CHECK_HSM( &me, &topB, HSM_ST_M_DURING, &subB1, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, topB.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 18) subB1.during --> topB.checking_guard */
	CHECK_HSM( &me, &subB1, HSM_ST_M_DURING, &topB, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, subB1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 19) topB.checking_guard --> subB1.checking_guard */
	CHECK_HSM( &me, &topB, HSM_ST_M_CHECKING_GUARD, &subB1, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_DURING, topB.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 20) subB1.checking_guard --> subB1.on_exit */
	CHECK_HSM( &me, &subB1, HSM_ST_M_CHECKING_GUARD, &subB1, HSM_ST_M_ON_EXIT );
	LONGS_EQUAL( HSM_ST_M_ON_EXIT, subB1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 21) subB1.on_exit --> topB.on_exit */
	CHECK_HSM( &me, &subB1, HSM_ST_M_ON_EXIT, &topB, HSM_ST_M_ON_EXIT );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subB1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* 22) topB.on_exit --> topA.on_entry */
	CHECK_HSM( &me, &topB, HSM_ST_M_ON_EXIT, &topA, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, topB.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/*
	 * Now the hsm repeats itself over and over again
	 *
	 *
	 */

	/* 23) topA.on_entry --> subA1.on_entry */
	CHECK_HSM( &me, &topA, HSM_ST_M_ON_ENTRY, &subA1, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_DURING, topA.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	/* 24) subA1.on_entry --> topA.during */
	CHECK_HSM( &me, &subA1, HSM_ST_M_ON_ENTRY, &topA, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_DURING, subA1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	
	LONGS_EQUAL( HSM_ST_M_DURING, topA.itsMode );
	LONGS_EQUAL( HSM_ST_M_DURING, subA1.itsMode );//was HSM_ST_M_ON_ENTRY
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subA2.itsMode );//was HSM_ST_M_DURING
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, topB.itsMode );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subB1.itsMode );

	/* 25) topA.during --> subA1.during */
	CHECK_HSM( &me, &topA, HSM_ST_M_DURING, &subA1, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, topA.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	/* 26) subA1.during --> topA.checking_guard */
	CHECK_HSM( &me, &subA1, HSM_ST_M_DURING, &topA, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, subA1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, topA.itsMode );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, subA1.itsMode );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subA2.itsMode );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, topB.itsMode );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subB1.itsMode );

	return;

	CHECK( hsm_handleEvent( &me, NULL ) == 1 );
	
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, topA.itsMode );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, subA1.itsMode );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subA2.itsMode );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, topB.itsMode );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subB1.itsMode );

	/* Tests */	
/*	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, topA.itsMode );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subA1.itsMode );
	LONGS_EQUAL( HSM_ST_M_DURING, subA2.itsMode );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, topB.itsMode );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subB1.itsMode );*/

	/* 27) topA.checking_guard --> subA1.checking_guard */
	CHECK_HSM( &me, &topA, HSM_ST_M_CHECKING_GUARD, &subA1, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_DURING, topA.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	return;

	/* 28) subA1.checking_guard --> subA1.on_exit */
	CHECK_HSM( &me, &subA1, HSM_ST_M_CHECKING_GUARD, &subA1, HSM_ST_M_ON_EXIT );
	LONGS_EQUAL( HSM_ST_M_ON_EXIT, subA1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	/* subA1.on_exit --> subA2.on_entry */
	CHECK_HSM( &me, &subA1, HSM_ST_M_ON_EXIT, &subA2, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subA1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	/* subA2.on_entry --> topA.during */
	CHECK_HSM( &me, &subA2, HSM_ST_M_ON_ENTRY, &topA, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_DURING, subA2.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	/* topA.during --> subA2.during */
	CHECK_HSM( &me, &topA, HSM_ST_M_DURING, &subA2, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, topA.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	/* subA2.during --> topA.checking_guard*/
	CHECK_HSM( &me, &subA2, HSM_ST_M_DURING, &topA, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, subA2.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	/* topA.checking_guard --> subA2.checking_guard*/
	CHECK_HSM( &me, &topA, HSM_ST_M_CHECKING_GUARD, &subA2, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_DURING, topA.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	/* subA2.checking_guard --> subA2.on_exit */
	CHECK_HSM( &me, &subA2, HSM_ST_M_CHECKING_GUARD, &subA2, HSM_ST_M_ON_EXIT );
	LONGS_EQUAL( HSM_ST_M_ON_EXIT, subA2.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	/* subA2.on_exit --> topA.on_exit */
	CHECK_HSM( &me, &subA2, HSM_ST_M_ON_EXIT, &topA, HSM_ST_M_ON_EXIT );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subA2.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );

	/* topA.on_exit --> topB.on_entry */
	CHECK_HSM( &me, &topA, HSM_ST_M_ON_EXIT, &topB, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, topA.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* topB.on_entry --> subB1.on_entry */
	CHECK_HSM( &me, &topB, HSM_ST_M_ON_ENTRY, &subB1, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_DURING, topB.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* subB1.on_entry --> topB.during */
	CHECK_HSM( &me, &subB1, HSM_ST_M_ON_ENTRY, &topB, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_DURING, subB1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* topB.during --> subB1.during */
	CHECK_HSM( &me, &topB, HSM_ST_M_DURING, &subB1, HSM_ST_M_DURING );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, topB.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* subB1.during --> topB.checking_guard */
	CHECK_HSM( &me, &subB1, HSM_ST_M_DURING, &topB, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_CHECKING_GUARD, subB1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* topB.checking_guard --> subB1.checking_guard */
	CHECK_HSM( &me, &topB, HSM_ST_M_CHECKING_GUARD, &subB1, HSM_ST_M_CHECKING_GUARD );
	LONGS_EQUAL( HSM_ST_M_DURING, topB.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* subB1.checking_guard --> subB1.on_exit */
	CHECK_HSM( &me, &subB1, HSM_ST_M_CHECKING_GUARD, &subB1, HSM_ST_M_ON_EXIT );
	LONGS_EQUAL( HSM_ST_M_ON_EXIT, subB1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* subB1.on_exit --> topB.on_exit */
	CHECK_HSM( &me, &subB1, HSM_ST_M_ON_EXIT, &topB, HSM_ST_M_ON_EXIT );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, subB1.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );

	/* topB.on_exit --> topA.on_entry */
	CHECK_HSM( &me, &topB, HSM_ST_M_ON_EXIT, &topA, HSM_ST_M_ON_ENTRY );
	LONGS_EQUAL( HSM_ST_M_ON_ENTRY, topB.itsMode );
	POINTERS_EQUAL( &subA2, topA.itsHistoryState );
	POINTERS_EQUAL( &subB1, topB.itsHistoryState );
}