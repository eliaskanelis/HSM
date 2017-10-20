/******************************************************************************
	Code
******************************************************************************/

/******************************************************************************
	Include files
******************************************************************************/

#include "newSys.h"

#include "hsm.h"
#include "console.h"

#include <stddef.h>		/* NULL */
#include <stdbool.h>	/* bool */
#include "string.h"		/* strcmp */

/******************************************************************************
	Local definitions
******************************************************************************/

typedef struct
{
	char* const input;
	state_t* expectedState;
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

//static state_t newSys_superState;
//static state_t newSys_onState;
//static state_t newSys_offState;
//static state_t newSys_outState;

static const testData_t testSimple[] = 
{
	/* Input	State */
	/*"?",		&newSys_superState, */
	{ "1",		&newSys_offState, },
	{ "1",		&newSys_outState, },
	{ "1",		&newSys_onState, },
	{ "1",		&newSys_offState, },
	{ "1",		&newSys_outState, },
	
	{ "0",		&newSys_outState, },
	{ "1",		&newSys_onState, },
};

void newSys_unit_test( void )
{
	int isTestFailed = 0;//Successful
	
	console_t serial = console_build();
	serial.puts( "TEST 1 STARTED for newSys\n" );
	newSys_t sysTest1 = newSys_build();
	for( uint32_t i = 0; i<(sizeof(testSimple)/sizeof(testSimple[0])); i++ )
	{
		newSys_handleEvent( &sysTest1, &(hsm_event_t){ 0, testSimple[i].input } );
		
		if( sysTest1.base.itsCurrentState != testSimple[i].expectedState )
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
	
	newSys_reset( &sysTest1 );
}

state_t newSys_superState = 
{
	.itsName = "SUPER",
	.itsInitialState = &newSys_onState,
	.itsParentState = NULL,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 0,
	.itsTransition = NULL,
	
	.itsHistoryState = &newSys_onState,
};

state_t newSys_onState = 
{
	.itsName = "ON   ",
	.itsInitialState = NULL,
	.itsParentState = &newSys_superState,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 1,
	.itsTransition = (transition_t[]){
		{ &guard, &action, &newSys_offState }
	},
	
	.itsHistoryState = NULL,
};

state_t newSys_offState = 
{
	.itsName = "OFF  ",
	.itsInitialState = NULL,
	.itsParentState = &newSys_superState,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 1,
	.itsTransition = (transition_t[]){
		{ &guard, &action, &newSys_outState }
	},
	
	.itsHistoryState = NULL,
};

state_t newSys_outState = 
{
	.itsName = "OUT  ",
	.itsInitialState = NULL,
	.itsParentState = NULL,
	.onEntry = &onEntry,
	.during = &during,
	.onExit = &onExit,
	.itsTransitionNum = 1,
	.itsTransition = (transition_t[]){
		{ &guard, &action, &newSys_onState }
	},
	
	.itsHistoryState = NULL,
};



/******************************************************************************
	Local function declarations
******************************************************************************/

static void newSys_init( hsm_t* obj, state_t* initialState )
{
	obj->itsInitialState = initialState;
	obj->itsCurrentState = initialState;
	obj->itsMode = HSM_ON_ENTRY;
}

/******************************************************************************
	Function definitions
******************************************************************************/

newSys_t newSys_build( void )
{
	/* Build it */
	newSys_t aux;
	newSys_init( (hsm_t*)&aux, &newSys_superState );
	
	/* Reset */
	newSys_reset( &aux );
	
	/* Get handle */
	return aux;
}

void newSys_reset( newSys_t* me )
{
	state_t* initialState = me->base.itsInitialState;
	newSys_init( (hsm_t*)&me, initialState );
	
	/* Initialize states */
	hsm_state_reset( &newSys_superState );
	hsm_state_reset( &newSys_onState );
	hsm_state_reset( &newSys_offState );
	hsm_state_reset( &newSys_outState );
}

void newSys_handleEvent( newSys_t* me, hsm_event_t* event )
{
	hsm_handleEvent( (hsm_t*)me, (hsm_event_t*)event);
}
