// ############################################################################
// ############################################################################
// Code

// ############################################################################
// ############################################################################
// Include files

#include "hsm.h"

#include <stdlib.h>
#include <stdbool.h>

// ############################################################################
// ############################################################################
// Local definitions

#ifdef DEBUG
#include <stdio.h>

static const char* hsm_state_modeName[] =
{
	"onEntry",
	"during ",
	"guard  ",
	"action ",
	"onExit ",
	"ERROR  "
};
#endif

static void hsm_debug( const state_t *fromState, const state_t *toState, const char *msg )
{
#ifdef DEBUG
	const char* fromStateModeName = hsm_state_modeName[fromState->itsMode];
	const char* toStateModeName = hsm_state_modeName[toState->itsMode];

	printf( "\x1b[32m%s\x1b[0m: ", fromStateModeName );
	printf( "\x1b[33m%s\x1b[0m.\x1b[33m%s\x1b[0m", fromState->itsName, fromStateModeName );
	printf( " ---> \x1b[33m%s.\x1b[33m%s\x1b[0m\x1b[0m", toState->itsName, toStateModeName );
	printf( "   \t[\x1b[34m%-40s\x1b[0m]\t", msg);
#else
	( void )msg;
	( void )fromState;
	( void )toState;
#endif
}

// ############################################################################
// ############################################################################
// Local functions

static bool hsm_init( hsm_t *me, const state_t *initialState,
                      state_t *allStates[], const uint32_t allStatesSize );

/*
 * State related
 */
static void state_reset( state_t *state );
static int state_hasChild( const state_t *state );
static int state_hasParent( const state_t *state );

static state_t *state_getTop( const state_t *state );

static int state_exec_onEntry( const state_t *state,
                               const hsm_event_t *event );
static int state_exec_during( const state_t *state, const hsm_event_t *event );
static int state_exec_onExit( const state_t *state, const hsm_event_t *event );

/**
 * \brief Initializes the hierarchical state machine.
 *
 * \param[in,out] me            The hierarchical state machine handle.
 * \param[in]     initialState  The state that the HSM will begin with.
 * \param[in]     allStates     A list with all the hsm's states.
 * \param[in]     allStatesSize The number of the hsm's states.
 *
 * \return True on success, False on failure.
 */
static bool hsm_init( hsm_t *me, const state_t *initialState,
                      state_t *allStates[], const uint32_t allStatesSize )
{
	/* Check valid input */
	bool success = ((me != NULL) && (initialState != NULL) && (allStates!=NULL) && (allStatesSize != 0U));

	/* Check if initial state */
	if( success )
	{
		if( initialState->itsParentState != NULL )
		{
			/* Not a top state */
			success = false;
		}
	}

	if( success )
	{
		/* Initialize it */
		me->itsInitialState = ( state_t * )initialState;
		me->itsCurrentState = ( state_t * )initialState;
		me->allStates = allStates;
		me->allStatesSize = allStatesSize;

		/* Reset all states */
		for( uint32_t i = me->allStatesSize ; i -- > 0 ; )
		{
			state_reset( me->allStates[i] );
		}
	}
	else
	{
		if (me != NULL)
		{
			/* Not a top state */
			me->itsInitialState = NULL;
			me->itsCurrentState = NULL;
			me->allStates = NULL;
			me->allStatesSize = 0;
			success = false;
		}
	}

	return success;
}

/**
 * \brief Resets state.
 *
 * \param[in,out] state The hsm state.
 */
static void state_reset( state_t *state )
{
	/* Reset state */
	state->itsMode = HSM_ST_M_ON_ENTRY;
	state->itsHistoryState = ( state_t * )state->itsInitialState;
}

/**
 * \brief Checks is state has children.
 *
 * \param[in] state The hsm state.
 *
 * \retval 0 No children.
 * \retval 1 Has children.
 */
static int state_hasChild( const state_t *state )
{
	if( state->itsHistoryState == NULL )
	{
		/* No children */
		return 0;
	}

	/* Has children */
	return 1;
}

/**
 * \brief Checks is state has parent.
 *
 * \param[in] state The hsm state.
 *
 * \retval 0 No parent.
 * \retval 1 Has parent.
 */
static int state_hasParent( const state_t *state )
{
	if( state->itsParentState == NULL )
	{
		/* No parent */
		return 0;
	}

	/* Has parent */
	return 1;
}

/**
 * \brief Gets the state's top state
 *
 * \param[in] state The hsm state.
 *
 * \return The top state.
 */
