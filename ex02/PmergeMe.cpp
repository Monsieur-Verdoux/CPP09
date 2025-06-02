/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:30:58 by akovalev          #+#    #+#             */
/*   Updated: 2025/05/20 16:30:58 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe(char **argv, int argc)
{
	validateInput(argv, argc);

	auto startVec = std::chrono::high_resolution_clock::now();
	_vectorSorted = _vectorUnsorted;
	vectorSort(1);
	auto endVec = std::chrono::high_resolution_clock::now();
	auto durationVec = std::chrono::duration_cast<std::chrono::microseconds>(endVec - startVec);

	std::cout << "Before: ";
	for (size_t i = 0; i < _vectorUnsorted.size(); ++i)
		std::cout << _vectorUnsorted[i] << " ";
	std::cout << std::endl;
	std::cout << "After: ";

	for (size_t i = 0; i < _vectorSorted.size(); ++i)
		std::cout << _vectorSorted[i] << " ";
	std::cout << std::endl;
	if (!std::is_sorted(_vectorSorted.begin(), _vectorSorted.end()))
		throw std::runtime_error("Error: vector is not sorted.");
	std::cout << "Time to process a range of " << _vectorUnsorted.size() << " elements with std::vector: " << durationVec.count() << " us" << std::endl;

	auto startDeq = std::chrono::high_resolution_clock::now();
	_dequeSorted = _dequeUnsorted;
	dequeSort(1);
	auto endDeq = std::chrono::high_resolution_clock::now();
	auto durationDeq = std::chrono::duration_cast<std::chrono::microseconds>(endDeq - startDeq);
	if (!std::is_sorted(_dequeSorted.begin(), _dequeSorted.end()))
		throw std::runtime_error("Error: deque is not sorted.");
	std::cout << "Time to process a range of " << _dequeUnsorted.size() << " elements with std::deque: " << durationDeq.count() << " us" << std::endl;
}

std::vector<int>::iterator PmergeMe::binaryFindInsertionVector(std::vector<int> &mainChain, int value, size_t maxPairsIndex, unsigned int pairSize)
{
	size_t left = 0;
	size_t right = maxPairsIndex;

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
}

void PmergeMe::vectorSort(unsigned int pairSize) 
{
	size_t n = _vectorSorted.size();
	if (pairSize * 2 > n)
		return;

	//first we recursively sort pairs of elements with size pairSize doubling each time
	
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

	// Now we create the main chain and sub chain from the sorted pairs, with the main chain containing the b1 element and all "a" (bigger) elements, and the sub chain containing the rest of "b" (smaller) elements.

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

	// Now we calculate the Jacobsthal numbers to determine how many elements to insert from the sub chain into the main chain in a specific order.

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
			if (maxChunkSearchIndex >= totalChunks)
				maxChunkSearchIndex = totalChunks;

			int valueToInsert = subChain[insertIndexSub];
			auto insertionIt = binaryFindInsertionVector(mainChain, valueToInsert, maxChunkSearchIndex, pairSize); //custom binary search to find the insertion point

			size_t insertionPos = insertionIt - mainChain.begin();
			size_t boundaryPos = maxChunkSearchIndex * pairSize;
		
			if (insertionPos == boundaryPos) // this is to handle the case when the insertion position is at the boundary of the current chunk so the seach area shrinks
				offset++;  

			mainChain.insert(insertionIt, subChain.begin() + insertIndexSub - pairSize + 1, subChain.begin() + insertIndexSub + 1); // insert the elements from the sub chain into the main chain
			inserted[insertIndexSub] = true;
			insertions--;
			insertedCount++; //tacking how many elements we have inserted so far which affects the next search area
			indexOffset += pairSize;
		}

		jacobsthalIndex++;
		jacobsthalPrev = jacobsthalCurr;
		jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);
		insertions = jacobsthalCurr - jacobsthalPrev;
		offset = 0;
	}

	for (int i = (int)subChain.size() - 1; i >= static_cast<int>(pairSize - 1); i -= pairSize)
	{
		if (inserted[i])
			continue;
		size_t maxChunkSearchIndex = jacobsthalCurr + insertedCount;
		size_t totalChunks = mainChain.size() / pairSize;
		if (maxChunkSearchIndex >= totalChunks)
			maxChunkSearchIndex = totalChunks;
		int valueToInsert = subChain[i];
		auto insertionIt = binaryFindInsertionVector(mainChain, valueToInsert, maxChunkSearchIndex, pairSize); //the remaining elements are inserted in reverse order to maintain the correct order in the main chain

		mainChain.insert(insertionIt, subChain.begin() + i - pairSize + 1, subChain.begin() + i + 1);
		insertedCount++;
	}

	for (size_t i = 0; i < mainChain.size(); ++i)
		_vectorSorted[i] = mainChain[i]; // copy the sorted main chain back to the original vector
}

std::deque<int>::iterator PmergeMe::binaryFindInsertionDeque(std::deque<int> &mainChain, int value, size_t maxPairsIndex, unsigned int pairSize)
{
	size_t left = 0;
	size_t right = maxPairsIndex;

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
}

void PmergeMe::dequeSort(unsigned int pairSize) 
{
	size_t n = _dequeSorted.size();
	if (pairSize * 2 > n)
		return;
	
	for (size_t i = 0; i + 2 * pairSize - 1 < n; i += 2 * pairSize)
	{
		auto firstPairStart = _dequeSorted.begin() + i;
		auto secondPairStart = firstPairStart + pairSize;

		int lastOfFirstPair = *(firstPairStart + pairSize - 1);
		int lastOfSecondPair = *(secondPairStart + pairSize - 1);

		if (lastOfFirstPair > lastOfSecondPair)
			std::swap_ranges(firstPairStart, secondPairStart, secondPairStart);
	}

	dequeSort(pairSize * 2);

	std::deque<int> mainChain;
	std::deque<int> subChain;

	auto firstPairStart = _dequeSorted.begin();
	for (size_t i = 0; i < pairSize; ++i)
		mainChain.push_back(*(firstPairStart + i));

	for (size_t i = pairSize; i - 1 < n; i += pairSize * 2)
	{
		auto pairStart = _dequeSorted.begin() + i;
		if (i + pairSize - 1 >= n)
			break;
		for (size_t j = 0; j < pairSize; ++j)
			mainChain.push_back(*(pairStart + j));
	}

	for (size_t i = pairSize * 2; i - 1 < n; i += pairSize * 2)
	{
		auto pairStart = _dequeSorted.begin() + i;
		if (i + pairSize - 1 >= n)
			break;
		for (size_t j = 0; j < pairSize; ++j)
			subChain.push_back(*(pairStart + j));
	}

	unsigned int jacobsthalIndex = 3;
	unsigned int jacobsthalPrev = jacobsthalCalc(jacobsthalIndex - 1);
	unsigned int jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);
	int insertions = jacobsthalCurr - jacobsthalPrev;

	std::deque<bool> inserted(subChain.size(), false);
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

			if (maxChunkSearchIndex >= totalChunks)
				maxChunkSearchIndex = totalChunks;
			int valueToInsert = subChain[insertIndexSub];
			auto insertionIt = binaryFindInsertionDeque(mainChain, valueToInsert, maxChunkSearchIndex, pairSize);

			size_t insertionPos = insertionIt - mainChain.begin();
			size_t boundaryPos = maxChunkSearchIndex * pairSize;
		
			if (insertionPos == boundaryPos)
			{
				offset++;  
			}

			mainChain.insert(insertionIt, subChain.begin() + insertIndexSub - pairSize + 1, subChain.begin() + insertIndexSub + 1);

			inserted[insertIndexSub] = true;
			insertions--;
			insertedCount++;
			indexOffset += pairSize;
		}

		jacobsthalIndex++;
		jacobsthalPrev = jacobsthalCurr;
		jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);
		insertions = jacobsthalCurr - jacobsthalPrev;
		offset = 0;
	}

	for (int i = (int)subChain.size() - 1; i >= static_cast<int>(pairSize - 1); i -= pairSize)
	{
		if (inserted[i])
			continue;

		size_t maxChunkSearchIndex = jacobsthalCurr + insertedCount;
		size_t totalChunks = mainChain.size() / pairSize;

		if (maxChunkSearchIndex >= totalChunks)
			maxChunkSearchIndex = totalChunks;

		int valueToInsert = subChain[i];
		auto insertionIt = binaryFindInsertionDeque(mainChain, valueToInsert, maxChunkSearchIndex, pairSize);

		mainChain.insert(insertionIt, subChain.begin() + i - pairSize + 1, subChain.begin() + i + 1);
		insertedCount++;
	}

	for (size_t i = 0; i < mainChain.size(); ++i)
		_dequeSorted[i] = mainChain[i];
}

void PmergeMe::validateInput(char **argv, int argc)
{
	if (argc < 2)
		throw std::runtime_error("Error: please provide a string of positive integers.");

	std::unordered_set<int> seen;
	
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error("Error: input string contains invalid characters.");
		try 
		{
			int num = std::stoi(arg);
			if (!seen.insert(num).second)
                throw std::runtime_error("Error: duplicate numbers are not allowed.");
			_vectorUnsorted.push_back(num);
			_dequeUnsorted.push_back(num);
		}
		catch (const std::out_of_range &e)
		{
			throw std::runtime_error("Error: number is greater than int max.");
		}
		catch (const std::invalid_argument &e)
		{
			throw std::runtime_error("Error: invalid argument(not a number).");
		}
	}

}

unsigned int PmergeMe::jacobsthalCalc(unsigned int n) const
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;

	unsigned int prev2 = 0;
	unsigned int prev1 = 1;
	unsigned int current = 0;

	for (unsigned int i = 2; i <= n; ++i)
	{
		current = prev1 + 2 * prev2;
		prev2 = prev1;
		prev1 = current;
	}
	return current;
}

PmergeMe::PmergeMe()
{
	
}

PmergeMe::PmergeMe(const PmergeMe &original) 
{
	(void)original;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &original)
{
	(void)original; 
	return *this;
}

PmergeMe::~PmergeMe() 
{

}