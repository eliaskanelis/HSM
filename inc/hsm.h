// ############################################################################
// ############################################################################
// About

/**
 * \file     hsm.h
 *
 * \brief    Hierachical state machine.
 *
 * Created:  02/05/2017
 *
 * \author   Kanelis Elias    e.kanelis@voidbuffer.com
 */

/**
 * \defgroup Ungrouped    Ungrouped
 *
 * \code
 * #include "hsm.h"
 * \endcode
 */

// ############################################################################
// ############################################################################
// Code

#ifndef HSM_H_ONLY_ONE_INCLUDE_SAFETY
#define HSM_H_ONLY_ONE_INCLUDE_SAFETY

#ifdef __cplusplus
extern "C"
{
#endif

// ############################################################################
// ############################################################################
// Dependencies

#include <stdint.h>
#include <stdbool.h>

// ############################################################################
// ############################################################################
// Types

/**
 * \brief HSM state alias.
 */
typedef struct state state_t;

/**
 * \brief HSM event signal.
 */
typedef struct
{
	uint32_t eventType; /**< The event type. */
	void *data; /**< Opague pointer data argument. */
} hsm_event_t;

/**
 * \brief HSM state transitions.
 */
typedef struct
{
	bool ( *const guard )( const state_t *me,
	                       const hsm_event_t *event ); /**< The transitions's guard. */
	void ( *const action )( const state_t *me,
	                        const hsm_event_t *event ); /**< The transitions's action. */
	state_t *targetState; /**< The transitions's target state. */
} hsm_transition_t;

/**
 * \brief HSM state mode.
 */
typedef enum
{
	HSM_ST_M_ON_ENTRY = 0u, /**< The HSM state is in onEntry action. */
	HSM_ST_M_DURING, /**< The HSM state is in during action. */
	HSM_ST_M_CHECKING_GUARD, /**< The HSM state is checking guards. */
	HSM_ST_M_TAKING_ACTION, /**< The HSM state is taking action. */
	HSM_ST_M_ON_EXIT, /**< The HSM state is in onExit. */
	HSM_ST_M_ERROR /**< The HSM state is in error. */
} hsm_st_mode_t;

/**
 * \brief HSM state.
 */
struct state
{
	/* Initialize and do not change again */
	const state_t
	*itsInitialState; /**< If it has children this is the initial. */
	const state_t *const itsParentState; /**< The state's parent. */
	bool ( *const onEntry )( const state_t *me,
	                         const hsm_event_t *event ); /**< The state's on entry action. */
	bool ( *const during )( const state_t *me,
	                        const hsm_event_t *event ); /**< The state's during action. */
	bool ( *const onExit )( const state_t *me,
	                        const hsm_event_t *event ); /**< The state's on exit action. */
	const hsm_transition_t *const
	itsTransition; /**< The state's transition. */
	const uint32_t
	itsTransitionNum; /**< The state's transition number. */

	/* Private data, do not touch */
	hsm_st_mode_t itsMode; /**< The state mode. */
	state_t *itsHistoryState; /**< If it has children, this is the history pseudostate. */

	const char *const itsName; /**< TODO: Delete. */
};

/**
 * \brief Hierarchical state machine
 */
typedef struct
{
	const state_t *itsInitialState; /**< The first state to enter. */
	state_t *itsCurrentState; /**< The current state. */
	state_t **allStates; /**< A list with all the hsm's states. */
	uint32_t allStatesSize; /**< The number of the hsm's states. */
} hsm_t;

// ############################################################################
// ############################################################################
// Function declarations

// cppcheck-suppress misra-c2012-2.5
/**
 * \brief Creates the hierarchical state machine handle.
 *
 * For valid hsm machine initial state must be a top level state. (no parent)
 *
 * \param[in] initialState The state that the HSM will begin with.
 * \param[in] allStates    A list with all the hsm's states.
 *
 * \return The hierarchical state machine handle.
 */
#define hsm_build( initialState, allStates ) _hsm_build( (initialState), (allStates), (sizeof(allStates)/sizeof(allStates[0])) )

/*
 * Basic
 */
hsm_t _hsm_build( const state_t *initialState, state_t *allStates[],
                  const uint32_t allStatesSize );

int hsm_reset( hsm_t *me );

int hsm_handleEvent( hsm_t *me, const hsm_event_t *event );

#ifdef __cplusplus
}
#endif

#endif /* HSM_H_ONLY_ONE_INCLUDE_SAFETY */
