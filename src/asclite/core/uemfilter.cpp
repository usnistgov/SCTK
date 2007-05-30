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

#include "uemfilter.h"

Logger* UEMElement::m_pLogger = Logger::getLogger();
Logger* UEMFilter::m_pLogger = Logger::getLogger();

UEMElement::UEMElement(string _file, string _channel, int _startTime, int _endTime)
{
	m_File = _file;
	m_Channel = _channel;
	m_StartTime = _startTime;
	m_EndTime = _endTime;
}

UEMFilter::~UEMFilter()
{
	for(size_t i=0; i<m_VectUEMElements.size(); ++i)
		if(m_VectUEMElements[i])
			delete m_VectUEMElements[i];
	
	m_VectUEMElements.clear();
}

void UEMFilter::FindElement(string file, string channel, list<UEMElement*>* pList)
{
	pList->clear();

	for(size_t i=0; i<m_VectUEMElements.size(); ++i)
		if(m_VectUEMElements[i])
			if( (file.find(m_VectUEMElements[i]->GetFile(), 0) != 0) && (channel.compare(m_VectUEMElements[i]->GetChannel()) == 0) )
				pList->push_back(m_VectUEMElements[i]);
}

int UEMFilter::ParseString(string chaine)
{
	if(strchr(chaine.c_str(),'.') == NULL)
	{
		return(atoi(chaine.c_str()) * 1000);
	}
	else
	{
		int bf, af;
		char c2[BUFFER_SIZE];
		sscanf(chaine.c_str(), "%d.%s", (int*) &bf, (char*) &c2);
		
		int len = strlen(c2);
		
		af = atoi(c2);
				
		if(len > 3)
		{
			af = (int)((double)(af)/pow(10, len-3));
		}
		else
		{
			af = (int)((double)(af)*pow(10, 3-len));
		}
		
		return( bf*1000+af);		
	}
}

void UEMFilter::LoadFile(string filename)
{
	ifstream file;
	string line;
	file.open(filename.c_str(), ifstream::in);
	
	long int lineNum = -1;
    
	if (! file.is_open())
	{ 
		LOG_FATAL(m_pLogger, "Error opening file " + filename); 
		exit (1); 
	}
	
	char l_file[BUFFER_SIZE];
	char l_channel[BUFFER_SIZE];
	char l_start[BUFFER_SIZE];
	char l_end[BUFFER_SIZE];
	
	while (getline(file,line,'\n'))
	{
		 ++lineNum;
	
		if (line.find_first_of(";;") == 0)
		{
			//comment so skip (for now)
		}
		else
		{
			int nbArgParsed = 0;
			nbArgParsed = sscanf(line.c_str(), "%s %s %s %s", (char*) &l_file, (char*) &l_channel, (char*) &l_start, (char*) &l_end);
			 
			if(nbArgParsed != 4)
			{
				char buffer[BUFFER_SIZE];
				sprintf(buffer, "Error parsing the line %li in file %s", lineNum, filename.c_str());
				LOG_ERR(m_pLogger, buffer);
			}
			else
			{
				int start_ms = ParseString(string(l_start));
				int end_ms = ParseString(string(l_end));
				
				if(start_ms < end_ms)
				{
					UEMElement* pUEMElement = new UEMElement(string(l_file), string(l_channel), start_ms, end_ms);
					AddUEMElement(pUEMElement);
				}
				else
				{
					char buffer[BUFFER_SIZE];
					sprintf(buffer, "The time is not proper at the line %li in file %s: begin time %s and endtime %s", lineNum, filename.c_str(), l_start, l_end);
					LOG_ERR(m_pLogger, buffer);
				}
			}
		}
	}
    
	LOG_INFO(m_pLogger, "loading of file '" + filename + "' done");
	file.close();
    
	if(isEmpty())
	{
		LOG_FATAL(m_pLogger, "UEM file '" + filename + "' contains no data!");
		exit(1);
	}
	
	m_bUseFile = true;
}

