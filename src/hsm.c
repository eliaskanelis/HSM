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

// cppcheck-suppress misra-c2012-8.9
static const char *hsm_state_modeName[] =
{
	"onEntry",
	"during ",
	"guard  ",
	"action ",
	"onExit ",
	"ERROR  "
};
#endif

static void hsm_debug( const state_t *fromState, const state_t *toState,
                       const char *msg )
{
#ifdef DEBUG
	const char *fromStateModeName = hsm_state_modeName[fromState->itsMode];
	const char *toStateModeName = hsm_state_modeName[toState->itsMode];

	const char *fmt =
	        // cppcheck-suppress misra-c2012-4.1
	        "\x1b[32m%s\x1b[0m: "
	        // cppcheck-suppress misra-c2012-4.1
	        "\x1b[33m%s\x1b[0m.\x1b[33m%s\x1b[0m"
	        // cppcheck-suppress misra-c2012-4.1
	        " ---> \x1b[33m%s.\x1b[33m%s\x1b[0m\x1b[0m"
	        // cppcheck-suppress misra-c2012-4.1
	        "   \t[\x1b[34m%-40s\x1b[0m]\t";

	( void ) printf( fmt,
	                 fromStateModeName,
	                 fromState->itsName, fromStateModeName,
	                 toState->itsName, toStateModeName,
	                 msg );
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

static bool state_exec_onEntry( const state_t *state,
                                const hsm_event_t *event );
static bool state_exec_during( const state_t *state, const hsm_event_t *event );
static bool state_exec_onExit( const state_t *state, const hsm_event_t *event );

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
	bool success = ( ( me != NULL ) && ( initialState != NULL ) &&
	                 ( allStates != NULL ) && ( allStatesSize != 0U ) );

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
		for( uint32_t i = 0U; i < me->allStatesSize; i++ )
		{
			state_reset( me->allStates[i] );
		}
	}
	else
	{
		if( me != NULL )
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
 * \retval  0 No children.
 * \retval  1 Has children.
 * \retval -1 Failure.
 */
static int state_hasChild( const state_t *state )
{
	int returnCode = 0;

	/* Validate input */
	if( state == NULL )
	{
		returnCode = -1;
	}

	if( returnCode == 0 )
	{
		if( state->itsHistoryState == NULL )
		{
			/* No children */
			returnCode = 0;
		}
		else
		{
			/* Has children */
			returnCode = 1;
		}
	}

	return returnCode;
}

/**
 * \brief Checks is state has parent.
 *
 * \param[in] state The hsm state.
 *
 * \retval  0 No parent.
 * \retval  1 Has parent.
 * \retval -1 Failure.
 */
static int state_hasParent( const state_t *state )
{
	int returnCode = 0;

	/* Validate input */
	if( state == NULL )
	{
		returnCode = -1;
	}

	if( returnCode == 0 )
	{
		if( state->itsParentState == NULL )
		{
			/* No parent */
			returnCode = 0;
		}
		else
		{
			/* Has parent */
			returnCode = 1;
		}
	}

	return returnCode;
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
	/* We assume this is the top state.*/
	state_t *topState = ( state_t * )state;

	/* Find top parent */
	do
	{
		/* Check for parent */
		const int hasParentReturnCode = state_hasParent( state );

		if( hasParentReturnCode == 1 )
		{
			/* It has a parent, so the parent is the top state */
			topState = ( state_t * )topState->itsParentState;
		}
		else if( hasParentReturnCode == 0 )
		{
			/* No action */
		}
		else
		{
			/* TODO: Handle error */
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
 * \retval True  Success.
 * \retval False Failure.
 */
static bool state_exec_onEntry( const state_t *state, const hsm_event_t *event )
{
	/* Validate inputs */
	bool success = ( state != NULL );

	if( success )
	{
		if( state->onEntry != NULL )
		{
			success = state->onEntry( state, event );
		}
	}

	return success;
}

/**
 * \brief Executes states during action.
 *
 * \param[in] state The hsm state.
 * \param[in] event The event signal.
 *
 * \retval True  Success.
 * \retval False Failure.
 */
static bool state_exec_during( const state_t *state, const hsm_event_t *event )
{
	/* Validate inputs */
	bool success = ( state != NULL );

	if( success )
	{
		if( state->during != NULL )
		{
			success = state->during( state, event );
		}
	}

	return success;
}

/**
 * \brief Executes states on exit action.
 *
 * \param[in] state The hsm state.
 * \param[in] event The event signal.
 *
 * \retval True  Success.
 * \retval False Failure.
 */
static bool state_exec_onExit( const state_t *state, const hsm_event_t *event )
{
	/* Validate inputs */
	bool success = ( state != NULL );

	if( success )
	{
		if( state->onExit != NULL )
		{
			success = state->onExit( state, event );
		}
	}

	return success;
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
	( void )hsm_init( &aux, initialState, allStates, allStatesSize );
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
	int errorCode = 0;

	if( me == NULL )
	{
		/* Invalid input. */
		errorCode = -1;
	}

	if( errorCode == 0L )
	{
		/* Reset hsm */
		const bool initSuccess = hsm_init( me, me->itsInitialState, me->allStates,
		                                   me->allStatesSize );

		if( !initSuccess )
		{
			/* Failed to initialize the HSM */
			errorCode = -1;
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
 * \retval  1  No event signal given.
 * \retval  0  Success.
 * \retval -1 Failure.
 */
int hsm_handleEvent( hsm_t *me, const hsm_event_t *event )
{
	int errorCode = 0;

	/* Check valid input */
	if( me == NULL )
	{
		/* Invalid input. */
		errorCode = -1;
	}

	if( errorCode == 0 )
	{
		if( me->itsCurrentState == NULL )
		{
			/* Invalid input. */
			errorCode = -1;
		}
	}

	state_t *newState = NULL;
	const char *msg = NULL;

	if( errorCode == 0L )
	{
		hsm_st_mode_t next_state_mode = HSM_ST_M_ERROR;

		/* Handle event */
		state_t *const oldState = me->itsCurrentState;
		newState = me->itsCurrentState;

		switch( oldState->itsMode )
		{
			case HSM_ST_M_ON_ENTRY:
			{
				/*
				 * Go to parent/child
				 */
				const int hasChildReturnCode = state_hasChild( oldState );

				if( hasChildReturnCode == 1 )
				{
					/* Change current state */
					newState = ( state_t * )oldState->itsInitialState;
					msg = "Top --> Child";
				}
				else if( hasChildReturnCode == 0 )
				{
					newState = state_getTop( oldState );
					msg = "Child --> Top";
				}
				else
				{
					/* Error */
					errorCode = -1;
				}

				/* Execute on_entry */
				if( !state_exec_onEntry( oldState, event ) )
				{
					/* Fail */
					errorCode = -1;
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
				const int hasChildReturnCode = state_hasChild( oldState );

				if( hasChildReturnCode == 1 )
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
				else if( hasChildReturnCode == 0 )
				{
					newState = state_getTop( oldState );
					msg = "Child --> Top";
				}
				else
				{
					/* Error */
					errorCode = -1;
				}

				/* Execute during */
				if( !state_exec_during( oldState, event ) )
				{
					/* Fail */
					errorCode = -1;
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
					const int hasChildReturnCode = state_hasChild( oldState );

					if( hasChildReturnCode == 1 )
					{
						/* Transition == NULL and no child */
						new_state_mode = HSM_ST_M_DURING;
						/* Change current state */
						newState = oldState->itsHistoryState;
						msg = "(Transition == NULL): Top --> Child";
					}
					else if( hasChildReturnCode == 0 )
					{
						newState = state_getTop( oldState );
						msg = "Child --> Top";
					}
					else
					{
						/* Error */
						errorCode = -1;
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

				const int hasParentReturnCode = state_hasParent( oldState );

				/* Change state */
				if( hasParentReturnCode == 1 )
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
				else if( hasParentReturnCode == 0 )
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
				else
				{
					/* Error */
					errorCode = -1;
				}

				/* Execute onExit */
				if( !state_exec_onExit( oldState, event ) )
				{
					/* Fail */
					errorCode = -1;
				}

				/* Current state is old now */
				next_state_mode = HSM_ST_M_ON_ENTRY;

				break;
			}

			default:
			{
				/* Fail */
				errorCode = -1;
			}
		}

		hsm_debug( oldState, newState, msg );
		oldState->itsMode = next_state_mode;
	}

	if( errorCode == 0 )
	{
		me->itsCurrentState = newState;

		/* Check for event signal */
		if( event == NULL )
		{
			/* No event signal given */
			errorCode = 1;
		}
	}

	/* Success */
	return errorCode;
}
