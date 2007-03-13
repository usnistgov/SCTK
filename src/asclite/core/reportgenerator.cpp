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
 * Abstract class to generate report.
 */
 
#include "reportgenerator.h" // class's header file

string ReportGenerator::GetFileNameFromPath(string path)
{
	string pass1(path, path.rfind("\\")+1, path.length()-path.rfind("\\")-1);
	return string(pass1, pass1.rfind("/")+1, pass1.length()-pass1.rfind("/")-1);
}
