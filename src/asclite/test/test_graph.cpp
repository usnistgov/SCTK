/*
 * asclite
 * Author: Ajot Jerome, Radde Nicolas, Laprun Chris
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
 
/** @file test_graph.cpp
  * @author Jerome AJOT <jerome.ajot@nist.gov>
	*/

#include "test_graph.h"

/** Constructor */
TestGraph::TestGraph()
{
}

/** Destructor */
TestGraph::~TestGraph()
{
}

/** 
 * Testing function 
 * return true if there is no errors
 * return false if there are and the descriptions in strerr
 */
bool TestGraph::TestAll()
{
	char* unit; 	
	float size = MemoryUsage();
	if( size < 1024 )
		unit = "KB";
	else
	{
		size /= 1024;
		unit = "MB";
	}	
	printf("Graph used %.1f%s of RAM.\n", size, unit);
	return true;
}
