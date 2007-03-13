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
 * A recording contain all the data needed to score a testset.
 * From the argument to the list of parser (input) and generator (output report).
 * This is the main entry to the aligner.
 */

#include "recording.h" // class's header file

Logger* Recording::logger = Logger::getLogger();

// class constructor
Recording::Recording()
{   
	//init the input parsers
	inputParsers["ctm"] = new CTMInputParser;
	inputParsers["stm"] = new STMInputParser;
	inputParsers["trn"] = new TRNInputParser;
    inputParsers["rttm"] = new RTTMInputParser;
	
	//init the report generator
	reportGenerators["rsum"] = new RAWSYSReportGenerator(1);
	reportGenerators["sum"] = new RAWSYSReportGenerator(2);
	reportGenerators["sgml"] = new SGMLReportGenerator();
	
	//init the segmentor
	segmentors["trn|trn"] = new TRNTRNSegmentor();
	segmentors["ctm|stm"] = new CTMSTMRTTMSegmentor();
	segmentors["rttm|stm"] = new CTMSTMRTTMSegmentor();
	segmentors["rttm|rttm"] = new CTMSTMRTTMSegmentor();
	
	//init the Aligner
	aligner["lev"] = new Levenshtein();
  
	//init the Scorer
	scorer["stt"] = new STTScorer();
  
	//init the Filters
	filters["validate.spkr_auto_overlap"] = new SpkrAutoOverlap();
	filters["validate.glm_filter"] = new GLMFilter();
  
	//init the Alignment result
	alignments = new Alignment();
	
	//Init SpeakerMatch
	m_pSpeakerMatch = new SpeakerMatch();
}

// class destructor
Recording::~Recording()
{
	if(m_pSpeakerMatch)
		delete m_pSpeakerMatch;
	
	map<string, InputParser*>::iterator ipi, ipe;
	
	ipi = inputParsers.begin();
	ipe = inputParsers.end();
	
	while(ipi != ipe)
	{
		InputParser* ptr_elt = ipi->second;
		
		if(ptr_elt)
			delete ptr_elt;
		
		++ipi;
	}
	
	inputParsers.clear();
	
	map<string, ReportGenerator*>::iterator rgi, rge;
	
	rgi = reportGenerators.begin();
	rge = reportGenerators.end();
	
	while(rgi != rge)
	{
		ReportGenerator* ptr_elt = rgi->second;
		
		if(ptr_elt)
			delete ptr_elt;
		
		++rgi;
	}
	
	reportGenerators.clear();
	
	map<string, Aligner*>::iterator ai, ae;
	
	ai = aligner.begin();
	ae = aligner.end();
	
	while(ai != ae)
	{
		Aligner* ptr_elt = ai->second;
		
		if(ptr_elt)
			delete ptr_elt;
		
		++ai;
	}
	
	aligner.clear();
	
	map<string, Scorer*>::iterator si, se;
	
	si = scorer.begin();
	se = scorer.end();
	
	while(si != se)
	{
		Scorer* ptr_elt = si->second;
		
		if(ptr_elt)
			delete ptr_elt;
		
		++si;
	}
	
	scorer.clear();
	
	map<string, Segmentor*>::iterator segi, sege;
	
	segi = segmentors.begin();
	sege = segmentors.end();
	
	while(segi != sege)
	{
		Segmentor* ptr_elt = segi->second;
		
		if(ptr_elt)
			delete ptr_elt;
		
		++segi;
	}
	
	segmentors.clear();
	
	map<string, Filter::Filter*>::iterator fi, fe;
	
	fi = filters.begin();
	fe = filters.end();
	
	while(fi != fe)
	{
		Filter::Filter* ptr_elt = fi->second;
		
		if(ptr_elt)
			delete ptr_elt;
		
		++fi;
	}
	
	filters.clear();
	delete alignments;
	delete references;
	
	map<string, SpeechSet* >::iterator ssi, sse;
	
	ssi = hypothesis.begin();
	sse = hypothesis.end();
	
	while(ssi != sse)
	{
		SpeechSet* ptr_elt = ssi->second;
		
		if(ptr_elt)
			delete ptr_elt;
		
		++ssi;
	}
	
	hypothesis.clear();
}

/**
* Load the reference&Hypothesis files into the system.
 * use the right loader based on the type.
 */
