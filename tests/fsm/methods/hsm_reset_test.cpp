#include "CppUTest/TestHarness.h"
#include "hsm.h"

#include <stdlib.h>

/* Initial state */
static state_t initialState = {.itsInitialState  = NULL,
                               .itsParentState   = NULL,
                               .onEntry          = NULL,
                               .during           = NULL,
                               .onExit           = NULL,
                               .itsTransition    = NULL,
                               .itsTransitionNum = 0};

/* Next state */
static state_t nextState = {.itsInitialState  = NULL,
                            .itsParentState   = NULL,
                            .onEntry          = NULL,
                            .during           = NULL,
                            .onExit           = NULL,
                            .itsTransition    = NULL,
                            .itsTransitionNum = 0};

static state_t* stateList[] = {&initialState, &nextState};

TEST_GROUP(hsm_reset)
{
	hsm_t    me; //Hierachical state machine under test
	uint32_t expectedStateNum;

	void setup()
	{
		/* Build hsm */
		me               = hsm_build(&initialState, stateList);
		expectedStateNum = sizeof(stateList) / sizeof(stateList[0]);
	}

	void teardown()
	{
		//
	}
};

TEST(hsm_reset, Should_ReturnToInitialState_When_NotInInitialState)
{
	/* Take hsm to an other than initial state */
	me.itsCurrentState = &nextState;

	/* Reset */
	const int err = hsm_reset(&me);

	/* Check */
	POINTERS_EQUAL(&initialState, me.itsInitialState);
	POINTERS_EQUAL(&initialState, me.itsCurrentState);
	POINTERS_EQUAL(stateList, me.allStates);
	LONGS_EQUAL(expectedStateNum, me.allStatesSize);
	CHECK_EQUAL(0, err);
}

TEST(hsm_reset, Should_GiveError_When_NULLGiven)
{
	/* Reset */
	const int err = hsm_reset(NULL);

	/* Check */
	CHECK_EQUAL(-1, err);
}

TEST(hsm_reset, Should_AlsoResetAllStates_When_HsmReset)
{
	/* Take hsm to an other than initial state */
	me.itsCurrentState = &nextState;

	/* Change States */
	initialState.itsMode = HSM_ST_M_DURING;
	nextState.itsMode    = HSM_ST_M_DURING;

	/* Reset */
	const int err = hsm_reset(&me);

	/* Check hsm */
	POINTERS_EQUAL(&initialState, me.itsInitialState);
	POINTERS_EQUAL(&initialState, me.itsCurrentState);
	POINTERS_EQUAL(stateList, me.allStates);
	LONGS_EQUAL(expectedStateNum, me.allStatesSize);
	CHECK_EQUAL(0, err);

	/* Check states through handle*/
	LONGS_EQUAL(HSM_ST_M_ON_ENTRY, me.allStates[0]->itsMode);
	LONGS_EQUAL(HSM_ST_M_ON_ENTRY, me.allStates[1]->itsMode);

	/* Check real states */
	LONGS_EQUAL(HSM_ST_M_ON_ENTRY, initialState.itsMode);
	LONGS_EQUAL(HSM_ST_M_ON_ENTRY, nextState.itsMode);
}
