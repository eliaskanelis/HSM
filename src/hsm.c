/******************************************************************************
	Code
******************************************************************************/

/******************************************************************************
	Include files
******************************************************************************/

#include "hsm.h"

#include <stddef.h>		/* NULL */
#include <stdbool.h>	/* bool */
#include "console.h"//TODO: JOB: Erase me.

/******************************************************************************
	Local function declarations
******************************************************************************/

/**
 * \brief Initializes the hierarchical state machine.
 * 
 * \param[in]	obj				The hierarchical state machine handle.
 * \param[in]	initialState	The state that the HSM will begin with. It must be a top state( no parents )
 */
static void hsm_init( hsm_t* obj, const state_t* initialState )
{
	obj->itsCurrentState = (state_t*)initialState;
	obj->itsLevel = 0;
	obj->itsMode = HSMM_ON_ENTRY;
}

/******************************************************************************
	Function definitions
******************************************************************************/

/**
 * \brief Hierarchical state machine builder.
 * 
 * \param[in]	initialState	The state that the HSM will begin with. It must be a top state( no parents )
 * 
 * \return		The hierarchical state machine handle.
 */
hsm_t hsm_build( const state_t* initialState )
{
	/* Build it */
	hsm_t aux;
	hsm_init( &aux, initialState );	
	return aux;
}

/**
 * \brief Reset the hierarchical state machine.
 * 
 * It does not reset the states.
 * 
 * \param[in]	me		The hierarchical state machine handle.
 */
void hsm_reset( hsm_t* me )
{
	state_t* initialState = me->itsInitialState;
	hsm_init( me, initialState );
}

/**
 * \brief Resets the state to its initial condition.
 * 
 * \param[in]	me		The hsm state.
 */
void hsm_state_reset( state_t* me )
{
	/* Reset states */
	me->isActivated = false;
	me->itsHistoryState = (state_t*)me->itsInitialState;
}

/**
 * \brief Checks if the state has a child.
 * 
 * \param[in]	me		The hsm state.
 * 
 * \retval		0		Has no children.
 * \retval		1		Has children.
 */
int hsm_state_hasChild( const state_t* me )
{
	/* Reset states */
	if ( me->itsInitialState == NULL )
	{
		/* Exit with has no child */
		return 0;
	}
	else
	{
		/* Return with has child */
		return 1;
	}
}

/**
 * \brief Checks if the state has a parent.
 * 
 * \param[in]	me		The hsm state.
 * 
 * \retval		0		Has no parent.
 * \retval		1		Has parent.
 */
int hsm_state_hasParent( const state_t* me )
{
	/* Check parent */
	if ( me->itsParentState == NULL )
	{
		/* Exit with has no parent */
		return 0;
	}
	else
	{
		/* Return with has parent */
		return 1;
	}
}

///**
// * \brief Gets the history depth number.
// * 
// * \param[in]	me		The hsm state.
// * 
// * \return		The history depth number.
// */
//uint32_t hsm_state_getHistoryDepth(state_t* me)
//{
//	/* Init */
//	uint32_t itsHistoryDepth = 0;
//	const state_t* state = me;
//	
//	/* Find depth */
//	do
//	{
//		/* Check for depth */
//		if ( state->itsHistoryState != NULL )
//		{
//			/* It has a child */
//			itsHistoryDepth++;
//			state = state->itsHistoryState;
//		}
//	} while( state == NULL );
//	
//	/* Exit with history depth */
//	return itsHistoryDepth;
//}

/**
 * \brief Gets the parent depth number.
 * 
 * \param[in]	me		The hsm state.
 * 
 * \return		The parent depth number.
 */
uint32_t hsm_state_getParentDepth( const state_t* me )
{
	/* Init */
	uint32_t itsParentDepth = 0;
	state_t* state = (state_t*)me;
	
	/* Find depth */
	do
	{
		/* Check for depth */
		if ( state->itsParentState != NULL )
		{
			/* It has a parent */
			itsParentDepth++;
			state = state->itsParentState;
		}
	} while( state == NULL );
	
	/* Exit with parent depth */
	return itsParentDepth;
}

