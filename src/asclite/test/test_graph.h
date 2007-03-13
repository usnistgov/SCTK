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
 
/** @file test_graph.h
  * @author Jerome AJOT <jerome.ajot@nist.gov>
	*/
	
#ifndef TEST_GRAPH_H
#define TEST_GRAPH_H

#include "stdinc.h"

#include "graph.h"
#include "token.h"
#include "graphalignedtoken.h"
#include "asctools.h"

class TestGraph
{
	public:
		/** Pointer to a Graph */
		Graph* m_pGraph;
		
		/** Constructor */
		TestGraph();
		/** Destructor */
		~TestGraph();
	
		/** Testing function */
		bool TestAll();
};

#endif
