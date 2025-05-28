#include "PmergeMe.hpp"
#include <deque>
#include <iostream>

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

	std::deque<bool> inserted(subChain.size(), false);
	size_t insertedCount = 0;

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

			size_t maxChunkSearchIndex = jacobsthalCurr + insertedCount;
			size_t totalChunks = mainChain.size() / pairSize;
			if (maxChunkSearchIndex >= totalChunks)
				maxChunkSearchIndex = totalChunks - 1;

			int valueToInsert = subChain[insertIndexSub];
			auto insertionIt = binaryFindInsertion(valueToInsert, maxChunkSearchIndex);

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
	}

	for (int i = (int)subChain.size() - 1; i >= static_cast<int>(pairSize - 1); i -= pairSize)
	{
		if (inserted[i])
			continue;

		size_t maxChunkSearchIndex = jacobsthalCurr + insertedCount;
		size_t totalChunks = mainChain.size() / pairSize;
		if (maxChunkSearchIndex >= totalChunks)
			maxChunkSearchIndex = totalChunks - 1;

		int valueToInsert = subChain[i];
		auto insertionIt = binaryFindInsertion(valueToInsert, maxChunkSearchIndex);

		mainChain.insert(insertionIt, subChain.begin() + i - pairSize + 1, subChain.begin() + i + 1);
		insertedCount++;
	}

	for (size_t i = 0; i < mainChain.size(); ++i)
		_dequeSorted[i] = mainChain[i];
}



// void PmergeMe::dequeSort(unsigned int pairSize) 
// {
//     size_t n = _dequeSorted.size();
//     if (pairSize * 2 > n)
//         return;

//     // Sort pairs in the deque (your existing code)
//     for (size_t i = 0; i + 2 * pairSize - 1 < n; i += 2 * pairSize)
//     {
//         auto firstPairStart = _dequeSorted.begin() + i;
//         auto secondPairStart = firstPairStart + pairSize;

//         int lastOfFirstPair = *(firstPairStart + pairSize - 1);
//         int lastOfSecondPair = *(secondPairStart + pairSize - 1);

//         if (lastOfFirstPair > lastOfSecondPair)
//             std::swap_ranges(firstPairStart, secondPairStart, secondPairStart);
//     }

//     // Recursive call to sort bigger pairs
//     dequeSort(pairSize * 2);

//     // Build mainChain and subChain as before
//     std::deque<int> mainChain;
//     std::deque<int> subChain;

//     auto firstPairStart = _dequeSorted.begin();

//     // Push b1 and all 'a's (first element of first pair plus all second elements)
//     for (size_t i = 0; i < pairSize; ++i)
//         mainChain.push_back(*(firstPairStart + i));

//     // Push remaining 'a's into mainChain
//     for (size_t i = pairSize; i < n; i += pairSize * 2)
//     {
//         if (i + pairSize - 1 >= n)
//             break;
//         auto pairStart = _dequeSorted.begin() + i;
//         for (size_t j = 0; j < pairSize; ++j)
//             mainChain.push_back(*(pairStart + j));
//     }

//     // Push 'b's (starting from second pair)
//     for (size_t i = pairSize * 2; i < n; i += pairSize * 2)
//     {
//         if (i + pairSize - 1 >= n)
//             break;
//         auto pairStart = _dequeSorted.begin() + i;
//         for (size_t j = 0; j < pairSize; ++j)
//             subChain.push_back(*(pairStart + j));
//     }

//     // Custom binary search: find insertion chunk in mainChain by largest element, up to boundary maxChunkIndex
// 	int binarySearchCalls = 0;
//     auto binaryFindInsertionChunk = [&](int valueToInsert, size_t maxChunkIndex) -> typename std::deque<int>::iterator {
//         // Clamp maxChunkIndex so it doesn't go beyond mainChain chunks count
// 		++binarySearchCalls;
//         size_t totalChunks = mainChain.size() / pairSize;
//         if (maxChunkIndex >= totalChunks)
//             maxChunkIndex = totalChunks - 1;

//         size_t left = 0;
//         size_t right = maxChunkIndex;

//         while (left < right)
//         {
//             size_t mid = left + (right - left) / 2;
//             int largestInMidChunk = *(mainChain.begin() + mid * pairSize + pairSize - 1);
//             if (largestInMidChunk > valueToInsert)
//                 right = mid;
//             else
//                 left = mid + 1;
//         }
//         return mainChain.begin() + left * pairSize;
//     };

