/******************************************************************************
	About
******************************************************************************/

/**
 * \file newSys.h
 *
 * \brief	The new simple system machine.
 * 
 * New machine state transitions
 * \dot
 * digraph G {
 * 
 * graph [fontsize=10 fontname="Verdana" compound=true rankdir=LR];
 * node [shape=record fontsize=10 fontname="Verdana"];
 * 
 * subgraph cluster_0 {
 * 		"ON" "OFF";
 * 		label = "SUPER";
 * }
 * 
 * // Edges between nodes render fine
 * "ON" -> "OFF";
 * "OFF" -> "OUT";
 * "OUT" -> "ON";
 * }
 * \enddot
 * 
 * Created:			05/05/2017
 *
 * \author	Elias Kanelis	hkanelhs@yahoo.gr
 */

/**
* \defgroup	Application	Application
*
* \code	#include <newSys.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef NEWSYS_H_ONLY_ONE_INCLUDE_SAFETY
#define NEWSYS_H_ONLY_ONE_INCLUDE_SAFETY

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
	Include files
******************************************************************************/

#include "hsm.h"

extern state_t newSys_superState;
extern state_t newSys_onState;
extern state_t newSys_offState;
extern state_t newSys_outState;

/******************************************************************************
	Type definitions
******************************************************************************/

/**
 * \brief Simple System machine.
 */
typedef struct newSys
{
	hsm_t base;			/**< It inherits from hsm_t. */
} newSys_t;

/**
 * \brief Event signal.
 */
typedef struct newSys_event
{
	hsm_event_t base;	/**< It inherits from hsm_event_t. */
} newSys_event_t;


/******************************************************************************
	Function declarations
******************************************************************************/

void		newSys_unit_test( void );

newSys_t	newSys_build( void );
void		newSys_reset( newSys_t* me );
void		newSys_handleEvent( newSys_t* me, hsm_event_t* event );

#ifdef __cplusplus
}
#endif

#endif /* NEWSYS_H_ONLY_ONE_INCLUDE_SAFETY */
