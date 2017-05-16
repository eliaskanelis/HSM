/******************************************************************************
	Code
******************************************************************************/

/******************************************************************************
	Include files
******************************************************************************/

#include "hsm.h"

#include <stddef.h>		/* NULL */
#include <stdint.h>		/* uint32_t */
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
	obj->itsInitialState = (state_t*)initialState;
	obj->itsCurrentState = (state_t*)initialState;
	obj->itsMode = HSM_ON_ENTRY;
	obj->itsNextState = NULL;
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
 * 
 * \retval			0		Action didn't run.
 * \retval			1		Action run and it was the last one.
 * \retval			2		Action run but has more children actions to run.
 */
uint32_t hsm_state_onEntry( state_t** me, hsm_event_t* event )
{
	/* Check input */
	if( *me == NULL )
	{
		/* Exit action didn't run */
		return 0;
	}
	
	/* Init */
	state_t* state = *me;
	
	/* 
	 * Check for valid action
	 */
	if( state->onEntry != NULL )
	{
		/* The action was valid. Run it */
		state->onEntry( state, event );
	}
	
	/* 
	 * Find the next state,
	 * and exit
	 */
	if( hsm_state_hasChild( state ) )
	{
		/* Set next state */
		*me = state->itsHistoryState;
		
		/* Exit with action run but has more */
		return 2;
	}
	else
	{
		/* Set next state */
		*me = hsm_state_getTopState( state );
		
		/* Exit with last action run */
		return 1;
	}
}

/**
 * \brief Runs during action for the current state's history tree.
 * 
 * \param[in]	me		The hsm state.
 * \param[in]	event	The event signal.
 * 
 * \retval			0		Error.
 * \retval			1		Action run and it was the last one.
 * \retval			2		Action run but has more children actions to run.
 */
uint32_t hsm_state_during( state_t** me, hsm_event_t* event )
{
	/* Check input */
	if( *me == NULL )
	{
		/* Exit with error */
		return 0;
	}
	
	/* Init */
	state_t* state = *me;
	
	/* 
	 * Check for valid action
	 */
	if( state->during != NULL )
	{
		/* The action was valid. Run it */
		state->during( state, event );
	}
	
	/* 
	 * Find the next state,
	 * and exit
	 */
	if( hsm_state_hasChild( state ) )
	{
		/* Set next state */
		*me = state->itsHistoryState;
		
		/* Exit with action run but has more */
		return 2;
	}
	else
	{
		/* Set next state */
		*me = hsm_state_getTopState( state );
		
		/* Exit with last action run */
		return 1;
	}
}

/**
 * \brief Checks for transition guards for the current state's history tree.
 * 
 * \param[in]	me		The hsm state.
 * \param[in]	event	The event signal.
 * 
 * \retval		0		Guard found.
 * \retval		1		Guard not found and it was the last one.
 * \retval		2		Guard not found but has more children to check.
 * \retval		3		Error.
 */
uint32_t hsm_state_guard( state_t** me, hsm_event_t* event, uint32_t* transition_id )
{
	/* Check input */
	if( ( *me == NULL ) || ( transition_id == NULL ) )
	{
		/* Exit with error */
		return 3;
	}
	
	/* Init */
	state_t* state = *me;
	
	/*
	 * For all the transitions
	 * check guards
	 */
	for( uint32_t i = 0; i<state->itsTransitionNum; i++ )
	{
		/* Check guard */
		int isGuardAllows = 0;//We assume guard does not allow
		if ( state->itsTransition[i].guard == NULL )
		{
			isGuardAllows = 1;/* Guard allows */
		}
		else if ( state->itsTransition[i].guard( state, event ) )
		{
			isGuardAllows = 1;/* Guard allows */
		}
		
		/* Run action */
		if( isGuardAllows )
		{
			/* Get the transition id */
			*transition_id = i;
			
			/* Exit with guard found */
			return 0;
		}
	}
	
	/* 
	 * All transitions were checked and no guards allowed access,
	 * we now need to set the next state
	 */
	if( hsm_state_hasChild( state ) )
	{
		/* Set next state */
		*me = state->itsHistoryState;
		
		/* Exit with guard not found but has more children to check */
		return 2;
	}
	else
	{
		/* Set next state */
		*me = hsm_state_getTopState( state );
		
		/* Exit with guard not found and it was the last one */
		return 1;
	}
}

/**
 * \brief Runs action for the given state and gives the next state.
 * 
 * \param[in,out]	me				The hsm state.
 * \param[in]		event			The event signal.
 * \param[in]		transition_id	Transition id.
 * 
 * \retval			0				Success.
 * \retval			1				Fail.
 */
uint32_t hsm_state_action( state_t** me, hsm_event_t* event, const uint32_t transition_id )
{
	/* Check input */
	if( *me == NULL )
	{
		/* Exit with fail */
		return 1;
	}
	
	/* Init */
	state_t* state = *me;
	
	/* Check for valid action */
	if ( state->itsTransition[transition_id].action != NULL )
	{
		/* It is a valid action so run it */
		state->itsTransition[transition_id].action( state, event );
	}
	
	/* Set next transition */
	*me = state->itsTransition[transition_id].targetState;
	
	/* Exit with success */
	return 0;
}