unsigned long int UEMFilter::ProcessSingleSpeech(Speech* speech)
{
	ulint nbrerr = 0;
	list<Segment*> listSegmentsToRemove;
	
	// Step 1: checking if the input is proper and listing segments to remove
	if(speech->GetParentSpeechSet()->IsRef())
	{
		// It's a Ref so check the bad ones
		for(size_t segindex=0; segindex<speech->NbOfSegments(); ++segindex)
		{
			Segment* pSegment = speech->GetSegment(segindex);
		
			string segFile = pSegment->GetSource();
			string segChannel = pSegment->GetChannel();
			int segStartTime = pSegment->GetStartTime();
			int segEndTime = pSegment->GetEndTime();
			
			list<UEMElement*>* pListUEMElement = new list<UEMElement*>;
			FindElement(segFile, segChannel, pListUEMElement);
			
			if(pListUEMElement->empty())
			{
				listSegmentsToRemove.push_back(pSegment);
			}
			else
			{
				list<UEMElement*>::iterator  i = pListUEMElement->begin();
				list<UEMElement*>::iterator ei = pListUEMElement->end();
				bool keep = false;
				
				while( (i != ei) || (!keep) )
				{
					UEMElement* pUEMElement = *i;
					int uemStartTime = pUEMElement->GetStartTime();
					int uemEndTime = pUEMElement->GetEndTime();
					keep = ( (uemStartTime <= segStartTime) && (segEndTime <= uemEndTime) );
					
					if(!keep)
					{
						if( (segStartTime < uemStartTime) && (uemEndTime < segEndTime) ||
							(uemStartTime < segStartTime) && (uemEndTime < segEndTime) ||
							(segStartTime < uemStartTime) && (segEndTime < uemEndTime) )
						{
							++nbrerr;
							LOG_ERR(m_pLogger, "UEMFilter - " + segFile + "/" + segChannel + " has an unproper time regarding the UEM file.");
						}
					}
					
					++i;
				}
				
				if(!keep)
				{
					listSegmentsToRemove.push_back(pSegment);
				}
			}
			
			pListUEMElement->clear();
			delete pListUEMElement;
		}
	}
	else if(speech->GetParentSpeechSet()->IsHyp())
	{
		// It's a Hyp so just remove them regarding the mid point of every token.
		for(size_t segindex=0; segindex<speech->NbOfSegments(); ++segindex)
		{
			Segment* pSegment = speech->GetSegment(segindex);
			int segMidPoint = (pSegment->GetStartTime() + pSegment->GetEndTime())/2;
			string segFile = pSegment->GetSource();
			string segChannel = pSegment->GetChannel();
			
			list<UEMElement*>* pListUEMElement = new list<UEMElement*>;
			FindElement(segFile, segChannel, pListUEMElement);
			
			if(pListUEMElement->empty())
			{
				listSegmentsToRemove.push_back(pSegment);
			}
			else
			{
				list<UEMElement*>::iterator  i = pListUEMElement->begin();
				list<UEMElement*>::iterator ei = pListUEMElement->end();
				bool keep = false;
				
				while( (i != ei) || (!keep) )
				{
					UEMElement* pUEMElement = *i;
					int uemStartTime = pUEMElement->GetStartTime();
					int uemEndTime = pUEMElement->GetEndTime();
					keep = ( (uemStartTime <= segMidPoint) && (segMidPoint <= uemEndTime) );					
					++i;
				}
				
				if(!keep)
				{
					listSegmentsToRemove.push_back(pSegment);
				}
			}
			
			pListUEMElement->clear();
			delete pListUEMElement;
		}
	}
	else
	{
		LOG_FATAL(m_pLogger, "UEMFilter::ProcessSingleSpeech() - Neither Ref nor Hyp - do nothing!");
		// Not defined so... for the moment do nothing
		exit(0);
	}
	
	// Step 2: removing the unwanted segments
	list<Segment*>::iterator  i = listSegmentsToRemove.begin();
	list<Segment*>::iterator ei = listSegmentsToRemove.end();
	
	while(i != ei)
	{
		speech->RemoveSegment(*i);
		++i;
	}
	
	listSegmentsToRemove.clear();
	
	return nbrerr;
}