void Recording::Load(string _references, string _refType, vector<string> _hypothesis, vector<string> _hypothesis_titles, string _hypType, string glmFile, string speakeralignfile)
{
	if(string("true").compare(Properties::GetProperty("align.speakeroptimization")) == 0)
		m_pSpeakerMatch->Load(speakeralignfile);

    //init the segmentor
    segmentor = segmentors[_hypType+"|"+_refType];
  
	//load reference
	LOG_INFO(logger, "Load 1 reference of type "+_refType);
	references = inputParsers[_refType]->loadFile(_references);
    references->SetOrigin("ref");
	
	//if(string("rttm") == _refType)
	//	AddInterSegmentGapsToRefs();
	
    //load hypothesis
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "Load %i hypothesis of type %s", (int)_hypothesis.size(), _hypType.c_str());
 	LOG_INFO(logger, buffer);
 	string title_temp;
    
    for (size_t i=0 ; i < _hypothesis.size() ; ++i)
	{
		if(string("rttm") == _hypType)
			inputParsers[_hypType]->SetOneTokenPerSegment(true);
		
        SpeechSet* hyps_loaded = inputParsers[_hypType]->loadFile(_hypothesis[i]);
		
        hyps_loaded->SetOrigin("hyp");
        
        if (_hypothesis_titles[i] == "")
            title_temp = _hypothesis[i];
        else
			title_temp = _hypothesis_titles[i];
		
		hypothesis[title_temp] = hyps_loaded;
		alignments->AddSystem(_hypothesis[i], title_temp);
    }
	
	if(string("rttm") == _refType)
		AddInterSegmentGapsToRefs();
}