static state_t *state_getTop( const state_t *state )
{
	/* Init */
	state_t *topState = ( state_t * )state; //We assume this is the top state.

	/* Find top parent */
	do
	{
		/* Check for parent */
		if( state_hasParent( state ) )
		{
			/* It has a parent, so the parent is the top state */
			topState = ( state_t * )topState->itsParentState;
		}
	}
	while( topState == NULL );

	/* Exit with parent */
	return topState;
}

/**
 * \brief Executes states on entry action.
 *
 * \param[in] state The hsm state.
 * \param[in] event The event signal.
 *
 * \retval 0   Success.
 * \retval Any Fail.
 */
static int state_exec_onEntry( const state_t *state, const hsm_event_t *event )
{
	/* Execute */
	int err = 0;

	if( state->onEntry != NULL )
	{
		err = state->onEntry( state, event );
	}

	/* Exit */
	return err;
}

/**
 * \brief Executes states during action.
 *
 * \param[in] state The hsm state.
 * \param[in] event The event signal.
 *
 * \retval 0   Success.
 * \retval Any Fail.
 */
static int state_exec_during( const state_t *state, const hsm_event_t *event )
{
	/* Execute */
	int err = 0;

	if( state->during != NULL )
	{
		err = state->during( state, event );
	}

	/* Exit */
	return err;
}

/**
 * \brief Executes states on exit action.
 *
 * \param[in] state The hsm state.
 * \param[in] event The event signal.
 *
 * \retval 0   Success.
 * \retval Any Fail.
 */
static int state_exec_onExit( const state_t *state, const hsm_event_t *event )
{
	/* Execute */
	int err = 0;

	if( state->onExit != NULL )
	{
		err = state->onExit( state, event );
	}

	/* Exit */
	return err;
}

// ############################################################################
// ############################################################################
// Function definitions

/**
 * \brief Creates the hierarchical state machine handle.
 *
 * For valid hsm machine initial state must be a top level state. (no parent)
 *
 * Use \see hsm_build instead.
 *
 * \param[in] initialState  The state that the HSM will begin with.
 * \param[in] allStates     A list with all the hsm's states.
 * \param[in] allStatesSize The number of the hsm's states.
 *
 * \return The hierarchical state machine handle.
 */
hsm_t _hsm_build( const state_t *initialState, state_t *allStates[],
                  const uint32_t allStatesSize )
{
	/* Build it */
	hsm_t aux;
	/* TODO: Check return code. */
	(void)hsm_init( &aux, initialState, allStates, allStatesSize );
	return aux;
}

/**
 * \brief Resets the hierarchical state machine and all its states.
 *
 * \param[in,out] me The hierarchical state machine handle.
 *
 * \retval 0  Success.
 * \retval -1 Failure.
 */
int hsm_reset( hsm_t *me )
{
	/* Check valid input */
	int errorCode = 0L;

	if ( me == NULL )
	{
		/* Invalid input. */
		errorCode = -1L;
	}

	if( errorCode == 0L )
	{
		/* Reset hsm */
		const bool initSuccess = hsm_init( me, me->itsInitialState, me->allStates, me->allStatesSize );
		if (!initSuccess)
		{
			/* Failed to initialize the HSM */
			errorCode = -1L;
		}
	}

	return errorCode;
}

/**
 * \brief Hierarchical state machine event handler.
 *
 * This is the handlers algorithm
 * \dot
 * 	digraph HSM{
 * 		graph [fontsize=10 fontname="Verdana" compound=true rankdir=LR];
 * 		node [shape=record fontsize=10 fontname="Verdana"];
 *
 * 		onEntry [ label="onEntry" ];
 * 		during [ label="during" ];
 * 		guard [ label="guard" ];
 * 		action [ label="action" ];
 * 		onExit [ label="onExit" ];
 *
 * 		onEntry -> during;
 * 		during -> guard [ label = "" ];
 * 		guard -> during [ label = "F" ];
 * 		guard -> action [ label = "T" ];
 * 		guard -> onExit [ label = "NULL" ];
 * 		action -> onExit;
 * 		onExit -> onEntry;
 * 	}
 * \enddot
 *
 * \param[in] me    The hierarchical state machine handle.
 * \param[in] event The event signal.
 *
 * \retval 1  No event signal given.
 * \retval 0  Success.
 * \retval -1 Failure.
 */
