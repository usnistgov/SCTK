/*
 * asclite
 * Author: Jerome Ajot, Nicolas Radde, Chris Laprun
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
 
/** 
 * Gestion of the coordinates in the LCM
 */
 
#ifndef GRAPH_COORDINATE_H
#define GRAPH_COORDINATE_H

#include "stdinc.h"

typedef list<size_t*>::iterator GraphCoordinateListPosition;

class GraphCoordinateList
{
	private:
		list<size_t*> m_ListCoordinates;
		size_t m_CoordinateDimension;
	
	public:
		GraphCoordinateList(size_t _CoordinateDimension) { m_CoordinateDimension = _CoordinateDimension; }
		~GraphCoordinateList() { RemoveAll(); }
	
		void AddFront(size_t* coordinate);
		void AddBack(size_t* coordinate);
		GraphCoordinateListPosition GetBeginPosition() { return m_ListCoordinates.begin(); }
		GraphCoordinateListPosition GetEndPosition() { return m_ListCoordinates.end(); }
		size_t* GetAt(GraphCoordinateListPosition GCPosition);
		void RemoveAll();
		void NextPosition(GraphCoordinateListPosition& GCPosition) { ++GCPosition; }
		bool isEmpty() { return m_ListCoordinates.empty(); }
		size_t GetSize() {return m_ListCoordinates.size(); }
};

#endif
