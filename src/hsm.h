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

#include <stdint.h>
#include <stdbool.h>

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
 * \brief HSM mode.
 */
typedef enum
{
	HSMM_ON_ENTRY,			/**< The HSM is in onEntry action. */
	HSMM_DURING,			/**< The HSM is in during action. */
	HSMM_CHECK_GUARD,		/**< The HSM is checking guard. */
	HSMM_ON_EXIT			/**< The HSM is in onExit. */
} hsmMode_t;

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
 * \brief HSM state.
 */
struct state
{
	/* User defined */
	const char* const itsName;									/**< The first state to enter. */
	const state_t* const itsInitialState;						/**< If it has children this is the initial. */
	state_t* const itsParentState;						/**< The state's parent. */
	void (*const onEntry)( state_t* me, hsm_event_t* event );	/**< The state's on entry action. */
	void (*const during)( state_t* me, hsm_event_t* event );	/**< The state's during action. */
	void (*const onExit)( state_t* me, hsm_event_t* event );	/**< The state's on exit action. */
	const uint32_t itsTransitionNum;							/**< The state's transition number. */
	transition_t* itsTransition;								/**< The state's transition. */
	
	/* Read only, defined internally */
	bool isActivated;											/**< A state is activated after onEntry action and deactivated after onExit action. */
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
	uint32_t itsLevel;			/**< The child depth level. */
	hsmMode_t itsMode;			/**< The hsm mode. */
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

int			hsm_state_onEntry( state_t** me, hsm_event_t* event );
void		hsm_state_during( const state_t* me, hsm_event_t* event );
state_t*	hsm_state_checkGuard_Action( const state_t* me, hsm_event_t* event );
void		hsm_state_onExit( const state_t* me, hsm_event_t* event, const uint32_t parentLevel );

state_t*	hsm_state_getTopState( const state_t* me );
state_t*	hsm_state_getBottomState( const state_t* me );

int			hsm_handleEvent( hsm_t* me, hsm_event_t* event );

#endif /* HSM_H_ONLY_ONE_INCLUDE_SAFETY */
