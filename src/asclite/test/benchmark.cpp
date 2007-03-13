/*
 * asclite
 * Author: Ajot Jerome, Radde Nicolas
 *
 * This software was developed at the National Institute of Standards and Technology by
 * employees of the Federal Government in the course of their official duties.  Pursuant to
 * Title 17 Section 105 of the United States Code this software is not subject to copyright
 * protection within the United States and is in the public domain. asclite is
 * an experimental system.  NIST assumes no responsibility whatsoever for its use by any party.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS."  With regard to this software, NIST MAKES NO EXPRESS
 * OR IMPLIED WARRANTY AS TO ANY MATTER WHATSOEVER, INCLUDING MERCHANTABILITY,
 * OR FITNESS FOR A PARTICULAR PURPOSE.
 */
 
#include "benchmark.h" // class's header file

// class constructor
Benchmark::Benchmark()
{
	// insert your code here
}

// class destructor
Benchmark::~Benchmark()
{
	// insert your code here
}

/**
 * Access the tests no index
 */
SegmentsGroup* Benchmark::GetTest(int index)
{
  return tests[index];
}

/**
 * Access the tests no index
 */
map<string, string> Benchmark::GetProperties(int index)
{
  return properties[index];
}

/**
 * Access the results no index
 */
GraphAlignedSegment* Benchmark::GetResult(int index)
{
  return results[index];
}

/**
 * Get the number of tests
 */
int Benchmark::GetTestSize()
{
  return tests.size();  
}
