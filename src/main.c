/******************************************************************************
	Code
******************************************************************************/

/*! \mainpage HOME
 *
 * \section intro_sec Introduction
 *
 * Hierarchical state machine
 */

/******************************************************************************
	Include files
******************************************************************************/

#include "newSys.h"
#include "console.h"

#include <stdint.h>
#include <stdbool.h>
#include <windows.h>	/* Sleep */

/******************************************************************************
	Function definitions
******************************************************************************/

void debugState( state_t* me )
{
	console_t serial = console_build();
	serial.puts( "State: " );
	serial.puts( me->itsName );
	
//	serial.puts( " | " );
//	serial.putNum( hsm_state_getHistoryDepth( me ) );
//	serial.puts( " histD" );
//	serial.puts( " & " );
//	serial.putNum( hsm_state_getParentDepth( me ) );
//	serial.puts( " parD" );
	
	/* TopState */
	serial.puts( "\thasTop: " );
	serial.puts( hsm_state_getTopState( me )->itsName );
	
	/* BottomState */
	serial.puts( "\thasBot: " );
	serial.puts( hsm_state_getBottomState( me )->itsName );
	
	if( hsm_state_hasChild( me ) )
	{
		serial.puts( "\thasChild" );
	}
	if( hsm_state_hasParent( me ) )
	{
		serial.puts( "\thasPar" );
	}
	
	serial.puts( "\n" );
}

static void debugStateDefActions( hsm_t* machine, state_t* me )
{
	/* Initialize states */
	hsm_reset( machine );
	
	/* Debug */
	console_t serial = console_build();
	serial.puts( "---- For the state: " );
	serial.puts( me->itsName );
	serial.puts( " ----\n" );
	
	hsm_state_onEntry( &me, &(hsm_event_t){ 0, "" } );
	hsm_state_onEntry( &me, &(hsm_event_t){ 0, "" } );//This time it will not run
	hsm_state_during( me, &(hsm_event_t){ 0, "" } );
	hsm_state_during( me, &(hsm_event_t){ 0, "" } );
	hsm_state_checkGuard_Action( me, &(hsm_event_t){ 0, "1" } );
	hsm_state_onExit( me, &(hsm_event_t){ 0, "" }, 1 );
	hsm_state_onExit( me, &(hsm_event_t){ 0, "" }, 1 );//This time it will not run
	serial.puts( "\n" );
	
	/* Initialize states */
	hsm_reset( machine );
}

/**
 * \brief This is the starting point of every C program...
 * \return Always returns 0. I do not care about OS error reporting.
 */
int main( void )
{
	/* Build console */
	console_t serial = console_build();
	
	serial.puts( "\n\n-------- UNIT TEST --------n\n" );
//	oldSys_unit_test();
	newSys_unit_test();
	
	serial.puts( "\n\n-------- DEVELOPMENT TEST AREA 1 --------\n\n" );
//	serial.puts( "---- OldSys ----\n" );
//	debugState( &oldSys_workState );
//	debugState( &oldSys_errorState );
//	debugState( &oldSys_onState );
//	debugState( &oldSys_offState );
	serial.puts( "---- NewSys ----\n" );
	debugState( &newSys_superState );
	debugState( &newSys_onState );
	debugState( &newSys_offState );
	debugState( &newSys_outState );
	
	serial.puts( "\n\n-------- DEVELOPMENT TEST AREA 2 --------\n\n" );
//	serial.puts( "---- OldSys ----\n" );
//	oldSys_t oldSys = oldSys_build();
//	debugStateDefActions( (hsm_t*)&oldSys, &oldSys_workState );
//	debugStateDefActions( (hsm_t*)&oldSys, &oldSys_errorState );
//	debugStateDefActions( (hsm_t*)&oldSys, &oldSys_onState );
//	debugStateDefActions( (hsm_t*)&oldSys, &oldSys_offState );
	serial.puts( "---- NewSys ----\n" );
	newSys_t newSys = newSys_build();
	debugStateDefActions( (hsm_t*)&newSys, &newSys_superState );
	debugStateDefActions( (hsm_t*)&newSys, &newSys_onState );
	debugStateDefActions( (hsm_t*)&newSys, &newSys_offState );
	debugStateDefActions( (hsm_t*)&newSys, &newSys_outState );
	
	/* Build new system */
	serial.puts( "\n\n---- FSM Professional Development ----\n\n" );
	newSys_t sys = newSys_build();
	serial.puts( "Initial state " );
	serial.puts( sys.itsCurrentState->itsName );
	serial.puts( "\n\nEvent: " );
	uint32_t i = 0;
	char* s = serial.gets();
	
	/* Endless loop */
	while( 1 )
	{
		/* Handle event */
//		state_t* oldState = sys.itsCurrentState;
		newSys_handleEvent( &sys, &(hsm_event_t){ 0, s } );
		
		/* Show debug data */
//		serial.puts( "\n" );
//		serial.puts( "From " );
//		serial.puts( oldState->itsName );
//		serial.puts( " ---\" " );
//		serial.puts( s );
//		serial.puts( " \"---> " );
//		serial.puts( sys.itsCurrentState->itsName );
//		serial.puts( " | loop: " );
//		serial.putNum( i );
//		serial.puts( " | Level: " );
//		serial.putNum( sys.itsLevel );
//		serial.puts( " | " );
		serial.puts( "\n\n-----------------------------------\n" );
		serial.puts( "\n\nEvent: " );
		s = serial.gets();
		serial.puts( "\n" );
		Sleep( 100 );
		i++;
	}
	
	return 0;
}