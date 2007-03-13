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

#ifndef STATISTICS_H
#define STATISTICS_H

#include "stdinc.h"

class Statistics
{
	private:
		vector<double> m_VecValues;
		double m_Mean;
		double m_SD;
		double m_Median;
		double m_Sum;
		int m_MaxSize;
		void Compute();
	
	public:
		Statistics(vector<int> _vecValues);
		Statistics(vector<double> _vecValues);
		~Statistics() { m_VecValues.clear(); }
	
		double GetMean(bool safe = false) { return(((m_Mean == 0.0) && safe) ? 1.0 : m_Mean); }
		double GetSD(bool safe = false) { return(((m_SD == 0.0) && safe) ? 1.0 : m_SD); }
		double GetMedian(bool safe = false) { return(((m_Median == 0.0) && safe) ? 1.0 : m_Median); }
		double GetSum(bool safe = false) { return(((m_Sum == 0.0) && safe) ? 1.0 : m_Sum); }
		int GetSize(bool safe = false) { return(((m_VecValues.size() == 0) && safe) ? 1 : m_VecValues.size()); }
		int GetMaxSizeString() { return m_MaxSize; }
};

#endif
