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
 * Check and Validate the overlaping of a speaker with himself
 */
 
#include "glmfilter.h" // class's header file

Logger* GLMFilter::logger = Logger::getLogger();

/**
 * Launch the check process on the specified speech.
 */
unsigned long int GLMFilter::Process(Speech* speech)
{

#ifdef STUB
    Segment* segPrec = NULL;
    Segment* segCurr = NULL;
    string channelPrec;
    string sourcePrec;
    unsigned long int nbErr = 0;

    cout << "GLMFilter " << speech->ToString() << endl;

    for (size_t i=0 ; i < speech->NbOfSegments() ; ++i)
    {
        segCurr = speech->GetSegment(i);

        // Loop over words 
		vector<Token*> tokens = segCurr->ToTopologicalOrderedStruct();
		Token* token;
	
		for(size_t i = 0; i < tokens.size(); ++i) 
		{
			token = tokens[i];
        
			if(token != NULL && token->GetText().compare("BECAUSE") == 0)
			{
				cout << "Found BECAUSE" << endl;
				int half = token->GetDuration() / 2;
				Token *t1 = Token::CreateWithDuration(token->GetStartTime(),half, segCurr, "P1");				
				Token *t2 = Token::CreateWithDuration(token->GetStartTime()+half,token->GetDuration()-half, segCurr, "P2");
				t1->LinkTokens(t2);
				// Link to the prec
				for (size_t x=0; x<token->GetNbOfPrecTokens(); x++)
					token->GetPrecToken(x)->LinkTokens(t1);
				for (size_t x=0; x<token->GetNbOfNextTokens(); x++)
					t2->LinkTokens(token->GetNextToken(x));
				cout << "T1 " << t1->ToString() << endl;
				cout << "T2 " << t2->ToString() << endl;
				cout << "Token " << token->ToString() << endl;
				cout << segCurr->ToString() << endl;
			}
		}		
    }
    
    return nbErr;
#else
	return 0;
#endif
}

void GLMFilter::LoadFile(string filename)
{
}