int hsm_handleEvent( hsm_t *me, const hsm_event_t *event )
{
	int errorCode = 0L;

	/* Check valid input */
	if( me == NULL )
	{
		/* Invalid input. */
		errorCode = -1L;
	}

	if (errorCode == 0L)
	{
		if (me->itsCurrentState == NULL)
		{
			/* Invalid input. */
			errorCode = -1L;
		}
	}

	state_t *newState = NULL;
	char* msg = NULL;
	if( errorCode == 0L )
	{
		hsm_st_mode_t next_state_mode = HSM_ST_M_ERROR;

		/* Handle event */
		state_t * const oldState = me->itsCurrentState;
		newState = me->itsCurrentState;

		switch( oldState->itsMode )
		{
			case HSM_ST_M_ON_ENTRY:
			{
				/*
				 * Go to parent/child
				 */
				if( state_hasChild( oldState ) )
				{
					/* Change current state */
					newState = ( state_t * )oldState->itsInitialState;
					msg = "Top --> Child";
				}
				else
				{
					newState = state_getTop( oldState );
					msg = "Child --> Top";
				}

				/* Execute on_entry */
				if( state_exec_onEntry( oldState, event ) )
				{
					/* Fail */
					errorCode = -1L;
				}

				/* Go to next mode */
				next_state_mode = HSM_ST_M_DURING;
				break;
			}
			case HSM_ST_M_DURING:
			{
				/*
				 * Go to parent/child
				 */
				if( state_hasChild( oldState ) )
				{
					/* Change current state */
					if( oldState->itsInitialState->itsMode == HSM_ST_M_DURING )
					{
						newState = ( state_t * )oldState->itsInitialState;
					}
					else
					{
						newState = oldState->itsHistoryState;//BUG
					}

					msg = "Top --> Child";
				}
				else
				{
					newState = state_getTop( oldState );
					msg = "Child --> Top";
				}

				/* Execute during */
				if( state_exec_during( oldState, event ) )
				{
					/* Fail */
					errorCode = -1L;
				}

				/* Go to next mode */
				next_state_mode = HSM_ST_M_CHECKING_GUARD;
				break;
			}
			case HSM_ST_M_CHECKING_GUARD:
			{
				hsm_st_mode_t new_state_mode = oldState->itsMode;

				/* Check guard */
				if( oldState->itsTransition == NULL )
				{
					if( state_hasChild( oldState ) )
					{
						/* Transition == NULL and no child */
						new_state_mode = HSM_ST_M_DURING;
						/* Change current state */
						newState = oldState->itsHistoryState;
						msg = "(Transition == NULL): Top --> Child";
					}
					else
					{
						newState = state_getTop( oldState );
						msg = "Child --> Top";
					}
				}
				else
				{
					/* Check guard */
					if( oldState->itsTransition[0].guard == NULL )
					{
						/* No guard */
						new_state_mode = HSM_ST_M_ON_EXIT;
						msg = "(No guard): Top --> Child";
					}
				}

				/* Execute guard */
				//

				/* Go to next mode */
				next_state_mode = new_state_mode;
				break;
			}
			case HSM_ST_M_ON_EXIT:
			{
				/*  */
				if( oldState->itsTransition != NULL )
				{
					newState = oldState->itsTransition[0].targetState;
				}

				/* Change state */
				if( state_hasParent( oldState ) )
				{
					if( oldState->itsParentState == newState->itsParentState )
					{
						/*
						 * Have common parent (but not NULL)
						 */
						state_t *parent = ( state_t * )oldState->itsParentState;
						parent->itsHistoryState = newState;
						msg = "(Common parent): Child --> Child";
					}
					else
					{
						/*
						 * We exit a parent state so parent should exit also
						 */
						newState = ( state_t * )oldState->itsParentState;
						newState->itsMode = HSM_ST_M_ON_EXIT;
						msg = "(Parent shouldExit also) --> ";
					}
				}
				else
				{
					if( oldState->itsTransition == NULL )
					{
						//printf( "Name: %s\n", oldState->itsHistoryState->itsTransition[0].targetState->itsName );
						newState = state_getTop(
					                   oldState->itsHistoryState->itsTransition[0].targetState );
						msg = "(Change TOP)Top --> Top";
					}
					else
					{
						newState = state_getTop( newState );
						msg = "? ? ? (Change TOP)Top --> Top";
					}
				}

				/* Execute onExit */
				if( state_exec_onExit( oldState, event ) )
				{
					/* Fail */
					errorCode = -1L;
				}

				/* Current state is old now */
				next_state_mode = HSM_ST_M_ON_ENTRY;

				break;
			}
			default:
			{
				/* Fail */
				errorCode = -1L;
			}
		}

		hsm_debug( oldState, newState, msg );
		oldState->itsMode = next_state_mode;
	}

	if( errorCode == 0L )
	{
		me->itsCurrentState = newState;

		/* Check for event signal */
		if( event == NULL )
		{
			/* No event signal given */
			errorCode = 1L;
		}
	}

	/* Success */
	return errorCode;
}
