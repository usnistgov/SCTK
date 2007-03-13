#ifndef TEST_RTTM_INPUTPARSER_H
#define TEST_RTTM_INPUTPARSER_H

#include "stdinc.h"
#include "segment.h"
#include "speech.h"
#include "token.h"
#include "rttm_inputparser.h"

/**
 * Test the CTM input parser
 */
class RTTMInputParserTest
{
	public:
		// class constructor
		RTTMInputParserTest();
		// class destructor
		~RTTMInputParserTest();
		/**
		 * Launch all test
		 */
		void testAll();
		/**
		 * Test the import of the Basic file
		 */
		void testBasicImport();
		void testAlternations();
};

#endif // TEST_RTTM_INPUTPARSER_H
