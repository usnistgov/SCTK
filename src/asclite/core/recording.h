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
 
#ifndef RECORDING_H
#define RECORDING_H

#include "stdinc.h"
#include "properties.h"
#include "speech.h"
#include "speechset.h"
#include "aligner.h"
#include "filter.h"
#include "levenshtein.h"
#include "inputparser.h"
#include "ctm_inputparser.h"
#include "stm_inputparser.h"
#include "trn_inputparser.h"
#include "rttm_inputparser.h"
#include "reportgenerator.h"
#include "rawsys_reportgenerator.h"
#include "sgml_reportgenerator.h"
#include "scorer.h"
#include "stt_scorer.h"
#include "ctmstmrttm_segmentor.h"
#include "trntrn_segmentor.h"
#include "spkrautooverlap.h"
#include "glmfilter.h"
#include "uemfilter.h"
#include "speakermatch.h"
#include "logger.h"

/**
 * A recording contain all the data needed to score a testset.
 * From the argument to the list of parser (input) and generator (output report).
 * This is the main entry to the aligner.
 */
class Recording
{
	public:
        // class constructor
        Recording();
        // class destructor
        ~Recording();

        /**
         * State constants
         */
        static const int INITIALIZED = 0;
        static const int LOADED = 1;
        static const int ALIGNED = 2;
        static const int SCORED = 3;		
        /**
         * Load the reference&Hypothesis files into the system.
         * use the right loader based on the type.
         */
        void Load(string refFile, string refType, vector<string> hypFile, vector<string> _hypothesis_titles, string hypType, string glmFile, string uemFile, string speakeralignfile);
        /**
         * Filter the references and hypothesis with the availables filters.
         */
        void Filter(vector<string> _filters);
        /**
         * Align the ref to the hyp with the select align algo
         */
        void Align();
        /**
         * Score the Alignement with the selected scoring system
         */
        void Score();
        /**
         * Generate the reports based on the scored alignment.
         */
        void GenerateReport(vector<string> reportType);
		
		void AddInterSegmentGapsToRefs();
        
	private:
        /**
         * contain all the available input parser
         */
        map<string, InputParser*> inputParsers;
		
        /**
         * contain all the available report generator
         */
        map<string, ReportGenerator*> reportGenerators;
		
        /**
         * contain all the available Aligner
         */
        map<string, Aligner*> aligner;
		
        /**
         * contain all the available Scorer
         */
        map<string, Scorer*> scorer;
		
        /**
         * contain all the available Segmentors
         */
        map<string, Segmentor*> segmentors;
		
        /**
		 * contain all the available Filters
         */
        map<string, Filter::Filter*> filters;
        
		/**
		 * Database for the optimization speaker alignment 
		 */
		SpeakerMatch* m_pSpeakerMatch;
		
		/** the logger */
        static Logger* logger;
	  
        //-------------------------------------
        // State dependents attribute
        //-------------------------------------
        /**
         * State of the Recording object
         * The different state can be 
         * - INITIALIZED
         * - LOADED
         * - ALIGNED
         * - SCORED
         * @todo Curently not implemented and use in the code
         */
        int state;
        /**
         * The current segmentor
         */
        Segmentor* segmentor;
		/**
		 * A list of all the references of the Recording.
		 */
		SpeechSet* references;
		/**
		 * A list of all the hypothesis of the Recording.
		 * More than one group of hypothesis can be done at once.
		 */
		map<string, SpeechSet*> hypothesis;
		/**
		 * Alignment object
		 */
		Alignment* alignments;
};

#endif // RECORDING_H