//     unsigned int jacobsthalIndex = 3;
//     unsigned int jacobsthalPrev = jacobsthalCalc(jacobsthalIndex - 1);
//     unsigned int jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);
//     int insertions = jacobsthalCurr - jacobsthalPrev;

//     std::deque<bool> inserted(subChain.size(), false);
//     int insertedCount = 0;

//     while (insertions > 0)
//     {
//         int indexOffset = 0;
//         while (insertions > 0)
//         {
//             int insertIndexSub = (jacobsthalCurr - 1) * pairSize - 1 - indexOffset;
//             if (insertIndexSub < 0 || insertIndexSub >= (int)subChain.size())
//                 break;

//             // Calculate boundary in chunks for insertion: jacobsthalCurr + insertedCount
// 			size_t maxChunkSearchIndex = std::min(static_cast<size_t>(jacobsthalCurr + insertedCount),
//                                       mainChain.size() / pairSize - 1);
// 			size_t totalChunks = mainChain.size() / pairSize;
// 			if (maxChunkSearchIndex >= totalChunks)
// 				maxChunkSearchIndex = totalChunks - 1;

//             int valueToInsert = subChain[insertIndexSub]; // largest in chunk

//             auto insertionIt = binaryFindInsertionChunk(valueToInsert, maxChunkSearchIndex);
// 			// std::cout << "Main chain before insertion: ";
// 			// for (const auto& val : mainChain)
// 			// 	std::cout << val << " ";
// 			// std::cout << std::endl;
//             mainChain.insert(insertionIt, subChain.begin() + insertIndexSub - pairSize + 1, subChain.begin() + insertIndexSub + 1);
// 			// std::cout << "Inserted chunk: ";
// 			// for (int j = insertIndexSub - pairSize + 1; j <= insertIndexSub; ++j)
// 			// 	std::cout << subChain[j] << " ";
// 			// std::cout << "into mainChain at position: " << (insertionIt - mainChain.begin()) / pairSize << std::endl;
//             inserted[insertIndexSub] = true;
//             insertedCount++;
//             insertions--;
//             indexOffset += pairSize;
//         }
//         jacobsthalIndex++;
//         jacobsthalPrev = jacobsthalCurr;
//         jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);
//         insertions = jacobsthalCurr - jacobsthalPrev;
//     }

//     // Insert remaining chunks backwards
//     for (int i = (int)subChain.size() - 1; i >= static_cast<int>(pairSize - 1); i -= pairSize)
//     {
//         if (inserted[i])
//             continue;

// 		size_t maxChunkSearchIndex = std::min(static_cast<size_t>(jacobsthalCurr + insertedCount),
// 			mainChain.size() / pairSize - 1);
// 		size_t totalChunks = mainChain.size() / pairSize;
// 		if (maxChunkSearchIndex >= totalChunks)
// 			maxChunkSearchIndex = totalChunks - 1;
			

//         int valueToInsert = subChain[i];

//         auto insertionIt = binaryFindInsertionChunk(valueToInsert,  maxChunkSearchIndex);

//         mainChain.insert(insertionIt, subChain.begin() + i - pairSize + 1, subChain.begin() + i + 1);
// 		// std::cout << "Inserted chunk: ";
// 		// for (int j = i - pairSize + 1; j <= i; ++j)
// 		// 	std::cout << subChain[j] << " ";
// 		// std::cout << "into mainChain at position: " << (insertionIt - mainChain.begin()) / pairSize << std::endl;
//     }

//     // Copy back to _dequeSorted
//     for (size_t i = 0; i < mainChain.size(); ++i)
// 	{
//         _dequeSorted[i] = mainChain[i];
// 	}
// 	// std::cout << "Sorted deque: ";
// 	// for (size_t i = 0; i < _dequeSorted.size(); ++i)
// 	// 	std::cout << _dequeSorted[i] << " ";
// 	// std::cout << std::endl;
// 	//std::cout << "Total binary search calls: " << binarySearchCalls << std::endl;

// }





// void PmergeMe::dequeSort(unsigned int pairSize) 
// {
// 	size_t n = _dequeSorted.size();
// 	if (pairSize * 2 > _dequeSorted.size())
// 		return;
// 	for (size_t i = 0; i + 2 * pairSize - 1 < n; i += 2 * pairSize)
// 	{
// 		auto firstPairStart = _dequeSorted.begin() + i; //auto replaces std::deque<int>::iterator here
// 		auto secondPairStart = firstPairStart + pairSize;

