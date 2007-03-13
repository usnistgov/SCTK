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

#ifndef SPEECHSET_H
#define SPEECHSET_H

#include "stdinc.h"
#include "speech.h"
#include "logger.h"

/**
 * A speech set represent all the data from one source (reference, participant)
 * it's a collection of Speech(Speaker) from one source.
 */
class SpeechSet
{
	public:
		// class constructors
		SpeechSet(string sourceFileName = "DEFAULT_FILE_NAME");
		// class destructor
		~SpeechSet();
		/**
		 * Return the nb of Speech contain in this set
		 */
		size_t GetNumberOfSpeech() { return speeches.size(); }
		/**
		 * Return the speech number i in the set
		 */
		Speech* GetSpeech(size_t index) {  return speeches[index]; }
		/**
		 * Add a speech into the set
		 */
		void AddSpeech(Speech* speech) { speeches.push_back(speech); }
		/**
		 * Return true id the set contain only references
		 */
		bool IsRef() { return ref; }
		/**
		 * Return true id the set contain only hypothesis
		 */
		bool IsHyp() { return hyp; }
		/**
		 * Set the hyp/ref status of this set
		 */
		void SetOrigin(string status);
		/** Determines if case is taken into account to align Tokens part of this Speech. */
		bool PerformCaseSensitiveAlignment();
		/** Determines if fragments are considered as correct when aligning Tokens part of this Speech. */
		bool AreFragmentsCorrect();
		/** Determines if optionally deletable Tokens need to be accounted for. */
		bool UseOptionallyDeletable();
		
		/** Retrieves the name of the file from which this SpeechSet originated. */
		string GetSourceFileName() { return fileName; }
	private:
        /**
         * The internal speech collection
         */
        vector<Speech*> speeches;
        /**
         * Store if the set is a reference set
         */
        bool ref;
        /**
         * Store if the set is a hypothesis set
         */
        bool hyp;
        /**
         * Reference to the logger
         */
        static Logger* logger;
		
		/** The name of the file from which this SpeechSet originated. */
		string fileName;
		
		/** Caches the value of the "align.case_sensitive" property. */
		bool case_sensitive;
		/** Caches the value of the "align.fragment_are_correct" property. */
		bool fragments_are_correct;
		/** Caches the values of the "align.optionally" property. */
		bool optionally_deletable;		
		/** Updates the cached properties if needed. */
		void UpdatePropertiesIfNeeded(bool force);
};

#endif // SPEECHSET_H
