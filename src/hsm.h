/******************************************************************************
	About
******************************************************************************/

/**
 * \file hsm.h
 *
 * \brief	Hierachical state machine.
 *
 * Created:			02/05/2017
 *
 * \author	Elias Kanelis	hkanelhs@yahoo.gr
 */

/**
 * \defgroup	Services	Services
 *
 * \code	#include <hsm.h>	\endcode
 * 
 * \todo	Change into const pointers.
 * 
 */

/******************************************************************************
	Code
******************************************************************************/

#ifndef HSM_H_ONLY_ONE_INCLUDE_SAFETY
#define HSM_H_ONLY_ONE_INCLUDE_SAFETY

/******************************************************************************
	Include files
******************************************************************************/

#include <stdint.h>		/* uint32_t */
#include <stdbool.h>	/* bool */


/******************************************************************************
	Custom definitions
******************************************************************************/

#define HSM_DEBUG			/**< When defined it enables debugging log. */

/*  ---   DO NOT TOUCH BELOW THIS LINE!   ---  */

/******************************************************************************
	Definitions
******************************************************************************/

/******************************************************************************
	Type definitions
******************************************************************************/

/**
 * \brief HSM event signal.
 */
typedef struct
{
	int eventType;		/**< The event type. */
	void* data;			/**< Universal data argument. */
} hsm_event_t;

/**
 * \brief HSM state alias.
 */
typedef struct state state_t;

/**
 * \brief HSM state transitions.
 */
typedef struct
{
	bool (*const guard)( state_t* me, hsm_event_t* event );		/**< The transitions's guard. */
	void (*const action)( state_t* me, hsm_event_t* event );	/**< The transitions's action. */
	state_t* targetState;										/**< The transitions's target state. */
} transition_t;

/**
 * \brief HSM mode.
 */
typedef enum
{
	HSM_ON_ENTRY,		/**< The HSM is in onEntry action. */
	HSM_DURING,			/**< The HSM is in during action. */
	HSM_GUARD,			/**< The HSM is checking guards. */
	HSM_ACTION,			/**< The HSM is in guard's action. */
	HSM_ON_EXIT,		/**< The HSM is in onExit. */
	HSM_ERROR			/**< The HSM is in error. */
} hsmMode_t;

/**
 * \brief HSM state.
 */
struct state
{
	/* User defined */
	const char* const itsName;									/**< The first state to enter. */
	const state_t* const itsInitialState;						/**< If it has children this is the initial. */
	state_t* const itsParentState;								/**< The state's parent. */
	void (*const onEntry)( state_t* me, hsm_event_t* event );	/**< The state's on entry action. */
	void (*const during)( state_t* me, hsm_event_t* event );	/**< The state's during action. */
	void (*const onExit)( state_t* me, hsm_event_t* event );	/**< The state's on exit action. */
	const uint32_t itsTransitionNum;							/**< The state's transition number. */
	transition_t* itsTransition;								/**< The state's transition. */
	
	/* Read only, defined internally */
	state_t* itsHistoryState;									/**< If it has children, this is the history pseudostate. */
};

/**
 * \brief Hierarchical state machine
 */
typedef struct
{
	state_t* itsInitialState;	/**< The first state to enter. */
	state_t* itsCurrentState;	/**< The current state. */
	state_t* itsNextState;		/**< The next state. */
	hsmMode_t itsMode;			/**< The hsm mode. */
	
	uint32_t transition_id;//TODO: This is temporary here...
} hsm_t;

/******************************************************************************
	Function declarations
******************************************************************************/

hsm_t		hsm_build( const state_t* initialState );
void		hsm_reset( hsm_t* me );

/* State related */
void		hsm_state_reset( state_t* me );
int			hsm_state_hasChild( const state_t* me );
int			hsm_state_hasParent( const state_t* me );

//uint32_t	hsm_state_getHistoryDepth( state_t* me );
uint32_t	hsm_state_getParentDepth( const state_t* me );

uint32_t	hsm_state_onEntry( state_t** me, hsm_event_t* event );
uint32_t	hsm_state_during( state_t** me, hsm_event_t* event );
uint32_t	hsm_state_guard( state_t** me, hsm_event_t* event, uint32_t* transition_id );
uint32_t	hsm_state_action( state_t** me, hsm_event_t* event, const uint32_t transition_id );

uint32_t	hsm_state_onExit_new( state_t** me, hsm_event_t* event );

void		hsm_state_onExit( const state_t* me, hsm_event_t* event, const uint32_t parentLevel );

state_t*	hsm_state_getTopState( const state_t* me );
state_t*	hsm_state_getBottomState( const state_t* me );

hsmMode_t	hsm_handleEvent( hsm_t* me, hsm_event_t* event );

#endif /* HSM_H_ONLY_ONE_INCLUDE_SAFETY */
