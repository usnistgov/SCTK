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

#include "alignedsegmentiterator.h"

AlignedSegmentIterator::AlignedSegmentIterator(AlignedSpeech* alignedSpeech)
{
	m_current = alignedSpeech->m_segments.begin();
	m_end = alignedSpeech->m_segments.end();
}

bool AlignedSegmentIterator::Current(AlignedSegment** current)
{
	if(m_current != m_end)
	{
		*current = m_current->second;
		++m_current;
		return true;
	}
	
	return false;
}