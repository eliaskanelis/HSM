/******************************************************************************
	About
******************************************************************************/

/**
 * \file oldSys.h
 *
 * \brief	The old simple system machine.
 * 
 * Old machine state transitions
 * \dot
 * digraph G {
 * 
 * graph [fontsize=10 fontname="Verdana" compound=true rankdir=LR];
 * node [shape=record fontsize=10 fontname="Verdana"];
 * 
 * subgraph cluster_0 {
 * 		"ON" "OFF";
 * 		label = "WORK";
 * }
 * 
 * "ON" -> "OFF";
 * "OFF" -> "ON";
 * "ON" -> "ERROR" [ltail=cluster_0,lhead=cluster_0];
 * "ERROR" -> "ON" [ltail=cluster_0,lhead=cluster_0];
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
 * \code	#include <oldSys.h>	\endcode
 */

/******************************************************************************
	Code
******************************************************************************/

#ifndef OLDSYS_H_ONLY_ONE_INCLUDE_SAFETY
#define OLDSYS_H_ONLY_ONE_INCLUDE_SAFETY

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
	Include files
******************************************************************************/

#include "hsm.h"

extern state_t oldSys_workState;
extern state_t oldSys_errorState;
extern state_t oldSys_onState;
extern state_t oldSys_offState;

/******************************************************************************
	Type definitions
******************************************************************************/

/**
 * \brief Simple System machine.
 */
typedef struct oldSys
{
	hsm_t;			/**< It inherits from hsm_t. */
} oldSys_t;

/**
 * \brief Event signal.
 */
typedef struct oldSys_event
{
	hsm_event_t;	/**< It inherits from hsm_event_t. */
} oldSys_event_t;


/******************************************************************************
	Function declarations
******************************************************************************/

void		oldSys_unit_test( void );

oldSys_t	oldSys_build( void );
void		oldSys_reset( oldSys_t* me );
void		oldSys_handleEvent( oldSys_t* me, hsm_event_t* event );

#ifdef __cplusplus
}
#endif

#endif /* OLDSYS_H_ONLY_ONE_INCLUDE_SAFETY */
