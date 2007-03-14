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
 
/**
 * Represent the Levenshtein Distance Matrix with compression
 */
	
#include "compressedlevenshteinmatrix.h"

Logger* CompressedLevenshteinMatrix::m_pLogger = Logger::getLogger(); 

CompressedLevenshteinMatrix::CompressedLevenshteinMatrix(size_t _NbrDimensions, size_t* _TabDimensionDeep)
{
	m_CompressionLevel = 1; /* compression level (1-10) */
	
	if (ucl_init() != UCL_E_OK)
	{
		LOG_FATAL(m_pLogger, "internal error - ucl_init() failed !!!");
		exit(0);
	}
	
	m_MaxMemoryKBProp = (size_t) ceil(1024*1024*atof(Properties::GetProperty("recording.maxnbofgb").c_str()));
	m_BlockSizeKB = (uint) atoi(Properties::GetProperty("align.memorycompressionblock").c_str());
		
	m_BaseLengthIn = m_BlockSizeKB * 256 * sizeof(int); /* block of 'm_BlockSizeKB' KB */
	m_BaseLengthOut = m_BaseLengthIn + m_BaseLengthIn / 8 + 256;
	
	m_NbrDimensions = _NbrDimensions;
	m_MultiplicatorDimension = new size_t[m_NbrDimensions];
	
	m_MultiplicatorDimension[0] = 1;
	m_MaxSize = _TabDimensionDeep[0] - 1;
		
	for(size_t i=1; i<m_NbrDimensions; ++i)
	{
		m_MultiplicatorDimension[i] = m_MultiplicatorDimension[i-1]*(_TabDimensionDeep[i-1] - 1);
		m_MaxSize = m_MaxSize * (_TabDimensionDeep[i] - 1);
	}
	
	m_NbrCompressedTabs = m_MaxSize / (m_BaseLengthIn/sizeof(int)) + 1;
	
	m_TabStartByte = new ucl_intp[m_NbrCompressedTabs];
	m_TabSizes = new ucl_uint[m_NbrCompressedTabs];
	m_TabbIsCompressed = new bool[m_NbrCompressedTabs];
	m_TabHitsTimer = new unsigned long long int[m_NbrCompressedTabs];
	m_TabIsCreated = new bool[m_NbrCompressedTabs];
	m_CurrentMemorySize = 0;
	
	m_Decompressions = 0;
	m_Compressions = 0;
	m_NbrCompressedBlocks = 0;
	m_NbrDecompressedBlocks = 0;
	m_Accesses = 0;
	
	for(size_t i=0; i<m_NbrCompressedTabs; ++i)
	{
		m_TabIsCreated[i] = false;
		m_TabSizes[i] = 0;
		m_TabStartByte[i] = NULL;
		m_CurrentMemorySize += 0;
	}
	
	m_SizeOfArray = 0;
	m_NbrCreatedBlocks = 0;
	
	m_OverHeadMemory = m_BaseLengthOut + m_NbrDimensions*sizeof(size_t) + m_NbrCompressedTabs*( sizeof(ucl_intp) + sizeof(ucl_uint) + 2*sizeof(bool) + sizeof(unsigned long long int) ) + 20*sizeof(double);
	m_UsableMemoryKB = 0.98*( ((double) m_MaxMemoryKBProp) - ((double) m_OverHeadMemory)/((double) 1024) );
	m_PercentageMemoryTriggerStart = 0.01;
	m_PercentageMemoryTriggerStop = 0.2;
	
	LOG_DEBUG(m_pLogger, "Allocation done!");
	
	char buffer[BUFFER_SIZE];
	sprintf(buffer, "Compressed Levenshtein Matrix: Overhead: %luKB, Usable: %.0fKB, StartGC: %.0fKB, StopGC: %.0fKB", m_OverHeadMemory/1024, m_UsableMemoryKB, m_UsableMemoryKB*(1.0-m_PercentageMemoryTriggerStart), m_UsableMemoryKB*(1.0-m_PercentageMemoryTriggerStop));	   
	LOG_DEBUG(m_pLogger, buffer);
}

