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

#ifndef UEM_H
#define UEM_H

#include "stdinc.h"
#include "logger.h"

/**
 * 
 */
class UemElement
{		
    public:
        UemElement() { }
		UemElement(string _file, string _channel, int _startTime, int _endTime);
        ~UemElement() { }
		
		string GetFile() { return m_File; }
		string GetChannel() { return m_Channel; }
		int    GetStartTime() { return m_StartTime; }
		int    GetEndTime() { return m_EndTime; }
			
    private:
		string m_File;
		string m_Channel;
		int    m_StartTime;
		int    m_EndTime;
		
		/**
         * the logger
         */
        static Logger* m_pLogger;
};

class Uem
{
	public:
		Uem() { }
		~Uem();
		
		void        AddUemElement(UemElement* _pUemElement) { m_VectUemElements.push_back(_pUemElement); }
		size_t      GetNumberElement() { m_VectUemElements.size(); }
		UemElement* FindElement(string file, string channel);
		
		bool isEmpty() { return m_VectUemElements.empty(); }
		void LoadUemFile(string filename);
		
	private:
		vector<UemElement*> m_VectUemElements;
		
		int ParseString(string chaine);
		
		/**
         * the logger
         */
        static Logger* m_pLogger;
};

#endif
