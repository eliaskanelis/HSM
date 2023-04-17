// ############################################################################
// ############################################################################
// Code

// ############################################################################
// ############################################################################
// Include files

#include "hsm.h"

#include <stdlib.h>

// ############################################################################
// ############################################################################
// Local definitions

#ifdef DEBUG
#include <stdio.h>

static const char *hsm_state_getModeName( const state_t *state )
{
	switch( state->itsMode )
	{
		case HSM_ST_M_ON_ENTRY:
			return "onEntry";

		case HSM_ST_M_DURING:
			return "during ";

		case HSM_ST_M_CHECKING_GUARD:
			return "guard  ";

		case HSM_ST_M_TAKING_ACTION:
			return "action ";

		case HSM_ST_M_ON_EXIT:
			return "onExit ";

		default:
			return "ERROR  ";
	}
}
#endif

static void hsm_debugFrom( const char *mode, const state_t *state )
{
#ifdef DEBUG
	printf( "\x1b[32m%s\x1b[0m: \x1b[33m%s\x1b[0m.\x1b[33m%s\x1b[0m", mode,
	        state->itsName,
	        hsm_state_getModeName( state ) );
#else
	( void )mode;
	( void )state;
#endif
}

static void hsm_debugTo( const char *msg, const state_t *state )
{
#ifdef DEBUG
	printf( "\t[\x1b[34m%s\x1b[0m]\t\t\x1b[33m%s.\x1b[33m%s\x1b[0m\x1b[0m", msg,
	        state->itsName,
	        hsm_state_getModeName( state ) );
#else
	( void )msg;
	( void )state;
#endif
}

// ############################################################################
// ############################################################################
// Local functions

static void hsm_init( hsm_t *me, const state_t *initialState,
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
 */
static void hsm_init( hsm_t *me, const state_t *initialState,
                      state_t *allStates[], const uint32_t allStatesSize )
{
	/* Check valid input */
	if( ( initialState == NULL ) || ( allStates == NULL ) ||
	                ( allStatesSize == 0U ) )
	{
		me->itsInitialState = NULL;
		me->itsCurrentState = NULL;
		me->allStates = NULL;
		me->allStatesSize = 0;
		return;
	}

	/* Check if initial state */
	if( initialState->itsParentState != NULL )
	{
		/* Not a top state */
		me->itsInitialState = NULL;
		me->itsCurrentState = NULL;
		me->allStates = NULL;
		me->allStatesSize = 0;
		return;
	}

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
	hsm_init( &aux, initialState, allStates, allStatesSize );
	return aux;
}

/**
 * \brief Resets the hierarchical state machine and all its states.
 *
 * \param[in,out] me The hierarchical state machine handle.
 *
 * \retval 0  Success.
 * \retval -1 Fail.
 */
int hsm_reset( hsm_t *me )
{
	/* Check valid input */
	if( me == NULL )
	{
		/* Fail */
		return -1;
	}

	/* Reset hsm */
	hsm_init( me, me->itsInitialState, me->allStates, me->allStatesSize );

	/* Success */
	return 0;
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
 * \retval -1 Fail.
 */
int hsm_handleEvent( hsm_t *me, const hsm_event_t *event )
{
	/* Check valid input */
	if( me == NULL )
	{
		/* Fail */
		return -1;
	}

	/* Handle event */
	state_t *oldState = me->itsCurrentState;
	state_t *newState = me->itsCurrentState;

	switch( oldState->itsMode )
	{
		case HSM_ST_M_ON_ENTRY:
			hsm_debugFrom( "ON_ENTRY", oldState );

			/* Go to next mode */
			oldState->itsMode = HSM_ST_M_DURING;

			/*
			 * Go to parent/child
			 */
			if( state_hasChild( oldState ) )
			{
				/* Change current state */
				newState = ( state_t * )oldState->itsInitialState;
				hsm_debugTo( "Top --> Child", newState );
			}
			else
			{
				newState = state_getTop( oldState );
				hsm_debugTo( "Child --> Top", newState );
			}

			/* Execute on_entry */
			if( state_exec_onEntry( oldState, event ) )
			{
				/* Fail */
				return -1;
			}

			break;

		case HSM_ST_M_DURING:
			hsm_debugFrom( "DURING  ", oldState );

			/* Go to next mode */
			oldState->itsMode = HSM_ST_M_CHECKING_GUARD;

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

				hsm_debugTo( "Top --> Child", newState );
			}
			else
			{
				newState = state_getTop( oldState );
				hsm_debugTo( "Child --> Top", newState );
			}

			/* Execute during */
			if( state_exec_during( oldState, event ) )
			{
				/* Fail */
				return -1;
			}

			break;

		case HSM_ST_M_CHECKING_GUARD:
			hsm_debugFrom( "GUARD   ", oldState );

			/* Check guard */
			if( oldState->itsTransition == NULL )
			{
				if( state_hasChild( oldState ) )
				{
					/* Transition == NULL and no child */
					oldState->itsMode = HSM_ST_M_DURING;
					/* Change current state */
					newState = oldState->itsHistoryState;
					hsm_debugTo( "(Transition == NULL): Top --> Child", newState );
				}
				else
				{
					newState = state_getTop( oldState );
					hsm_debugTo( "Child --> Top", newState );
				}
			}
			else
			{
				/* Check guard */
				if( oldState->itsTransition[0].guard == NULL )
				{
					/* No guard */
					oldState->itsMode = HSM_ST_M_ON_EXIT;
					hsm_debugTo( "(No guard): Top --> Child", newState );
				}
			}

			/* Execute guard */
			//


			break;

		case HSM_ST_M_ON_EXIT:
			hsm_debugFrom( "ON_EXIT ", oldState );

			/* Current state is old now */
			oldState->itsMode = HSM_ST_M_ON_ENTRY;

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
					hsm_debugTo( "(Common parent): Child --> Child", newState );
				}
				else
				{
					/*
					 * We exit a parent state so parent should exit also
					 */
					newState = ( state_t * )oldState->itsParentState;
					newState->itsMode = HSM_ST_M_ON_EXIT;
					hsm_debugTo( "(Parent shouldExit also) --> ", newState );
				}
			}
			else
			{
				if( state_hasParent( oldState ) )
				{
					//hsm_debugTo( "(Change TOP)Top --> Child (But go to TOP)", newState );
				}
				else
				{
					if( oldState->itsTransition == NULL )
					{
						//printf( "Name: %s\n", oldState->itsHistoryState->itsTransition[0].targetState->itsName );
						newState = state_getTop(
						                   oldState->itsHistoryState->itsTransition[0].targetState );
						hsm_debugTo( "(Change TOP)Top --> Top", newState );
					}
					else
					{
						newState = state_getTop( newState );
						hsm_debugTo( "? ? ? (Change TOP)Top --> Top", newState );
					}
				}
			}

			/* Execute onExit */
			if( state_exec_onExit( oldState, event ) )
			{
				/* Fail */
				return -1;
			}

			break;

		default:
			/* Fail */
			return -1;
	}

	me->itsCurrentState = newState;

	/* Check for event signal */
	if( event == NULL )
	{
		/* No event signal given */
		return 1;
	}

	/* Success */
	return 0;
}