CompressedLevenshteinMatrix::~CompressedLevenshteinMatrix()
{
	char buffer[BUFFER_SIZE];
	sprintf(buffer, "Compressed Levenshtein Matrix: Total Size: %lu, Theory-blocks: %lu, Created: %lu, Calculated: %lu", (ulint) m_MaxSize, (ulint) m_NbrCompressedTabs, (ulint) m_NbrCreatedBlocks, (ulint) m_SizeOfArray);	   
	LOG_DEBUG(m_pLogger, buffer);
	
	// For information only
	/*
	for(size_t i=0; i<m_NbrCompressedTabs; ++i)
	{
		if(isBlockCreated(i))
			CompressBlock(i);		
	}
	char buffer2[BUFFER_SIZE];
	sprintf(buffer2, "Compressed Matrix (ALL): Acc: %lu, Decompr-op: %lu, Compr-op: %lu, Size: %lu, DecompBlk: %lu, CompBlk: %lu", m_Accesses, m_Decompressions, m_Compressions, m_CurrentMemorySize, m_NbrDecompressedBlocks, m_NbrCompressedBlocks);	   
	LOG_DEBUG(m_pLogger, buffer2);
	*/
	// End Information
	
	delete [] m_MultiplicatorDimension;
	
	for(size_t i=0; i<m_NbrCompressedTabs; ++i)
	{
		if(isBlockCreated(i))
			if(m_TabStartByte[i])
				ucl_free(m_TabStartByte[i]);		
	}
	
	delete [] m_TabStartByte;
	delete [] m_TabSizes;
	delete [] m_TabbIsCompressed;
	delete [] m_TabHitsTimer;
	delete [] m_TabIsCreated;
}

void CompressedLevenshteinMatrix::CreateBlock(size_t block_index)
{
	if(! isBlockCreated(block_index))
	{
		ucl_uint decomp_lengh = m_BaseLengthIn;
		ucl_intp decomp_data = (ucl_intp) ucl_malloc(decomp_lengh);
		ucl_memset(decomp_data, C_UNCALCULATED, decomp_lengh);
		
		m_TabSizes[block_index] = decomp_lengh;
		m_TabStartByte[block_index] = decomp_data;
		m_CurrentMemorySize += decomp_lengh;
		m_TabbIsCompressed[block_index] = false;
		++m_NbrDecompressedBlocks;
		m_TabIsCreated[block_index] = true;
		TouchBlock(block_index);
		++m_NbrCreatedBlocks;
		GarbageCollection();
	}
}

void CompressedLevenshteinMatrix::CompressBlock(size_t block_index)
{
	CreateBlock(block_index);
	
	if(!m_TabbIsCompressed[block_index])
	{		
		// Block is not compressed, then compress it;
		ucl_uint decomp_lengh = m_TabSizes[block_index];
		ucl_intp decomp_data = m_TabStartByte[block_index];
		
		ucl_uint comp_lengh = m_BaseLengthOut;
		ucl_intp comp_data = (ucl_intp) ucl_malloc(comp_lengh);
		
		if( ucl_nrv2d_99_compress((ucl_bytep) decomp_data, decomp_lengh, (ucl_bytep) comp_data, &comp_lengh, NULL, m_CompressionLevel, NULL, NULL) != UCL_E_OK)
		{
			LOG_FATAL(m_pLogger, "internal error - ucl_nrv2d_99_compress() failed !!!");
			exit(0);
		}
		
		ucl_free(decomp_data);
		m_TabSizes[block_index] = comp_lengh;
		m_TabStartByte[block_index] = comp_data;
		m_TabbIsCompressed[block_index] = true;
		m_CurrentMemorySize += comp_lengh - decomp_lengh;
		++m_Compressions;
		++m_NbrCompressedBlocks;
		--m_NbrDecompressedBlocks;
	}
}

