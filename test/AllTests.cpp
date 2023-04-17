#include "CppUTest/CommandLineTestRunner.h"

int main( int argc, char **argv )
{
	return CommandLineTestRunner::RunAllTests( argc, argv );
}


/*
 * Tests name convention:
 *
 * Should_ExpectedBehavior_When_StateUnderTest
 */