/**
 * \brief Runs onEntry action for the current state's history tree.
 * 
 * \param[in,out]	me		The hsm state.
 * \param[in]		event	The event signal.
 */
void hsm_state_onEntry( state_t** me, hsm_event_t* event )
{
	/* Init */
	state_t* state = hsm_state_getTopState( *me );
	
	/* Run for all children */
	int hasChildren = 0;//We assume it has no children
	do
	{
		 /* Check first run */
		if ( state->isActivated == false )
		{
			/* Activate it */
			state->isActivated = true;
			
			/* Run entry action */
			if( state->onEntry != NULL )
			{
				state->onEntry( state, event );
			}
		}
		
		/* Get children */
		hasChildren = hsm_state_hasChild( state );
		if( hasChildren )
		{
			state = state->itsHistoryState;
		}
		
	} while( hasChildren );
	
	*me = state;
}

/**
 * \brief Runs during action for the current state's history tree.
 * 
 * \param[in]	me		The hsm state.
 * \param[in]	event	The event signal.
 */
void hsm_state_during( const state_t* me, hsm_event_t* event )
{
	/* Init */
	state_t* state = hsm_state_getTopState( me );
	
	/* Run for all children */
	int hasChildren = 0;//We assume it has no children
	do
	{
		/* Run during action */
		if( state->during != NULL )
		{
			state->during( state, event );
		}
		
		/* Get children */
		hasChildren = hsm_state_hasChild( state );
		if( hasChildren )
		{
			state = state->itsHistoryState;
		}
		
	} while( hasChildren );
}

/**
 * \brief Checks for transition guards for the current state's history tree.
 * 
 * When a guard is reached and found true then its action is run and exits.
 * 
 * \param[in]	me		The hsm state.
 * \param[in]	event	The event signal.
 * 
 * \return		The next state address or NULL if any guard is not passed.
 */
state_t* hsm_state_checkGuard_Action( const state_t* me, hsm_event_t* event )
{
	/* Init */
	state_t* state = hsm_state_getTopState( me );
	int guard = 0;//We assume guard is false
	
	/* Run for all children */
	int hasChildren = 0;//We assume it has no children
	state_t* nextState = NULL;//We assume there is not a next state.
	do
	{
		/* For all the transitions */
		guard = 0;//We assume guard is false
		for( uint32_t trans_i = 0; trans_i<state->itsTransitionNum; trans_i++ )
		{
			/* Check guard */
			if ( state->itsTransition[trans_i].guard == NULL )
			{
				guard = 1;
			}
			else if ( state->itsTransition[trans_i].guard( state, event ) )
			{
				guard = 1;
			}
			
			/* Run action */
			if( guard )
			{
				/* Set next transition */
				nextState = state->itsTransition[trans_i].targetState;
				
				/* Run action */
				if ( state->itsTransition[trans_i].action != NULL )
				{
					state->itsTransition[trans_i].action( state, event );
				}
			}
		}
		
		/* Get children */
		hasChildren = hsm_state_hasChild( state );
		if( hasChildren )
		{
			state = state->itsHistoryState;
		}
		
	} while ( hasChildren && (!guard) );
	
	return nextState;
}

/**
 * \brief Runs onExit action for the current state's history tree.
 * 
 * \param[in]	me				The hsm state.
 * \param[in]	event			The event signal.
 * \param[in]	parentLevel		The parent depth to let exit.
 */
void hsm_state_onExit( const state_t* me, hsm_event_t* event, const uint32_t parentLevel )
{
	/* Init */
	state_t* state = hsm_state_getBottomState( me );
	int hasParent = false;//We assume it does not have a parent
	
	/* For all parents */
	uint32_t i = 0;
	do
	{
		/* Only for activated states */
		if( state->isActivated )
		{
			/* Run on exit action */
			if( state->onExit != NULL )
			{
				state->onExit( state, event );
			}
			
			/* Deactivate it */
			state->isActivated = false;
		}
		
		/* Get parent */
		hasParent = hsm_state_hasParent( state );
		if( hasParent )
		{
			state = state->itsParentState;
		}
		
		i++;
		
	} while( hasParent && ( parentLevel >= i ) );
}

