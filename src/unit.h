/******************************************************************************
	About
******************************************************************************/

/**
 * \file unit.h
 *
 * \brief	Unit test framework
 *
 * Created:			17/11/2016
 *
 * \author	Elias Kanelis	hkanelhs@yahoo.gr
 */

/**
* \defgroup	Tests	Tests
*
* \code	#include <unit.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef UNIT_H_ONLY_ONE_INCLUDE_SAFETY
#define UNIT_H_ONLY_ONE_INCLUDE_SAFETY

/******************************************************************************
	Include files
******************************************************************************/

#include <stdbool.h>	/* bool */

/******************************************************************************
	Definitions
******************************************************************************/

#define UNIT_STRINGIFY2( s )			(#s)					/**< Help put quotes in string before compile time */
#define UNIT_STRINGIFY( s )				(UNIT_STRINGIFY2( s ))	/**< Put quotes in string before compile time */

/**
 * \brief Printout for the start of a test.
 *
 * Do not use directly. See \see START_TEST instead.
 *
 * \param[in] name	The name of the test.
 * 
 * \return void
 */
#define start_test( name )			unit_test_start( UNIT_STRINGIFY(name) )

/**
 * \brief Starts the declaration of a test.
 * 
 * Should end with \see END_TEST.
 * 
 * \param[in]	name	The name of the test.
 *
 * \return void
 */
#define START_TEST( name )			static inline void (name)( void ) { start_test( name );

/**
 * Ends the declaration of a test created with \see START_TEST.
 */
#define END_TEST					unit_test_end(); };

/**
 * \brief Printout for the start of a subtest.
 *
 * Do not use directly. See \see START_SUBTEST instead.
 *
 * \param[in] name	The name of the subtest.
 * 
 * \return void
 */
#define start_subtest( name )		unit_subtest_start( UNIT_STRINGIFY(name) )

/**
 * \brief Starts the declaration of a subtest.
 * 
 * Should end with \see END_SUBTEST.
 * 
 * \param[in]	name	The name of the subtest.
 *
 * \return void
 */
#define START_SUBTEST( name )		static inline void (name)( void ) { start_subtest( name );
	
/**
 * Ends the declaration of a subtest created with \see START_SUBTEST.
 */
#define END_SUBTEST					unit_subtest_end(); };

/**
 * \brief Attaches a subtest inside a test declaration.
 * 
 * \param[in]	name	The name of the subtest.
 */
#define ATTACH_SUBTEST( name )		(name)()

/**
 * \brief Runs a test.
 * 
 * \param[in]	name	The name of the test to run.
 */
#define RUN_TEST( name )			(name)()

/**
 * \brief Checks the expression and reports the results of the test and some message.
 * 
 * \param[in]	expression	The expression under test.
 * \param[in]	msg			The message to be shown.
 *
 * \return void
 */
#define CHECK( expression, msg )	unit_check( (expression), ( UNIT_STRINGIFY(__LINE__) ), (msg) )

/******************************************************************************
	Function declarations
******************************************************************************/

void unit_test_start( const char* name );
void unit_test_end( void );
void unit_subtest_start( const char* name );
void unit_subtest_end( void );
void unit_check( bool expression, const char* line, const char* msg );

#endif /* UNIT_H_ONLY_ONE_INCLUDE_SAFETY */