void Recording::AddInterSegmentGapsToRefs()
{
	LOG_INFO(logger, "Adding Intersegment Gaps to refs");
	
	CTMSTMRTTMSegmentor* pCTMSTMRTTMSegmentor = new CTMSTMRTTMSegmentor();
	
	SpeechSet* tmppSpeechSet = new SpeechSet();
	Speech* ISGspeech = new Speech(references);
	
	string file = "";
	string channel = "";
	
	pCTMSTMRTTMSegmentor->Reset(references, tmppSpeechSet);
	
	list<int> listTime;
	
	int minRef = INT_MAX2;
	int maxRef = 0;
	
	while (pCTMSTMRTTMSegmentor->HasNext())
	{
		SegmentsGroup* pSG = pCTMSTMRTTMSegmentor->Next();
		int minSG = INT_MAX2;
		int maxSG = 0;
		
		size_t numRefs = pSG->GetNumberOfReferences();
		
		for(size_t i=0; i<numRefs; ++i)
		{
			vector<Segment*> vecSeg = pSG->GetReference(i);
			
			for(size_t j=0; j<vecSeg.size(); ++j)
			{
				if(vecSeg[j]->GetStartTime() < minSG)
					minSG = vecSeg[j]->GetStartTime();
					
				if(vecSeg[j]->GetEndTime() > maxSG)
					maxSG = vecSeg[j]->GetEndTime();
					
				file = vecSeg[j]->GetSource();
				channel = vecSeg[j]->GetChannel();
			}
		}
		
		listTime.push_back(minSG);
		listTime.push_back(maxSG);
		
		if(minSG < minRef)
			minRef = minSG;
			
		if(maxSG > maxRef)
			maxRef = maxSG;
		
		if(pSG)
			delete pSG;
	}
	
	map<string, SpeechSet* >::iterator hi  = hypothesis.begin();
	map<string, SpeechSet* >::iterator hei = hypothesis.end();
	
	int minHyp = minRef;
	int maxHyp = maxRef;
	
	while(hi != hei)
	{
		SpeechSet* spkset = hi->second;;
		
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
	
	//listTime.push_back(INT_MAX2);
	//listTime.push_front(0);
	listTime.sort();
	
	//list<int>::iterator bl = listTime.begin();
	list<int>::iterator  l = listTime.begin();
	list<int>::iterator el = listTime.end();
	
	while(l != el)
	{
		int begintime = (*l);
		++l;
		
		if(l == el)
		{
			LOG_FATAL(logger, "Invalid list of time");
			exit(-1);
		}
		
		int endtime = (*l);
		
		if(begintime != endtime)
		{
			Segment* Inter_Segment_Gap = Segment::CreateWithEndTime(begintime, endtime, ISGspeech);
			
			Inter_Segment_Gap->SetSource(file);
			Inter_Segment_Gap->SetChannel(channel);
			Inter_Segment_Gap->SetSpeakerId(string("inter_segment_gap"));
			Inter_Segment_Gap->SetSourceLineNum(0);
			Inter_Segment_Gap->SetSourceElementNum(0);
			
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
}

/**
* Filter the references and hypothesis with the availables filters.
 */
void Recording::Filter(vector<string> _filters)
{
    unsigned long int nbErr = 0;
  
    for (size_t i=0 ; i < _filters.size() ; ++i)
    {
        LOG_INFO(logger, "Filtering ==> "+_filters[i]);
    
        for (size_t j=0 ; j < references->GetNumberOfSpeech() ; ++j)
            nbErr += filters[_filters[i]]->Process(references->GetSpeech(j));
    }
  
    if (nbErr != 0)
    {
        char buffer[BUFFER_SIZE];
        sprintf(buffer, "%li Errors in the input files, programs exiting", nbErr);
        LOG_FATAL(logger, buffer);
        exit(0);
    }
}

/**
* Align the ref to the hyp with the select align algo
 */
void Recording::Align()
{
	uint max_spkrOverlaping = atoi(Properties::GetProperty("recording.maxspeakeroverlaping").c_str());
    uint max_hypothesisOverlaping = atoi(Properties::GetProperty("recording.maxoverlapinghypothesis").c_str());
    size_t max_nb_gb = (size_t) ceil(GIGCELLS*atof(Properties::GetProperty("recording.maxnbofgb").c_str()));
	bool bForceMemoryCompression = (string("true").compare(Properties::GetProperty("align.forcememorycompression")) == 0);

    map<string, SpeechSet* >::iterator i = hypothesis.begin();
    map<string, SpeechSet* >::iterator ei = hypothesis.end();

	if(logger->isAlignLogON())
		LOG_ALIGN(logger, "Aligned,SegGrpID,File,Channel,Eval,RefSegID,RefSegBT,RefSegET,RefSpkrID,RefTknID,RefTknBT,RefTknET,RefTknTxt,RefTknConf,RefTknPrev,RefTknNext,HypSegID,HypSegBT,HypSegET,HypSpkrID,HypTknID,HypTknBT,HypTknET,HypTknTxt,HypTknConf,HypTknPrev,HypTknNext");
	
    while(i != ei)
    {		
        string hyp_name = i->first;
        LOG_INFO(logger, "Processing system "+hyp_name);
        SegmentsGroup* segmentsGroup;
        segmentor->Reset(references, hypothesis[hyp_name]);
		        
        while (segmentor->HasNext())
        {
            segmentsGroup = segmentor->Next();
			
            //cerr << "Nbr ref: " << segmentsGroup->GetNumberOfReferences() << endl;
            //cerr << "Nbr hyp: " << segmentsGroup->GetNumberOfHypothesis() << endl;
			//cerr << "SG:" << segmentsGroup->GetsID() << endl;
			//cerr << segmentsGroup->ToString() << endl;
			
			bool emptyHypvsISG = false;
            
            //Special case where there is no hyp
            if (segmentsGroup->GetNumberOfHypothesis() == 0)
            {
                //May need to put the segment time
                Segment* emptyseg = Segment::CreateWithEndTime(0, 0, NULL);
                emptyseg->SetAsSegmentExcludeFromScoring();
                segmentsGroup->AddHypothesis(emptyseg);
				
				if(segmentsGroup->GetNumberOfReferences() == 1)
				{
					if( segmentsGroup->GetReference(0)[0]->GetSpeakerId() == string("inter_segment_gap") )
					{
						emptyHypvsISG = true;
					}
				}
            }
      
            size_t cellnumber = segmentsGroup->GetDifficultyNumber();

            float KBused = (((float) cellnumber)*1024*1024)/((float) GIGCELLS);
            float MBused = KBused/((float) 1024);
            float GBused = MBused/((float) 1024);

            char buffer_size[BUFFER_SIZE];

            if(GBused > 1.0)
                sprintf(buffer_size, "%.2f GB", GBused);
            else if(MBused > 1.0)
                sprintf(buffer_size, "%.2f MB", MBused);
            else
                sprintf(buffer_size, "%.2f kB", KBused);
      
			
            char buffer[BUFFER_SIZE];

			sprintf(buffer, "Align SG %lu [%s] %lu dimensions, Difficulty: %lu (%s) ---> bt=%.3f et=%.3f", 
							(ulint) segmentsGroup->GetsID(), 
                            segmentsGroup->GetDifficultyString().c_str(),
                            (ulint) (segmentsGroup->GetNumberOfHypothesis()+segmentsGroup->GetNumberOfReferences()),
                            (ulint) cellnumber,
                            buffer_size,
                            segmentsGroup->GetMinTime()/1000.0,
                            segmentsGroup->GetMaxTime()/1000.0);
	   
            LOG_DEBUG(logger, buffer);
	  
            bool ignoreSegs = false;
			bool buseCompArray = false;
            
			if(emptyHypvsISG)
			{
				ignoreSegs = true;
				sprintf(buffer, "Skip this group of segments (%lu): Inter Segment Gap versus Empty Hyp", (ulint) segmentsGroup->GetsID());
                LOG_WARN(logger, buffer);
			}
            else if (segmentsGroup->isIgnoreInScoring())
            {
                ignoreSegs = true;
				sprintf(buffer, "Skip this group of segments (%lu): Ignore this time segments in scoring set into the references", (ulint) segmentsGroup->GetsID());
                LOG_WARN(logger, buffer);
            }
			else if (segmentsGroup->GetNumberOfReferences() > max_spkrOverlaping || segmentsGroup->GetNumberOfHypothesis() > max_hypothesisOverlaping)
            {
                ignoreSegs = true;        
				sprintf(buffer, "Skip this group of segments (%lu): nb of speaker overlaping to high", (ulint) segmentsGroup->GetsID());
                LOG_WARN(logger, buffer);
            }
			else if(bForceMemoryCompression)
			{
				buseCompArray = true;
				sprintf(buffer, "Forcing Levenshtein Matrix Compression Algorihm for group of segments (%lu)", (ulint) segmentsGroup->GetsID());
				LOG_INFO(logger, buffer);
			}
			else if (cellnumber > max_nb_gb)
            {
				bool m_bUseCompression = (string("true").compare(Properties::GetProperty("align.memorycompression")) == 0);
				
				if(!m_bUseCompression)
				{
					ignoreSegs = true;
					sprintf(buffer, "Skip this group of segments (%lu): the graph size will be too large", (ulint) segmentsGroup->GetsID());
					LOG_WARN(logger, buffer);
				}
				else
				{
					buseCompArray = true;
					sprintf(buffer, "Using Levenshtein Matrix Compression Algorihm for group of segments (%lu)", (ulint) segmentsGroup->GetsID());
					LOG_INFO(logger, buffer);
				}
            }
			
            //Special case where the SegmentGroup contain a segment time to ignore
            if (!ignoreSegs)
            {
                //cerr << segmentsGroup->ToString() << endl;
				
                Aligner* aligner_instance = aligner["lev"];
                aligner_instance->SetSegments(segmentsGroup, m_pSpeakerMatch, buseCompArray);
                aligner_instance->Align();
                GraphAlignedSegment* gas = aligner_instance->GetResults();
				
				//cerr << gas->ToString() << endl;
				
                sprintf(buffer, "Store %li GAS for hyp: '%s' cost: (%d)", gas->GetNbOfGraphAlignedToken(), hyp_name.c_str(), ((Levenshtein*)aligner_instance)->GetCost());
                LOG_DEBUG(logger, buffer);
				
				alignments->AddGraphAlignedSegment(gas, hyp_name, segmentsGroup);
								
				delete gas;
			}
			else //segmentsGroup ignored
			{
				if(logger->isAlignLogON() && !emptyHypvsISG)
					segmentsGroup->LoggingAlignment(string("NO"));
			}
			
			if(segmentsGroup)
			{
				if(logger->isAlignLogON() && !emptyHypvsISG)
					segmentsGroup->LoggingAlignment(string("SG"));
				
				delete segmentsGroup;
			}
		}
		
        ++i;
    }
}

/**
 * Score the Alignment with the selected scoring system
 */
void Recording::Score()
{
	
	Scorer* scorer_instance = scorer["stt"];
    scorer_instance->Score(alignments, m_pSpeakerMatch);
}

/**
 * Generate the reports based on the scored alignment.
 */
void Recording::GenerateReport(vector<string> reportsType)
{
	int outputint = 1;
	
	for (size_t i=0 ; i < reportsType.size() ; ++i)
		if(reportsType[i] == "stdout")
			outputint = 0;
	
	for (size_t i=0 ; i < reportsType.size() ; ++i)
		if(reportsType[i] != "stdout")
            reportGenerators[reportsType[i]]->Generate(alignments, outputint);
}

