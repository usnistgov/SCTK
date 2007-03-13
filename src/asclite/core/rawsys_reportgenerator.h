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

#ifndef RAWSYS_REPORTGENERATOR_H
#define RAWSYS_REPORTGENERATOR_H

#include "reportgenerator.h" // inheriting class's header file
#include "alignment.h"
#include "statistics.h"
#include "logger.h"

class RAWSYS_Datas
{
	private:
		double m_SumConfidenceCorrect;
		double m_SumConfidenceIncorrect;
		uint m_NumberSegments;
		uint m_NumberRefWords;
		uint m_NumberCorrectWords;
		uint m_NumberSubstitutions;
		uint m_NumberSpeakerErrors;
		uint m_NumberDeletions;
		uint m_NumberInsertions;
		uint m_NumberSegmentsErrors;
		
	public:
		RAWSYS_Datas();
		~RAWSYS_Datas() {}
	
		double GetNCE();
		uint GetNumberSegments() { return(m_NumberSegments); }
		uint GetNumberRefWords() { return(m_NumberRefWords); }
		uint GetNumberCorrectWords() { return(m_NumberCorrectWords); }
		uint GetNumberSubstitutions() { return(m_NumberSubstitutions); }
		uint GetNumberSpeakerErrors() { return(m_NumberSpeakerErrors); }
		uint GetNumberDeletions() { return(m_NumberDeletions); }
		uint GetNumberInsertions() { return(m_NumberInsertions); }
		uint GetNumberErrors() { return(m_NumberSubstitutions+m_NumberInsertions+m_NumberDeletions+m_NumberSpeakerErrors); }
		uint GetNumberSegmentsErrors() { return(m_NumberSegmentsErrors); }
	
		double GetPercentCorrectWords() { return( 100.0*((double)m_NumberCorrectWords)/((double)m_NumberRefWords) ); }
		double GetPercentSubstitutions() { return( 100.0*((double)m_NumberSubstitutions)/((double)m_NumberRefWords) ); }
		double GetPercentSpeakerErrors() { return( 100.0*((double)m_NumberSpeakerErrors)/((double)m_NumberRefWords) ); }
		double GetPercentDeletions() { return( 100.0*((double)m_NumberDeletions)/((double)m_NumberRefWords) ); }
		double GetPercentInsertions() { return( 100.0*((double)m_NumberInsertions)/((double)m_NumberRefWords) ); }
		double GetPercentErrors() { return( 100.0*((double)GetNumberErrors())/((double)m_NumberRefWords) ); }
		double GetPercentSegmentsErrors() { return( 100.0*((double)m_NumberSegmentsErrors)/((double)m_NumberSegments) ); }
	
		double AddCorrectWord(double confidence);
		void AddCorrectWord();
		double AddSubstitutionWord(double confidence);
		double AddSpeakerErrorWord(double confidence);
		void AddDeletionWord();
		double AddInsertionWord(double confidence);
		void AddSegment(bool withinError);
};

/**
 * Generate a Raw or Sys like report based on
 * how sclite does it.
 */
class RAWSYSReportGenerator : public ReportGenerator
{
	private:
		/** Raw = 1
		  * Sys = 2
		  */
		int m_RawSys;
		/** Structure which store the needed infos sorted by name (usedID) */
		map<string, RAWSYS_Datas*> m_MapDatas;
	
		double m_SumTotConfidenceCorrect;
		double m_SumTotConfidenceIncorrect;
	
		/** Add spaces to the output */
		void AddChar(int numspc, string str, ostream &outpt);
		/** Adding a string, double, and integer to the display */
		void AddStringText(ostream &outpt, string value, string justify, uint totalspace, string addstr = "");
		void AddStringText(ostream &outpt, double value, int floating, string justify, uint totalspace, string addstr = "");
		void AddStringText(ostream &outpt, int value, string justify, uint totalspace, string addstr = "");
	
		void AddSeparator(ostream &outpt, string str, uint fullsize);
		
		static string FILE_SEPARATOR;
		static Logger* logger;
	
	public:
		/** class constructor with the type */
		RAWSYSReportGenerator(int _RawSys);
		/** class destructor */
		virtual ~RAWSYSReportGenerator();
		/** Generate the SYSRAW report */
        void Generate(Alignment* alignment, int where);
		/** Generate the SYSRAW report by system */
        void GenerateSystem(Alignment* alignment, string systm, ostream &output);
		/** Return the number of different speakers */
		size_t GetNumSpeakers() { return m_MapDatas.size(); }
	
		double GetTotalNCE(double numcorrects, double numinsertions, double numsubstitutions, double numspeakererrors);
};

#endif // RAWSYS_REPORTGENERATOR_H
