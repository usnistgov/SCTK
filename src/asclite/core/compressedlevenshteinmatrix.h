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
	
#ifndef COMPRESSEDLEVENSHTEINMATRIX_H
#define COMPRESSEDLEVENSHTEINMATRIX_H

#include "levenshteinmatrix.h"
#include "properties.h"

/**
 * Represent the Levenshtein Distance Matrix w/ compression
 */
class CompressedLevenshteinMatrix : public LevenshteinMatrix
{
	private:
		ullint  m_SizeOfArray;
		size_t  m_NbrDimensions;
		ullint  m_MaxSize;
		ullint* m_MultiplicatorDimension;
		bool*   m_TabbIsCompressed;
		ulint*  m_TabHitsTimer;
		
		int** m_TabStartByte;
		int** m_TabStartByteCompressed;
		uint* m_TabSizes;
		size_t    m_NbrCompressedTabs;
		lzo_uint  m_BaseLengthIn;
		lzo_uint  m_BaseLengthOut;
		
		lzo_align_t* m_pWorkMemory;
		
		size_t m_MaxMemoryKBProp;
		uint m_BlockSizeKB;
		size_t m_CurrentMemorySize;
			
		static Logger* m_pLogger;
	
        void CoordinatesToBlockOffset(size_t* coordinates, size_t & blockNum, size_t & blockOffset);
		
		void CreateBlock(size_t block_index);
		
		void CompressBlock(size_t block_index);
		bool DecompressBlock(size_t block_index);
		
		bool isBlockCreated(size_t block_index) { return m_TabIsCreated[block_index]; }
				
		void GarbageCollection();
		bool ForcedGarbageCollection();
		void TouchBlock(size_t block_index) { m_TabHitsTimer[block_index] = m_Accesses++; }
		
		ulint m_Decompressions;
		ulint m_Compressions;
		ulint m_NbrCompressedBlocks;
		ulint m_NbrDecompressedBlocks;
		
		bool*   m_TabIsCreated;
		size_t  m_NbrCreatedBlocks;
		
		size_t m_OverHeadMemory;
		double m_UsableMemoryKB;
		double m_PercentageMemoryTriggerStart;
		double m_PercentageMemoryTriggerStop;
		
		ulint m_Accesses;
		
		double MemoryUsedKB() { return(((double)m_CurrentMemorySize)/((double)1024)); }
		bool isCallGarbageCollector() { return( (MemoryUsedKB()+((double)(m_BaseLengthIn))/1024.0) >= m_UsableMemoryKB*(1.0-m_PercentageMemoryTriggerStart) ); }
		bool isStopGarbageCollector() { return( MemoryUsedKB() <= m_UsableMemoryKB*(1.0-m_PercentageMemoryTriggerStop) ); }
		
		size_t* m_TabBlockDimensionDeep;
		size_t* m_TabBlockDivider;
		size_t* m_TabDimensionDeep;
		size_t* m_MultiplicatorDivider;
		
		void BlockComputation(size_t maxadddiv);
		
		size_t* m_MultiplicatorBlockDimension;
		size_t m_BlockSizeElts;

	public:
		CompressedLevenshteinMatrix(size_t _NbrDimensions, size_t* _TabDimensionDeep);
		~CompressedLevenshteinMatrix();
	
		int GetCostFor(size_t* coordinates);
		void SetCostFor(size_t* coordinates, int cost);
		bool IsCostCalculatedFor(size_t* coordinates) { return(GetCostFor(coordinates) != C_UNCALCULATED); }
		size_t GetNumberOfCalculatedCosts() { return m_SizeOfArray; }
        size_t GetMaxSize() { return m_MaxSize; }
		
		string ToString();
};

#endif
