/*
 * asclite
 * Author: Jerome Ajot, Nicolas Radde, Chris Laprun, Jonathan Fiscus
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
 
#ifndef GLMFILTER_H
#define GLMFILTER_H

#include "checker.h"
#include "logger.h"

/**
 * Check and Validate the overlaping of a speaker with himself
 */
class GLMFilter : public Checker
{
	public:
		// class constructor
		GLMFilter() {}
		// class destructor
		~GLMFilter() {}
		
		void LoadFile(string filename);
		
		bool isProcessSingleAllSpeechSet() { return false; }
		/**
		 * Launch the check process on the specified speech.
		 */
		unsigned long int ProcessSingleSpeech(Speech* speech);
		unsigned long int ProcessSpeechSet(SpeechSet* ref, map<string, SpeechSet*> &hyp) { return 0; }
	
	private:
        static Logger* logger;
};

#endif