bool CompressedLevenshteinMatrix::DecompressBlock(size_t block_index)
{
	CreateBlock(block_index);
	
	bool decomp = false;
	
	if(decomp = m_TabbIsCompressed[block_index])
	{
		// Block is compressed, then compress it;
		ucl_uint comp_lengh = m_TabSizes[block_index];
		ucl_intp comp_data = m_TabStartByte[block_index];
		
		ucl_uint decomp_lengh = m_BaseLengthIn;
		ucl_intp decomp_data = (ucl_intp) ucl_malloc(decomp_lengh);
		
		if( ucl_nrv2d_decompress_8((ucl_bytep) comp_data, comp_lengh, (ucl_bytep) decomp_data, &decomp_lengh, NULL) != UCL_E_OK)
		{
			LOG_FATAL(m_pLogger, "internal error - ucl_nrv2d_decompress_8() failed !!!");
			exit(0);
		}
		
		ucl_free(comp_data);
		m_TabSizes[block_index] = decomp_lengh;
		m_TabStartByte[block_index] = decomp_data;
		m_TabbIsCompressed[block_index] = false;
		m_CurrentMemorySize += decomp_lengh - comp_lengh;
		++m_Decompressions;
		--m_NbrCompressedBlocks;
		++m_NbrDecompressedBlocks;
	}
	
	TouchBlock(block_index);
	return decomp;
}
/*
void CompressedLevenshteinMatrix::TouchBlock(size_t block_index)
{
	m_TabHitsTimer[block_index] = m_Accesses++;
	
	// For information only
	if(m_pLogger->getLogLevel() >= 5 && m_pLogger->getLogLevel() != 7)
	{
		char buffer[BUFFER_SIZE];
		sprintf(buffer, "Compressed Matrix: Acc: %lu, Decompr-op: %lu, Compr-op: %lu, Size: %lu, DecompBlk: %lu, CompBlk: %lu", m_Accesses, m_Decompressions, m_Compressions, m_CurrentMemorySize, m_NbrDecompressedBlocks, m_NbrCompressedBlocks);	   
		LOG_DEBUG(m_pLogger, buffer);
	}
	// End Information
}
*/

void CompressedLevenshteinMatrix::GarbageCollection()
{
	if(isCallGarbageCollector())
	{
		bool found = false;
		ulint count = 0;
		
		do
		{
			found = ForcedGarbageCollection();
			
			if(found)
				++count;
		}
		while(found && !isStopGarbageCollector());

		char buffer[BUFFER_SIZE];
		sprintf(buffer, "Garbage collector called %lu times", count);	   
		LOG_DEBUG(m_pLogger, buffer);
	}
}

bool CompressedLevenshteinMatrix::ForcedGarbageCollection()
{
	unsigned long long int mintouch = ULLONG_MAX;
	size_t min_index = 0;

	// Do the ugly Java GC
	bool found = false;
	
	for(size_t i=0; i<m_NbrCompressedTabs; ++i)
	{
		if(isBlockCreated(i))
		{
			if(!m_TabbIsCompressed[i])
			{
				// not compressed
				if(m_TabHitsTimer[i] < mintouch)
				{				
					mintouch = m_TabHitsTimer[i];
					min_index = i;
					found = true;
				}
			}
		}
	}
	
	if(found)
		CompressBlock(min_index);
	
	return found;
}

string CompressedLevenshteinMatrix::ToString()
{
	return string("");
}

size_t CompressedLevenshteinMatrix::CoordinatesToSize(size_t* coordinates)
{
	size_t outSize = 0;
	
	for(size_t i=0; i<m_NbrDimensions; ++i)
		outSize += m_MultiplicatorDimension[i]*coordinates[i];
	
	return outSize;
}

int CompressedLevenshteinMatrix::GetCostFor(size_t* coordinates)
{
	size_t coord_size = CoordinatesToSize(coordinates);
	size_t coord_x = coord_size / (m_BaseLengthIn/sizeof(int));
	size_t coord_y = coord_size % (m_BaseLengthIn/sizeof(int));

	bool decomp = DecompressBlock(coord_x);
	ucl_intp data = m_TabStartByte[coord_x];
	int out = data[coord_y];
	
	if(decomp)
		GarbageCollection();
		
	return (out);
}

void CompressedLevenshteinMatrix::SetCostFor(size_t* coordinates, int cost)
{
	size_t coord_size = CoordinatesToSize(coordinates);
	size_t coord_x = coord_size / (m_BaseLengthIn/sizeof(int));
	size_t coord_y = coord_size % (m_BaseLengthIn/sizeof(int));
	
	bool decomp = DecompressBlock(coord_x);
	ucl_intp data = m_TabStartByte[coord_x];
	
	if(data[coord_y] == C_UNCALCULATED)
		++m_SizeOfArray;
	
	data[coord_y] = cost;
		
	if(decomp)
		GarbageCollection();
}
