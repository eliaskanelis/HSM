/******************************************************************************
	Code
******************************************************************************/

/******************************************************************************
	Include files
******************************************************************************/

#include "hsm.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
	Function definitions
******************************************************************************/

static state_t topA;
static state_t subA1;
static state_t subA2;
static state_t topB;
static state_t subB1;

static state_t topA = {.itsInitialState  = &subA1,
                       .itsParentState   = NULL,
                       .onEntry          = NULL,
                       .during           = NULL,
                       .onExit           = NULL,
                       .itsTransition    = NULL,
                       .itsTransitionNum = 0,
                       .itsName          = " topA"};

static state_t subA1 = {.itsInitialState = NULL,
                        .itsParentState  = &topA,
                        .onEntry         = NULL,
                        .during          = NULL,
                        .onExit          = NULL,
                        .itsTransition =
                            (hsm_transition_t[]){{NULL, NULL, &subA2}},
                        .itsTransitionNum = 1,
                        .itsName          = "subA1"};

static state_t subA2 = {.itsInitialState = NULL,
                        .itsParentState  = &topA,
                        .onEntry         = NULL,
                        .during          = NULL,
                        .onExit          = NULL,
                        .itsTransition =
                            (hsm_transition_t[]){{NULL, NULL, &subB1}},
                        .itsTransitionNum = 1,
                        .itsName          = "subA2"};

static state_t topB = {.itsInitialState  = &subB1,
                       .itsParentState   = NULL,
                       .onEntry          = NULL,
                       .during           = NULL,
                       .onExit           = NULL,
                       .itsTransition    = NULL,
                       .itsTransitionNum = 0,
                       .itsName          = " topB"};

static state_t subB1 = {.itsInitialState = NULL,
                        .itsParentState  = &topB,
                        .onEntry         = NULL,
                        .during          = NULL,
                        .onExit          = NULL,
                        .itsTransition =
                            (hsm_transition_t[]){{NULL, NULL, &subA1}},
                        .itsTransitionNum = 1,
                        .itsName          = "subB1"};

static state_t* stateList[] = {&topA, &subA1, &subA2, &topB, &subB1};


/**
 * \brief This is the starting point of every C program...
 * \return Always returns 0. I do not care about OS error reporting.
 */
int main(void)
{
	hsm_t sys = hsm_build(&topA, stateList);

	/* Greeting */
	printf("Hierarchical state machine\nSystem ready...\n\n");

	int i = 0;

	while (1)
	{
		/* Handle event */
		printf("[%5d] ", ++i);
		hsm_handleEvent(&sys, NULL);

		char buffer[100];
		fgets(&buffer[0], 10, stdin);
	}

	return 0;
}