/**
 * \brief Gets the top hsm state
 * 
 * \param[in]	me		The hsm state.
 * 
 * \return		The top hsm state.
 */
state_t* hsm_state_getTopState( const state_t* me )
{
	/* Init */
	state_t* topState = (state_t*)me;//We assume this is the top state.
	
	/* Find depth */
	do
	{
		/* Check for parent */
		if ( topState->itsParentState != NULL )
		{
			/* It has a parent, so the parent is the top state */
			topState = topState->itsParentState;
		}
	} while( topState == NULL );
	
	/* Exit with parent */
	return topState;
}

/**
 * \brief Gets the bottom hsm state
 * 
 * \param[in]	me		The hsm state.
 * 
 * \return		The bottom hsm state.
 */
state_t* hsm_state_getBottomState( const state_t* me )
{
	/* Init */
	state_t* bottomState = (state_t*)me;//We assume this is the bottom state.
	
	/* Find depth */
	do
	{
		/* Check for children */
		if ( bottomState->itsHistoryState != NULL )
		{
			/* It has a child, so the child is the bottom state */
			bottomState = bottomState->itsHistoryState;
		}
	} while( bottomState == NULL );
	
	/* Exit with parent */
	return bottomState;
}

/**
 * \brief Hierarchical state machine event handler.
 * 
 * This is the handlers algorithm
 * \dot
 * digraph HSM{
 * 		node [ shape=box, splines="curved" ];
 * 		onEntry [ label="onEntry" ];
 * 		during [ label="during" ];
 * 		guard [ label="guard" ];
 * 		action [ label="action" ];
 * 		onExit [ label="onExit" ];
 * 
 * 		onEntry -> during;
 * 		during -> guard [ label = "" ];
 * 		guard -> action [ label = "T" ];
 * 		guard -> during [ label = "F" ];
 * 		action -> onExit;
 * 		onExit -> onEntry;
 * }
 * \enddot
 * 
 * \param[in]	me		The hierarchical state machine handle.
 * \param[in]	event	The event signal.
 * 
 * \retval		0		The hsm did not change state.
 * \retval		1		The hsm changed state.
 */
int hsm_handleEvent( hsm_t* me, hsm_event_t* event )
{
	/* Init */
	state_t* curr = me->itsCurrentState;
	
	/* On entry */
	hsm_state_onEntry( &curr, event );
	
	/* During */
	hsm_state_during( curr, event );
	
	/* Guard/Action */
	state_t* nextState = hsm_state_checkGuard_Action( curr, event );
	if( nextState == NULL )
	{
		return 1;
	}
	
	/* 
	 * Compare current vs nextState
	 * 
	 * If current parent and next have the same parent
	 * the the parent should not exit.
	 */
	int flag = 1;
	if ( curr->itsParentState == nextState->itsParentState )
	{
		if ( hsm_state_getParentDepth( nextState ) > 0 )
		{
			flag = 0;
		}
	}
	
	/* On exit */
	if ( flag == 0 )
	{
		hsm_state_onExit( curr, event, 0 );
	}
	else
	{
		hsm_state_onExit( curr, event, 1 );
	}
	
	/* Debug */
	console_t serial = console_build();
	serial.puts( "\nTransition: " );
	serial.puts( hsm_state_getBottomState(curr)->itsName );
	serial.puts( " ---> " );
	serial.puts( nextState->itsName );
	serial.puts( "\n" );
	
	/* Set this child as parent's history */
	if( hsm_state_hasParent( nextState ) )
	{
		nextState->itsParentState->itsHistoryState = nextState;
	}
	
	/* Set next state */
	me->itsCurrentState = nextState;
	
	/* Set history */
	//TODO: Change currents history.
	
	/* Exit */
	return 0;//TODO: Feature: Return hsm isFinished
}