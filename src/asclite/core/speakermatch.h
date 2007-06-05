/*
 * ASCLITE
 * Author: Ajot Jerome, Jon Fiscus, Radde Nicolas, Laprun Chris
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
 * Database for matching speaker optimization
 */
 
#ifndef SPEAKERMATCH_H
#define SPEAKERMATCH_H

#include "stdinc.h"
#include "logger.h"

class SpeakerMatch
{
    public:
        /** Constructor */
        SpeakerMatch();
        
        /** Destructor */
        ~SpeakerMatch();
		
		/** Load the file */
		void LoadFile(string filename);
		
		/** Initialize a couple, sys-ref */
		void SetSysRef(string source, string channel, string sys, string ref);
		
		/** Return the ref coupled to the sys */
		string GetRef(string source, string channel, string sys);
        
    protected:
           
    private:
		map < string, string > * m_pMapSourceChannelSysRef;  
		
        static Logger* logger;
};

#endif