/**
 * \brief Runs onExit action for the current state's history tree.
 * 
 * \param[in,out]	me				The hsm state.
 * \param[in]		event			The event signal.
 * 
 * \retval			0				Error.
 * \retval			1				Action run and it was the last one.
 * \retval			2				Action run but has more parent actions to run.
 */
uint32_t hsm_state_onExit_new( state_t** me, hsm_event_t* event )
{
	/* Check input */
	if( *me == NULL )
	{
		/* Exit with fail */
		return 0;
	}
	
	/* Init */
	state_t* state = *me;
	
	/* Check fir valid exit action */
	if( state->onExit != NULL )
	{
		/* Run exit action */
		state->onExit( state, event );
	}
	
	/* Get parent */
	if( hsm_state_hasParent( state ) )
	{
		/* Set next state */
		*me = state->itsParentState;
		
		/* Exit with action run but has more parents */
		return 2;
	}
	else
	{
		/* Set next state */
		//TODO: Set new state
		
		/* Exit with last action run */
		return 1;
	}
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
		/* Run on exit action */
		if( state->onExit != NULL )
		{
			state->onExit( state, event );
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
hsmMode_t hsm_handleEvent( hsm_t* me, hsm_event_t* event )
{
	/* Init */
	state_t* curr = me->itsCurrentState;
	
	/* State machine for the state machine!!! */
	switch ( me->itsMode )
	{
		case HSM_ON_ENTRY:
		{
			/* On entry */
			int err = hsm_state_onEntry( &curr, event );
			me->itsCurrentState = curr;
			if( err == 0 )
			{
				/* Entry didn't run */
				me->itsMode = HSM_ERROR;
			}
			else if( err == 1 )
			{
				/* Last entry run */
				me->itsMode = HSM_DURING;
			}
			else
			{
				/* Entry run but has more children */
			}
			
			break;
		}
		case HSM_DURING:
		{
			/* During */
			uint32_t err = hsm_state_during( &curr, event );
			me->itsCurrentState = curr;
			
			if( err == 0 )
			{
				/* Error */
				me->itsMode = HSM_ERROR;
			}
			else if( err == 1 )
			{
				/* Last during action run */
				me->itsMode = HSM_GUARD;
			}
			else
			{
				/* During action run but has more children */
			}
			
			break;
		}
		case HSM_GUARD:
		{
			/* Guard */
			uint32_t err = hsm_state_guard( &curr, event, &(me->transition_id) );
			me->itsCurrentState = curr;
			if ( err == 0 )
			{
				/* Allows transition */
				me->itsMode = HSM_ACTION;
			}
			else if ( err == 1 )
			{
				/* Does not allow and last */
				me->itsMode = HSM_DURING;
			}
			else if ( err == 2 )
			{
				/* Does not allow and has more children */
			}
			else
			{
				/* Error */
				me->itsMode = HSM_ERROR;
			}
			
			break;
		}
		case HSM_ACTION:
		{
			console_t serial = console_build();
			serial.puts( "Num = " );
			serial.putNum( me->transition_id );
			serial.puts( "\n" );
			
			uint32_t err = hsm_state_action( &curr, event, me->transition_id );
			me->itsCurrentState = curr;
			
			if ( err )
			{
				/* Exit with error */
				me->itsMode = HSM_ERROR;
			}
			else
			{
				/* Exit with action run */
				me->itsMode = HSM_ON_EXIT;
			}
			break;
		}
		case HSM_ON_EXIT:
		{
			uint32_t err = hsm_state_onExit_new( &curr, event );
			me->itsCurrentState = curr;
			
			if ( err == 0 )
			{
				/* Exit with error */
				me->itsMode = HSM_ERROR;
			}
			else if ( err == 1 )
			{
				/* Exit with last action run */
				me->itsMode = HSM_ON_ENTRY;
			}
			else
			{
				/* Exit with action run, but has more parents */
			}
			
			
			
			
			
			
//			/* 
//			 * Compare current vs nextState
//			 * 
//			 * If current parent and next have the same parent
//			 * then the parent should not exit.
//			 */
//			int flag = 1;
//			if ( curr->itsParentState == me->itsNextState->itsParentState )
//			{
//				if ( hsm_state_getParentDepth( me->itsNextState ) > 0 )
//				{
//					flag = 0;
//				}
//			}
//			
//			/* On exit */
//			if ( flag == 0 )
//			{
//				hsm_state_onExit( curr, event, 0 );
//			}
//			else
//			{
//				hsm_state_onExit( curr, event, 1 );
//			}
//			
//			/* Debug */
//			console_t serial = console_build();
//			serial.puts( "\nTransition: " );
//			serial.puts( hsm_state_getBottomState(curr)->itsName );
//			serial.puts( " ---> " );
//			serial.puts( me->itsNextState->itsName );
//			serial.puts( "\n" );
//			
//			/* Set this child as parent's history */
//			if( hsm_state_hasParent( me->itsNextState ) )
//			{
//				me->itsNextState->itsParentState->itsHistoryState = me->itsNextState;
//			}
//			
//			/* Set next state */
//			me->itsCurrentState = me->itsNextState;
//			
//			/* Set history */
//			//TODO: Change currents history.
//			
//			me->itsMode = HSM_ON_ENTRY;
			
			break;
		}
		case HSM_ERROR:
		{
			while(1){};
			break;
		}
		default:
		{
			while(1){};
			break;
		}
	}
	
	
	return me->itsMode;
}