/*
 * ASCLITE
 * Author: Jerome Ajot, Jon Fiscus, Nicolas Radde, Chris Laprun
 *
 * This software was developed at the National Institute of Standards and
 * Technology by employees of the Federal Government in the course of
 * their official duties.  Pursuant to Title 17 Section 105 of the United
 * States Code this software is not subject to copyright protection within
 * the United States and is in the public domain. It is an experimental
 * system.  NIST assumes no responsibility whatsoever for its use by any
 * party.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS."  With regard to this software, NIST
 * MAKES NO EXPRESS OR IMPLIED WARRANTY AS TO ANY MATTER WHATSOEVER,
 * INCLUDING MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 */
 
/**
 * Tools package
 */
	
#include "tools.h"

void toLowerCase(std::string &str)
{
	const size_t length = str.length();
	
	for(size_t i=0; i < length; ++i)
	{
		str[i] = std::tolower(str[i]);
	}
}

void toUpperCase(std::string &str)
{
	const size_t length = str.length();
	
	for(size_t i=0; i < length; ++i)
	{
		str[i] = std::toupper(str[i]);
	}
}
