/******************************************************************************
	Code
******************************************************************************/

/******************************************************************************
	Include files
******************************************************************************/

#include "oldSys.h"

#include "hsm.h"
#include "console.h"

#include <stddef.h>		/* NULL */
#include <stdbool.h>		/* bool */
#include "string.h"		/* strcmp */

/******************************************************************************
	Local definitions
******************************************************************************/

typedef struct
{
	char* const input;
	state_t* expectedState;
	const uint32_t expected_level;
} testData_t;

static void onEntry( state_t* me, hsm_event_t* event )
{
#ifdef HSM_DEBUG
	console_t serial = console_build();
	serial.puts( me->itsName );
	serial.puts( ":\tonEntry\n" );
#endif
};

static void during( state_t* me, hsm_event_t* event )
{
#ifdef HSM_DEBUG
	console_t serial = console_build();
	serial.puts( me->itsName );
	serial.puts( ":\tduring\n" );
#endif
};

static void onExit( state_t* me, hsm_event_t* event )
{
#ifdef HSM_DEBUG
	console_t serial = console_build();
	serial.puts( me->itsName );
	serial.puts( ":\tonExit\n" );
#endif
};

static bool guard( state_t* me, hsm_event_t* event )
{
	//console_t serial = console_build();
	//serial.puts( me->itsName );
	//serial.puts( ":\tGuard " );
	if ( strcmp( event->data, "1" ) == 0 )
	{
		//serial.puts( "Pass\n" );
		return true;
	}
	else
	{
		//serial.puts( "Failed\n" );
		return false;
	}
};

static void action( state_t* me, hsm_event_t* event )
{
	console_t serial = console_build();
	serial.puts( me->itsName );
	serial.puts( ":\tAction\n" );
};

/******************************************************************************
	Local variables
******************************************************************************/

//static state_t oldSys_workState;
//static state_t oldSys_errorState;
//static state_t oldSys_onState;
//static state_t oldSys_offState;

static const testData_t testSimple[] = 
{
	/* Input	State					Level */
	/*"?",		&oldSys_workState,		0	*/
	{ "1",		&oldSys_errorState,		0	},
	{ "0",		&oldSys_errorState,		0	},
	{ "1",		&oldSys_workState,		0	},
	{ "0",		&oldSys_onState,		1	},
	
	{ "0",		&oldSys_onState,		1	},
	{ "1",		&oldSys_offState,		1	},
	{ "0",		&oldSys_offState,		1	},
	{ "1",		&oldSys_onState,		1	},
};

void oldSys_unit_test( void )
{
	int isTestFailed = 0;//Successful
	
	console_t serial = console_build();
	serial.puts( "TEST 1 STARTED for oldSys\n" );
	oldSys_t sysTest1 = oldSys_build();
	for( uint32_t i = 0; i<(sizeof(testSimple)/sizeof(testSimple[0])); i++ )
	{
		oldSys_handleEvent( &sysTest1, &(hsm_event_t){ 0, testSimple[i].input } );
		
		if( sysTest1.itsCurrentState != testSimple[i].expectedState )
		{
			isTestFailed = 1;
		}
	}
	if( isTestFailed == 1 )
	{
		serial.puts( "\n\nTEST 1 FAILED!!!!\n\n" );
	}
	else
	{
		serial.puts( "\n\nTEST 1 PASSED\n\n" );
	}
	
	oldSys_reset( &sysTest1 );
}

state_t oldSys_workState = 
{
	.itsName = "WORK ",
	.itsInitialState = &oldSys_onState,
	.itsParentState = NULL,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 1,
	.itsTransition = (transition_t[]){
		{ &guard, &action, &oldSys_errorState }
	},
	.itsMode = false,
	.itsMode = ENTRY,
	.itsHistoryState = &oldSys_onState,
};

state_t oldSys_errorState = 
{
	.itsName = "ERROR",
	.itsInitialState = NULL,
	.itsParentState = NULL,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 1,
	.itsTransition = (transition_t[]){
		{ &guard, &action, &oldSys_workState }
	},
	.itsMode = false,
	.itsMode = ENTRY,
	.itsHistoryState = NULL,
};

state_t oldSys_onState = 
{
	.itsName = "ON   ",
	.itsInitialState = NULL,
	.itsParentState = &oldSys_workState,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 1,
	.itsTransition = (transition_t[]){
		{ &guard, &action, &oldSys_offState }
	},
	.itsMode = false,
	.itsMode = ENTRY,
	.itsHistoryState = NULL,
};

state_t oldSys_offState = 
{
	.itsName = "OFF  ",
	.itsInitialState = NULL,
	.itsParentState = &oldSys_workState,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 1,
	.itsTransition = (transition_t[]){
		{ &guard, &action, &oldSys_onState }
	},
	.itsMode = false,
	.itsMode = ENTRY,
	.itsHistoryState = NULL,
};

/******************************************************************************
	Local function declarations
******************************************************************************/

static void oldSys_init( hsm_t* obj, state_t* initialState )
{
	obj->itsInitialState = initialState;
	obj->itsCurrentState = initialState;
	obj->itsLevel = 0;
	obj->itsMode = HSMM_ON_ENTRY;
}

/******************************************************************************
	Function definitions
******************************************************************************/

oldSys_t oldSys_build( void )
{
	/* Build it */
	oldSys_t aux;
	oldSys_init( (hsm_t*)&aux, &oldSys_workState );
	
	/* Reset */
	oldSys_reset( &aux );
	
	/* Get handle */
	return aux;
}

void oldSys_reset( oldSys_t* me )
{
	state_t* initialState = me->itsInitialState;
	oldSys_init( (hsm_t*)&me, initialState );
	
	/* Initialize states */
	hsm_state_reset( &oldSys_workState );
	hsm_state_reset( &oldSys_errorState );
	hsm_state_reset( &oldSys_onState );
	hsm_state_reset( &oldSys_offState );
}

void oldSys_handleEvent( oldSys_t* me, hsm_event_t* event )
{
	hsm_handleEvent( (hsm_t*)me, (hsm_event_t*)event);
}