unsigned long int UEMFilter::ProcessSpeechSet(SpeechSet* references, map<string, SpeechSet*> &hypothesis)
{
	/* Generation of the ISGs */
	if(references->HasInterSegmentGap())
	{
		LOG_INFO(m_pLogger, "UEMFilter: Inter Segment Gap detected on the input - Abording addition of ISGs");
		return 0;
	}
	
	LOG_INFO(m_pLogger, "UEMFilter:  Adding Inter Segment Gaps to refeferences");
	
	ulint nbrerr = 0;
	CTMSTMRTTMSegmentor* pCTMSTMRTTMSegmentor = new CTMSTMRTTMSegmentor();
	SpeechSet* tmppSpeechSet = new SpeechSet();
	Speech* ISGspeech = new Speech(references);
	string file = "";
	string channel = "";
	
	pCTMSTMRTTMSegmentor->Reset(references, tmppSpeechSet);
	
	list<int> listTime;
	
	int minRef = -1;
	int maxRef = -1;
	long int ElmNum = 0;
	long int LinNum = 0;
	
	while (pCTMSTMRTTMSegmentor->HasNext())
	{
		SegmentsGroup* pSG = pCTMSTMRTTMSegmentor->Next();
		int minSG = -1;
		int maxSG = -1;
		
		size_t numRefs = pSG->GetNumberOfReferences();
		
		for(size_t i=0; i<numRefs; ++i)
		{
			vector<Segment*> vecSeg = pSG->GetReference(i);
			
			for(size_t j=0; j<vecSeg.size(); ++j)
			{
				if( (minSG == -1) || (vecSeg[j]->GetStartTime() < minSG) )
					minSG = vecSeg[j]->GetStartTime();
					
				if( (maxSG == -1) || (vecSeg[j]->GetEndTime() > maxSG) )
					maxSG = vecSeg[j]->GetEndTime();
					
				file = vecSeg[j]->GetSource();
				channel = vecSeg[j]->GetChannel();
				
				if(vecSeg[j]->GetSourceLineNum() > LinNum)
					LinNum = vecSeg[j]->GetSourceLineNum();
					
				if(vecSeg[j]->GetSourceElementNum() > ElmNum)
					ElmNum = vecSeg[j]->GetSourceElementNum();
			}
		}
		
		listTime.push_back(minSG);
		listTime.push_back(maxSG);
		
		if( (minRef == -1) || (minSG < minRef) )
			minRef = minSG;
			
		if( (maxRef == -1) || (maxSG > maxRef) )
			maxRef = maxSG;
		
		if(pSG)
			delete pSG;
	}
	
	if(m_bUseFile)
	{
		list<UEMElement*>* pListUEMElement = new list<UEMElement*>;
		FindElement(file, channel, pListUEMElement);
		
		list<UEMElement*>::iterator  i = pListUEMElement->begin();
		list<UEMElement*>::iterator ei = pListUEMElement->end();
		
		while(i != ei)
		{
			UEMElement* pUEMElement = *i;
			listTime.push_back(pUEMElement->GetStartTime());
			listTime.push_back(pUEMElement->GetEndTime());					
			++i;
		}
		
		pListUEMElement->clear();
		delete pListUEMElement;
	}
	else
	{	
		map<string, SpeechSet* >::iterator hi  = hypothesis.begin();
		map<string, SpeechSet* >::iterator hei = hypothesis.end();
		
		int minHyp = minRef;
		int maxHyp = maxRef;
		
		while(hi != hei)
		{
			SpeechSet* spkset = hi->second;
			
			for(size_t spseti = 0; spseti < spkset->GetNumberOfSpeech(); ++spseti)
			{
				Speech* speh = spkset->GetSpeech(spseti);
				
				for(size_t spj=0; spj<speh->NbOfSegments(); ++spj)
				{
					vector<Token*> vectok = speh->GetSegment(spj)->ToTopologicalOrderedStruct();
					
					for(size_t veci=0; veci<vectok.size(); ++veci)
					{
						if(vectok[veci]->GetStartTime() < minHyp)
							minHyp = vectok[veci]->GetStartTime();
							
						if(vectok[veci]->GetEndTime() > maxHyp)
							maxHyp = vectok[veci]->GetEndTime();
					}
						
					vectok.clear();
				}
			}
			
			++hi;
		}
		
		listTime.push_back(maxHyp);
		listTime.push_front(minHyp);
	}
	
	listTime.sort();
	
	list<int>::iterator  l = listTime.begin();
	list<int>::iterator el = listTime.end();
	
	while(l != el)
	{
		int begintime = (*l);Á
		
		++l;
		
		if(l == el)
		{
			LOG_FATAL(m_pLogger, "UEMFilter::ProcessSpeechSet() - Invalid list of time");
			exit(-1);
		}
		
		int endtime = (*l);
		
		if(begintime != endtime)
		{
			Segment* Inter_Segment_Gap = Segment::CreateWithEndTime(begintime, endtime, ISGspeech);
			
			++ElmNum;
			++LinNum;
			Inter_Segment_Gap->SetSource(file);
			Inter_Segment_Gap->SetChannel(channel);
			Inter_Segment_Gap->SetSpeakerId(string("inter_segment_gap"));
			Inter_Segment_Gap->SetSourceLineNum(ElmNum);
			Inter_Segment_Gap->SetSourceElementNum(LinNum);
			
			size_t nbSeg = ISGspeech->NbOfSegments();
			
			ostringstream osstr;
			osstr << "(Inter_Segment_Gap-";
			osstr << setw(3) << nouppercase << setfill('0') << nbSeg << ")";
			Inter_Segment_Gap->SetId(osstr.str());
						
			ISGspeech->AddSegment(Inter_Segment_Gap);
		}
		
		++l;
	}
		
	references->AddSpeech(ISGspeech);
		
	listTime.clear();
	
	if(pCTMSTMRTTMSegmentor)
		delete pCTMSTMRTTMSegmentor;
		
	if(tmppSpeechSet)
		delete tmppSpeechSet;
	
	return nbrerr;
}
