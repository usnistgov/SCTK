/*
 * asclite
 * Author: Jerome Ajot
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
 
#ifndef UEMFILTER_H
#define UEMFILTER_H

#include "stdinc.h"
#include "checker.h"
#include "ctmstmrttm_segmentor.h"
#include "logger.h"

class UEMElement
{		
    public:
        UEMElement() {}
		UEMElement(string _file, string _channel, int _startTime, int _endTime);
        ~UEMElement() {}
		
		string GetFile() { return m_File; }
		string GetChannel() { return m_Channel; }
		int    GetStartTime() { return m_StartTime; }
		int    GetEndTime() { return m_EndTime; }
			
    private:
		string m_File;
		string m_Channel;
		int    m_StartTime;
		int    m_EndTime;
		
        static Logger* m_pLogger;
};

class UEMFilter : public Checker
{
	public:
		// class constructor
		UEMFilter() { m_bUseFile = false; }
		// class destructor
		~UEMFilter();
		
		void        AddUEMElement(UEMElement* _pUEMElement) { m_VectUEMElements.push_back(_pUEMElement); }
		size_t      GetNumberElement() { m_VectUEMElements.size(); }
		UEMElement* FindElement(string file, string channel);
		
		bool isEmpty() { return m_VectUEMElements.empty(); }
		void LoadFile(string filename);
		bool HasInterSegmentGaps(Speech* speech);
		bool isProcessSingleSpeech() { return m_bUseFile; }
		
		unsigned long int ProcessSingleSpeech(Speech* speech);
		unsigned long int ProcessSpeechSet(SpeechSet* pSpeechSet);
	
	private:
		vector<UEMElement*> m_VectUEMElements;
		bool m_bUseFile;
		
		int ParseString(string chaine);
		
        static Logger* m_pLogger;
};

#endif
