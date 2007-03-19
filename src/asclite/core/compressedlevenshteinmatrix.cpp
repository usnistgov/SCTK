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
							/* 1 faster and lower compression but still efficient */
	
	if (ucl_init() != UCL_E_OK)
	{
		LOG_FATAL(m_pLogger, "internal error - ucl_init() failed !!!");
		exit(0);
	}
	
	m_MaxMemoryKBProp = (size_t) ceil(1024*1024*atof(Properties::GetProperty("recording.maxnbofgb").c_str()));
	m_BlockSizeKB = (uint) atoi(Properties::GetProperty("align.memorycompressionblock").c_str());
	
	m_NbrDimensions = _NbrDimensions;
	m_TabDimensionDeep = new size_t[m_NbrDimensions];
	m_MultiplicatorDimension = new size_t[m_NbrDimensions];
	m_TabBlockDivider = new size_t[m_NbrDimensions];
	m_TabBlockDimensionDeep = new size_t[m_NbrDimensions];
	
	m_MultiplicatorDimension[0] = 1;
	m_TabDimensionDeep[0] = _TabDimensionDeep[0] - 1;
	m_MaxSize = m_TabDimensionDeep[0];
	m_TabBlockDivider[0] = 1;
		
	for(size_t i=1; i<m_NbrDimensions; ++i)
	{
		m_TabDimensionDeep[i] = _TabDimensionDeep[i] - 1;
		m_MultiplicatorDimension[i] = m_MultiplicatorDimension[i-1]*m_TabDimensionDeep[i-1];
		m_MaxSize = m_MaxSize * m_TabDimensionDeep[i];
		m_TabBlockDivider[i] = 1;
	}
	
	BlockComputation();
	
	m_BaseLengthOut = m_BaseLengthIn + m_BaseLengthIn / 8 + 256;
	
	m_MultiplicatorBlockDimension = new size_t[m_NbrDimensions];
	m_MultiplicatorDivider = new size_t[m_NbrDimensions];
		
	m_MultiplicatorBlockDimension[0] = 1;
	m_MultiplicatorDivider[0] = 1;
		
	for(size_t i=1; i<m_NbrDimensions; ++i)
	{
		m_MultiplicatorBlockDimension[i] = m_MultiplicatorBlockDimension[i-1]*m_TabBlockDimensionDeep[i-1];
		m_MultiplicatorDivider[i] = m_MultiplicatorDivider[i-1]*m_TabBlockDivider[i-1];
	}	
		
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
	sprintf(buffer, "Compressed Levenshtein Matrix: %lu blocks of %.1fKB, Overhead: %luKB, Usable: %.0fKB, StartGC: %.0fKB, StopGC: %.0fKB", 
			(ulint) m_NbrCompressedTabs, ((double)(m_BaseLengthIn))/1024.0 , m_OverHeadMemory/1024, m_UsableMemoryKB, m_UsableMemoryKB*(1.0-m_PercentageMemoryTriggerStart), m_UsableMemoryKB*(1.0-m_PercentageMemoryTriggerStop));	   
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
	sprintf(buffer2, "Compressed Matrix (ALL): Acc: %llu, Decompr-op: %lu, Compr-op: %lu, Size: %lu, DecompBlk: %lu, CompBlk: %lu", m_Accesses, m_Decompressions, m_Compressions, m_CurrentMemorySize, m_NbrDecompressedBlocks, m_NbrCompressedBlocks);	   
	LOG_DEBUG(m_pLogger, buffer2);
	*/
	// End Information
	
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
	delete [] m_TabBlockDimensionDeep;
	delete [] m_MultiplicatorBlockDimension;
	delete [] m_TabBlockDivider;
	delete [] m_TabDimensionDeep;
	delete [] m_MultiplicatorDivider;
	delete [] m_MultiplicatorDimension;
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
		sprintf(buffer, "Compressed Matrix: Acc: %llu, Decompr-op: %lu, Compr-op: %lu, Size: %lu, DecompBlk: %lu, CompBlk: %lu", m_Accesses, m_Decompressions, m_Compressions, m_CurrentMemorySize, m_NbrDecompressedBlocks, m_NbrCompressedBlocks);	   
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

