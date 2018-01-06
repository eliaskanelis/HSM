#include "CppUTest/CommandLineTestRunner.h"

int main( int ac, char** av )
{
    return CommandLineTestRunner::RunAllTests( ac, av );
}


/*
 * Tests name convention:
 * 
 * Should_ExpectedBehavior_When_StateUnderTest
 */