// 		int lastOfFirstPair = *(firstPairStart + pairSize - 1); //dereferencing the iterator to get the last element of the first pair
// 		int lastOfSecondPair = *(secondPairStart + pairSize - 1);

// 		if (lastOfFirstPair > lastOfSecondPair) 
// 			std::swap_ranges(firstPairStart, secondPairStart, secondPairStart); 
// 	}

// 	dequeSort(pairSize * 2);

// 	//now we will create the main and sub chains and will put into them the pairSize pairs of numbers
// 	std::deque<int> mainChain;
// 	std::deque<int> subChain;

// 	auto firstPairStart = _dequeSorted.begin(); 
// 	for (size_t i = 0; i < pairSize; ++i)
// 	{
// 		mainChain.push_back(*(firstPairStart + i));
// 	}

// 	//now the as are pushed into the main chain
// 	for (size_t i = pairSize; i - 1 < n; i += pairSize * 2)
// 	{
// 		auto pairStart = _dequeSorted.begin() + i;
// 		if (i + pairSize - 1 >= n)
// 			break;
// 		for (size_t j = 0; j < pairSize; ++j)
// 		{
// 			mainChain.push_back(*(pairStart + j));
// 		}
// 	}

// 	//now the sub chain starting with b2 and then the rest of bs
// 	for (size_t i = pairSize * 2 ; i - 1 < n; i += pairSize * 2)
// 	{
// 		auto pairStart = _dequeSorted.begin() + i;
// 		if (i + pairSize - 1 >= n)
// 			break;
// 		for (size_t j = 0; j < pairSize; ++j)
// 		{
// 			subChain.push_back(*(pairStart + j));
// 		}
// 	}


// 	unsigned int jacobsthalIndex = 3; 
// 	unsigned int jacobsthalPrev = jacobsthalCalc(jacobsthalIndex - 1); 
// 	unsigned int jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);     
// 	int insertions = jacobsthalCurr - jacobsthalPrev;

// 	std::deque<bool> inserted(subChain.size(), false);
// 	while (insertions > 0)
// 	{
// 		if (insertions <= 0)
// 			break;
// 		if ((size_t)insertions > subChain.size())
// 			break;
// 		int indexOffset = 0;
// 		while (insertions > 0)
// 		{
// 			int insertIndexSub = (jacobsthalCurr - 1) * pairSize - 1 - indexOffset;

// 			if (insertIndexSub < 0 || insertIndexSub >= (int)subChain.size())
// 				break;
// 			auto insertionIt = mainChain.end(); // default to end in case nothing is greater
// 			for (auto it = mainChain.begin(); it + pairSize - 1 < mainChain.end(); it += pairSize) {
// 				if (*(it + pairSize - 1) >= subChain[insertIndexSub]) {
// 					insertionIt = it;
// 					break;
// 				}
// 			}
// 			mainChain.insert(insertionIt, subChain.begin() + insertIndexSub - pairSize + 1, subChain.begin() + insertIndexSub + 1);
// 			inserted[insertIndexSub] = true;
// 			insertions--;
// 			indexOffset += pairSize;
// 		}
// 		jacobsthalIndex++;
// 		jacobsthalPrev = jacobsthalCurr;
// 		jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);
// 		insertions = jacobsthalCurr - jacobsthalPrev;
// 		insertions = jacobsthalCurr - jacobsthalPrev;
// 	}
	
// 	for (int i = subChain.size() - 1; i >= static_cast<int>(pairSize - 1); i -= pairSize) 
// 	{
// 		if (inserted[i])
// 			continue;
// 		auto insertionIt = mainChain.end();
// 		for (auto it = mainChain.begin(); it + pairSize - 1 < mainChain.end(); it += pairSize) 
// 		{
// 			if (*(it + pairSize - 1) >= subChain[i]) 
// 			{
// 				insertionIt = it;
// 				break;
// 			}
// 		}
// 		mainChain.insert(insertionIt, subChain.begin() + i - pairSize + 1, subChain.begin() + i + 1);
// 	}

// 	for (size_t i = 0; i < mainChain.size(); ++i)
// 			_dequeSorted[i] = mainChain[i];
// 	return;
// }

