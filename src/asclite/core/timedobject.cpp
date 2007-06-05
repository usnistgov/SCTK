/*
 * ASCLITE
 * Author: Jerome Ajot, Jon Fiscus, Nicolas Radde, Chris Laprun
 *
 * This software was developed at the National Institute of Standards and Technology by 
 * employees of the Federal Government in the course of their official duties. Pursuant
 * to title 17 Section 105 of the United States Code this software is not subject to
 * copyright protection and is in the public domain. ASCLITE is an experimental system.
 * NIST assumes no responsibility whatsoever for its use by other parties, and makes no
 * guarantees, expressed or implied, about its quality, reliability, or any other
 * characteristic. We would appreciate acknowledgement if the software is used.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS."  With regard to this software, NIST MAKES NO EXPRESS
 * OR IMPLIED WARRANTY AS TO ANY MATTER WHATSOEVER, INCLUDING MERCHANTABILITY,
 * OR FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Represent a timed object.
 * A timed object is represented by a start time and a end time.
 */
 
#include "timedobject.h" // class's header file

bool TimedObject::OverlapWith(TimedObject* other)
{
	if(other == NULL)
    {
		return false;
	}
	
	TimedObject *first, *second;
    
	if(GetStartTime() <= other->GetStartTime()) 
    {
		first = this;
		second = other;
	} 
    else 
    {
		first = other;
		second = this;
	}
    
	return (first->GetEndTime() >= second->GetStartTime());
}

void* TimedObject::InitWithDuration(int _startTime, int _duration) 
{
    startTime = _startTime;
	endTime = _startTime + _duration;
	return this;
}

bool TimedObject::AreStartTimeAndDurationValid(int _startTime, int _duration)
{
	if(_startTime >= 0 && _duration >= 0)
		return true;
        
	return false;
}

void* TimedObject::InitWithEndTime(int _startTime, int _endTime) 
{
    startTime = _startTime;
    endTime = _endTime;
	return this;
}

bool TimedObject::AreStartTimeAndEndTimeValid(int _startTime, int _endTime) 
{
	if(_startTime >= 0 && _endTime >= _startTime)
		return true;
        
	return false;
}

string TimedObject::ToString() 
{
	std::ostringstream oss;
	oss << "[" << startTime << ", " << endTime << "]";
	return oss.str();
}

bool TimedObject::Equals(TimedObject* to) 
{
	if(this == to)
		return true;
	if(to == NULL)
		return false;
        
	return this->startTime == to->startTime && this->endTime == to->endTime;
}