void CompressedLevenshteinMatrix::CoordinatesToBlockOffset(size_t* coordinates, size_t & blockNum, size_t & blockOffset)
{
	blockNum = 0;
	blockOffset = 0;
	
	for(size_t i=0; i<m_NbrDimensions; ++i)
	{
		blockNum += (coordinates[i]/m_TabBlockDimensionDeep[i])*m_MultiplicatorDivider[i];
		blockOffset += (coordinates[i]%m_TabBlockDimensionDeep[i])*m_MultiplicatorBlockDimension[i];
	}
}

int CompressedLevenshteinMatrix::GetCostFor(size_t* coordinates)
{
	size_t coord_x;
	size_t coord_y;
	CoordinatesToBlockOffset(coordinates, coord_x, coord_y);

	bool decomp = DecompressBlock(coord_x);
	ucl_intp data = m_TabStartByte[coord_x];
	int out = data[coord_y];
	
	if(decomp)
		GarbageCollection();
		
	return (out);
}

void CompressedLevenshteinMatrix::SetCostFor(size_t* coordinates, int cost)
{
	size_t coord_x;
	size_t coord_y;
	CoordinatesToBlockOffset(coordinates, coord_x, coord_y);
	
	bool decomp = DecompressBlock(coord_x);
	ucl_intp data = m_TabStartByte[coord_x];
	
	if(data[coord_y] == C_UNCALCULATED)
		++m_SizeOfArray;
	
	data[coord_y] = cost;
		
	if(decomp)
		GarbageCollection();
}

size_t CompressedLevenshteinMatrix::BlockComputation_GetNumElts(size_t* _tab)
{
	size_t numberElts = 1;

	for(size_t i=0; i<m_NbrDimensions; ++i)
		numberElts *= _tab[i];
	
	return numberElts;
}

void CompressedLevenshteinMatrix::BlockComputation_CreateNewDim()
{
	for(size_t i=0; i<m_NbrDimensions; ++i)
	{
		m_TabBlockDimensionDeep[i] = m_TabDimensionDeep[i]/m_TabBlockDivider[i];
		
		if(m_TabDimensionDeep[i] % m_TabBlockDivider[i] != 0)
			m_TabBlockDimensionDeep[i]++;
	}
}

size_t CompressedLevenshteinMatrix::BlockComputation_BiggerDim()
{
	size_t max = 0;
	size_t indexmax = ULONG_MAX;
	size_t num1 = 0;
	size_t index1 = ULONG_MAX;
	
	for(size_t i=0; i<m_NbrDimensions; ++i)
	{
		if(m_TabDimensionDeep[i] == m_TabBlockDimensionDeep[i])
		{
			num1++;
			index1 = i;
		}
	
		if(m_TabBlockDimensionDeep[i] > max)
		{
			max = m_TabBlockDimensionDeep[i];
			indexmax = i;
		}
	}

	if(num1 != 0)
		return index1;
	else
		return indexmax;
}

void CompressedLevenshteinMatrix::BlockComputation()
{
	BlockComputation_CreateNewDim();
	size_t blocksize = m_BlockSizeKB*256;
	BlockComputation_CreateNewDim();
	size_t numberElts = BlockComputation_GetNumElts(m_TabBlockDimensionDeep);
	size_t dimm = BlockComputation_BiggerDim();
	
	while(numberElts > blocksize && dimm != ULONG_MAX)
	{
		m_TabBlockDivider[dimm]++;		
		BlockComputation_CreateNewDim();
		numberElts = BlockComputation_GetNumElts(m_TabBlockDimensionDeep);
		dimm = BlockComputation_BiggerDim();
	}

	m_BlockSizeElts = numberElts;
	m_BaseLengthIn = numberElts * sizeof(int);
	m_NbrCompressedTabs = BlockComputation_GetNumElts(m_TabBlockDivider);
}

