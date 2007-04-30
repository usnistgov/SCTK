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
 * Load the named file into a speech element.
 */

#include "stm_inputparser.h" // class's header file

const string STMInputParser::IGNORE_TIME_SEGMENT_IN_SCORING = "ignore_time_segment_in_scoring";
Logger* STMInputParser::logger = Logger::getLogger();

/**
 * Load the named file into a Speech element.
 * @todo Finish this :P
 */
SpeechSet* STMInputParser::loadFile(string name)
{
	string line;
	ifstream file;
	long int lineNum = -1;
	long int elemNum = -1;
	map<string, int> nbIgnoreForSpkr;
            
	file.open(name.c_str(), ifstream::in);
    
	if (! file.is_open())
	{ 
		LOG_FATAL(logger, "Error opening STM file " + name); 
		exit (1); 
	}
	
	SpeechSet* vec = new SpeechSet(name);
	map<string, Speech*> res;
	
	while (getline(file,line,'\n'))
	{
		++lineNum;
        //cout << lineNum << endl;
		//cout << "prec:"<< prec_seg->isEmpty() << endl;
        
		if (line.find_first_of(";;") == 0)
		{
			//comment so skip (for now)
		}
		else
		{
			char f_file[BUFFER_SIZE];
			char channel[BUFFER_SIZE];
			char spkr[BUFFER_SIZE];
			char start_char[BUFFER_SIZE];
			char end_char[BUFFER_SIZE];
			int start, end;
			char lur[BUFFER_SIZE];                   
			int nbArgParsed = 0;
			
			nbArgParsed = sscanf(line.c_str(), "%s %s %s %s %s %s", (char*) &f_file, (char*) &channel, (char*) &spkr, (char*) &start_char, (char*) &end_char, (char*) &lur);
			int wordsBegin = line.find(">")+1;
			
			start = ParseString(string(start_char));
			end = ParseString(string(end_char));
			
			//start = (int)(atof(start_char)*1000.0);
			//end = (int)(atof(end_char)*1000.0);
			            
			if (nbArgParsed < 6 || lur[0] != '<')
            {
                nbArgParsed = sscanf(line.c_str(), "%s %s %s %s %s", (char*) &f_file, (char*) &channel, (char*) &spkr, (char*) &start_char, (char*) &end_char);
				
				start = ParseString(string(start_char));
				end = ParseString(string(end_char));
				
                //start = (int)(atof(start_char)*1000.0);
                //end = (int)(atof(end_char)*1000.0);
				
                wordsBegin = line.find(start_char)+1;
                wordsBegin = line.find(end_char, wordsBegin)+string(end_char).size();
                
                if (nbArgParsed < 5)
                {
                    char buffer[BUFFER_SIZE];
                    sprintf(buffer, "Error parsing the line %li in file %s", lineNum, name.c_str());
                    LOG_ERR(logger, buffer);
                }
            }
						
            /*
			cout << "load line: " << line << endl;
			cout << "     file: " << f_file << endl;
			cout << "     chan: " << channel << endl;
			cout << "     spkr: " << spkr << endl;        
			cout << "     beg : " << start << endl;  
			cout << "     end : " << end << endl;
			cout << "     lur : " << lur << endl;
            */
			Speech* speech = res[spkr];
            
			if (!speech)
			{
				res[spkr] = new Speech(vec);
				speech = res[spkr];
			}    
                  
			string s_tokens(line, wordsBegin, line.size());
			string temp_tokens = s_tokens;
			Segment* seg;
			transform(temp_tokens.begin(), temp_tokens.end(), temp_tokens.begin(), (int(*)(int))tolower);
            
			if (temp_tokens.find(IGNORE_TIME_SEGMENT_IN_SCORING) != string::npos)
			{
                seg = Segment::CreateWithEndTime(start, end, speech);
				seg->SetSource(string(f_file));
				seg->SetChannel(string(channel));
				seg->SetSpeakerId(string(spkr));
				seg->SetAsSegmentExcludeFromScoring();
                
                if(nbIgnoreForSpkr.find(spkr) == nbIgnoreForSpkr.end())
                {
                    nbIgnoreForSpkr[spkr]=0;								  
				}
                                
				++(nbIgnoreForSpkr[spkr]);
            }
			else
			{
				seg = ParseWords(string(f_file), string(channel), string(spkr), start, end, speech, s_tokens);
				
				//vector<Token*> temp = seg->ToTopologicalOrderedStruct();
				//            cout << "  " << temp.size();
				//            cout << "[ ";
				//            for (int k=0 ; k < temp.size() ; k++)
				//            {
				//              cout << temp[k]->GetText();
				//            }
				//            cout << " ]" << endl;    
				++elemNum;
			}			
			
			seg->SetSourceLineNum(lineNum);
            seg->SetSourceElementNum(elemNum);
			size_t nbSeg = speech->NbOfSegments();
            
            ostringstream osstr;
			osstr << "(" << spkr << "-";
			osstr << setw(3) << nouppercase << setfill('0') << (nbSeg - nbIgnoreForSpkr[spkr]) << ")";
			seg->SetId(osstr.str());
			
			//if (nbSeg != 0)
			//{
			//	Attach(speech->GetSegment(nbSeg - 1), seg);
			//}
            
			speech->AddSegment(seg);
		}
	}
    	
	map<string, Speech*>::iterator  ssp = res.begin();
	map<string, Speech*>::iterator essp = res.end();
	
	while(ssp != essp)
	{
		string _spkr = ssp->first;
		Speech* _speech = ssp->second;
		size_t nbrseg = _speech->NbOfSegments();
		
		if(nbrseg > 1)
		{			
			Segment* prevSeg = NULL;
			Segment* currSeg = NULL;
			
			for(size_t ii=0; ii<nbrseg; ++ii)
			{
				Segment* segalpha = _speech->GetSegment(ii);
				
				if( (segalpha->GetNumberOfFirstToken() != 0) && (segalpha->GetNumberOfLastToken() != 0) )
					currSeg = segalpha;
				
				if(prevSeg && currSeg)
					Attach(prevSeg, currSeg);
				
				if(currSeg)
				{
					prevSeg = currSeg;
					currSeg = NULL;
				}
			}
		}
		
		++ssp;
	}
	
	file.close();
	LOG_INFO(logger, "loading of file " + name + " done");
	
	map<string, Speech*>::iterator i = res.begin();
	map<string, Speech*>::iterator ei = res.end();
    
	while (i != ei)
	{
		vec->AddSpeech(i->second);
		++i;
	}
    
	return vec;
}

void STMInputParser::Attach(Segment* seg1, Segment* seg2)
{
    for (size_t i=0 ; i < seg1->GetNumberOfLastToken() ; ++i)
    {
        for (size_t j=0 ; j < seg2->GetNumberOfFirstToken() ; ++j)
        {
            seg1->GetLastToken(i)->AddNextToken(seg2->GetFirstToken(j));
            seg2->GetFirstToken(j)->AddPrecToken(seg1->GetLastToken(i));
        }    
    }
}
