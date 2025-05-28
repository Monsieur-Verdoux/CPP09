#include "PmergeMe.hpp"
#include <vector>
#include <iostream>

void PmergeMe::vectorSort(unsigned int pairSize) 
{
	size_t n = _vectorSorted.size();
	if (pairSize * 2 > n)
		return;
	
	for (size_t i = 0; i + 2 * pairSize - 1 < n; i += 2 * pairSize)
	{
		auto firstPairStart = _vectorSorted.begin() + i;
		auto secondPairStart = firstPairStart + pairSize;

		int lastOfFirstPair = *(firstPairStart + pairSize - 1);
		int lastOfSecondPair = *(secondPairStart + pairSize - 1);

		if (lastOfFirstPair > lastOfSecondPair)
			std::swap_ranges(firstPairStart, secondPairStart, secondPairStart);
	}

	vectorSort(pairSize * 2);

	std::vector<int> mainChain;
	std::vector<int> subChain;

	auto firstPairStart = _vectorSorted.begin();
	for (size_t i = 0; i < pairSize; ++i)
		mainChain.push_back(*(firstPairStart + i));

	for (size_t i = pairSize; i - 1 < n; i += pairSize * 2)
	{
		auto pairStart = _vectorSorted.begin() + i;
		if (i + pairSize - 1 >= n)
			break;
		for (size_t j = 0; j < pairSize; ++j)
			mainChain.push_back(*(pairStart + j));
	}

	for (size_t i = pairSize * 2; i - 1 < n; i += pairSize * 2)
	{
		auto pairStart = _vectorSorted.begin() + i;
		if (i + pairSize - 1 >= n)
			break;
		for (size_t j = 0; j < pairSize; ++j)
			subChain.push_back(*(pairStart + j));
	}

	auto binaryFindInsertion = [&](int value, size_t maxPairsIndex) {
		size_t left = 0;
		size_t right = maxPairsIndex; // inclusive

		while (left < right)
		{
			size_t mid = left + (right - left) / 2;
			int midVal = *(mainChain.begin() + mid * pairSize + pairSize - 1);
			if (midVal > value)
				right = mid;
			else
				left = mid + 1;
		}
		return mainChain.begin() + left * pairSize;
	};

	unsigned int jacobsthalIndex = 3;
	unsigned int jacobsthalPrev = jacobsthalCalc(jacobsthalIndex - 1);
	unsigned int jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);
	int insertions = jacobsthalCurr - jacobsthalPrev;

	std::vector<bool> inserted(subChain.size(), false);
	size_t insertedCount = 0;
	int offset = 0;

	while (insertions > 0)
	{
		if ((size_t)insertions > subChain.size())
			break;
		int indexOffset = 0;
		while (insertions > 0)
		{
			int insertIndexSub = (jacobsthalCurr - 1) * pairSize - 1 - indexOffset;
			if (insertIndexSub < 0 || insertIndexSub >= (int)subChain.size())
				break;

			size_t maxChunkSearchIndex = jacobsthalCurr + insertedCount - offset;
			size_t totalChunks = mainChain.size() / pairSize;
			//  std::cout << "Total chunks: " << totalChunks << std::endl;
			//std::cout << "Max chunk search index: " << maxChunkSearchIndex << std::endl;
			if (maxChunkSearchIndex >= totalChunks)
				maxChunkSearchIndex = totalChunks;

			int valueToInsert = subChain[insertIndexSub];
			auto insertionIt = binaryFindInsertion(valueToInsert, maxChunkSearchIndex);

			size_t insertionPos = insertionIt - mainChain.begin();
			size_t boundaryPos = maxChunkSearchIndex * pairSize;
			// std::cout << "Insertion position: " << insertionPos << ", Boundary position: " << boundaryPos << std::endl;
			if (insertionPos == boundaryPos && insertionPos == mainChain.size()) //needs to be investigated
			{
				offset++;  // shrink search area for next b(x-1)
			}

			// std::cout << "We are insertign chunk: ";
			// for (int j = insertIndexSub - pairSize + 1; j <= insertIndexSub; ++j)
			// 	std::cout << subChain[j] << " ";
			// std::cout << "into mainChain at position: " << (insertionIt - mainChain.begin()) / pairSize << std::endl;

			// std::cout << "Main chain before insertion: ";
			// for (const auto& val : mainChain)
			// 	std::cout << val << " ";
			// std::cout << std::endl;

			// std::cout << "Sub chain before insertion: ";
			// for (const auto& val : subChain)
			// 	std::cout << val << " ";
			// std::cout << std::endl;


			mainChain.insert(insertionIt, subChain.begin() + insertIndexSub - pairSize + 1, subChain.begin() + insertIndexSub + 1);

			// std::cout << "Main chain after insertion: ";
			// for (const auto& val : mainChain)
			// 	std::cout << val << " ";
			// std::cout << std::endl;

			inserted[insertIndexSub] = true;
			insertions--;
			insertedCount++;
			indexOffset += pairSize;
		}

		jacobsthalIndex++;
		jacobsthalPrev = jacobsthalCurr;
		jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);
		insertions = jacobsthalCurr - jacobsthalPrev;
	}

	for (int i = (int)subChain.size() - 1; i >= static_cast<int>(pairSize - 1); i -= pairSize)
	{
		if (inserted[i])
			continue;

		size_t maxChunkSearchIndex = jacobsthalCurr + insertedCount; // - offset;
		size_t totalChunks = mainChain.size() / pairSize;
		// std::cout << "Total chunks: " << totalChunks << std::endl;
		// std::cout << "Max chunk search index: " << maxChunkSearchIndex << std::endl;
		if (maxChunkSearchIndex >= totalChunks)
			maxChunkSearchIndex = totalChunks;

		int valueToInsert = subChain[i];
		auto insertionIt = binaryFindInsertion(valueToInsert, maxChunkSearchIndex);

		// size_t insertionPos = insertionIt - mainChain.begin();
		// size_t boundaryPos = maxChunkSearchIndex * pairSize;
		// // std::cout << "Insertion position: " << insertionPos << ", Boundary position: " << boundaryPos << std::endl;
		// if (insertionPos == boundaryPos) {
		// 	offset++;  // shrink search area for next b(x-1)
		// }

		// std::cout << "We are insertign chunk: ";
		// for (int j = i - pairSize + 1; j <= i; ++j)
		// 	std::cout << subChain[j] << " ";
		// std::cout << "into mainChain at position: " << (insertionIt - mainChain.begin()) / pairSize << std::endl;
		// std::cout << "Main chain before insertion: ";
		// for (const auto& val : mainChain)
		// 	std::cout << val << " ";
		// std::cout << std::endl;
		// std::cout << "Sub chain before insertion: ";
		// for (const auto& val : subChain)
		// 	std::cout << val << " ";
		// std::cout << std::endl;


	

		mainChain.insert(insertionIt, subChain.begin() + i - pairSize + 1, subChain.begin() + i + 1);
		insertedCount++;
	}

	for (size_t i = 0; i < mainChain.size(); ++i)
		_vectorSorted[i] = mainChain[i];
}