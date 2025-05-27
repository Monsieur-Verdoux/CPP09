#include "PmergeMe.hpp"

void PmergeMe::dequeSort(unsigned int pairSize) 
{
	size_t n = _dequeSorted.size();
	if (pairSize * 2 > _dequeSorted.size())
		return;
	for (size_t i = 0; i + 2 * pairSize - 1 < n; i += 2 * pairSize)
	{
		auto firstPairStart = _dequeSorted.begin() + i; //auto replaces std::deque<int>::iterator here
		auto secondPairStart = firstPairStart + pairSize;

		int lastOfFirstPair = *(firstPairStart + pairSize - 1); //dereferencing the iterator to get the last element of the first pair
		int lastOfSecondPair = *(secondPairStart + pairSize - 1);

		if (lastOfFirstPair > lastOfSecondPair) 
			std::swap_ranges(firstPairStart, secondPairStart, secondPairStart); 
	}

	dequeSort(pairSize * 2);

	//now we will create the main and sub chains and will put into them the pairSize pairs of numbers
	//we will also check for the remainder here
	std::deque<int> mainChain;
	std::deque<int> subChain;

	//first the main chain is initialized with elements which may include multiple integers based on pairSize, which will be elements b1 and the rest of a, while sub chain is initialized with all the b
	// first we push the very first pair of pairSize into the main chain and then we loop putting only every second element (the a's) into the main chain


	//push the first pair of pairSize into the main chain
	auto firstPairStart = _dequeSorted.begin(); //auto replaces std::deque<int>::iterator here
	for (size_t i = 0; i < pairSize; ++i)
	{
		mainChain.push_back(*(firstPairStart + i));
	}
//	std::cout << "Main chain after first pair: ";
	//for (const auto& val : mainChain)
	//	std::cout << val << " ";
	//std::cout << std::endl;


	//now the as are pushed into the main chain
	for (size_t i = pairSize; i - 1 < n; i += pairSize * 2)
	{
		auto pairStart = _dequeSorted.begin() + i;
		if (i + pairSize - 1 >= n)
			break;
		for (size_t j = 0; j < pairSize; ++j)
		{
			mainChain.push_back(*(pairStart + j));
		}
	}
	
	//std::cout << "Main chain after inserting the rest of as: ";
//	for (const auto& val : mainChain)
	//	std::cout << val << " ";
	//std::cout << std::endl;

	//now the sub chain starting with b2 and then the rest of bs
	for (size_t i = pairSize * 2 ; i - 1 < n; i += pairSize * 2)
	{
		auto pairStart = _dequeSorted.begin() + i;
		//std::cout << "i is currently: " << i << " and the deque size is: " << n << " and i + pairSize * 2 - 1 is: " << i + pairSize * 2 - 1 << std::endl;
		//std ::cout << "pairStart: " << *pairStart << std::endl;
		if (i + pairSize - 1 >= n)
			break;
		for (size_t j = 0; j < pairSize; ++j)
		{
			subChain.push_back(*(pairStart + j));
		}
	}

//	std::cout << "Sub chain: ";	
	//for (const auto& val : subChain)
	//	std::cout << val << " ";
//	std::cout << std::endl;	
	
	unsigned int jacobsthalIndex = 3; 
	unsigned int jacobsthalPrev = jacobsthalCalc(jacobsthalIndex - 1); // calc(2)
	unsigned int jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);     // calc(3)
	int insertions = jacobsthalCurr - jacobsthalPrev;
//	std::cout << "Initial jacobsthal number is: " << jacobsthalCurr << std::endl;
//	std::cout << "Initial insertions count is: " << insertions << std::endl;
//	std::cout << "Previous jacobsthal number is: " << jacobsthalPrev << std::endl;


	std::deque<bool> inserted(subChain.size(), false);


	while (insertions > 0)
	{
		if (insertions <= 0)
			break;
		if ((size_t)insertions > subChain.size())
			break;
	//	std::cout << "Insertions: " << insertions << " and the size of subChain is: " << subChain.size() << std::endl;
		int indexOffset = 0;
		while (insertions > 0)
		{
			int insertIndexSub = (jacobsthalCurr - 1) * pairSize - 1 - indexOffset;
		//	std::cout << "Pair size: " << pairSize << std::endl;
		//	std::cout << "Insert index sub: " << insertIndexSub << std::endl;
			if (insertIndexSub < 0 || insertIndexSub >= (int)subChain.size())
			{
			//	std::cout << "Insert index sub is out of bounds, breaking the loop." << std::endl;
				break;
			}
		//	std::cout << "The biggest number in the inserted chunk is : " << subChain[insertIndexSub] << std::endl;
			//now we find where to insert the subChain element into the mainChain
			//for that we need to look at the biggest numbers each pairSize element in the mainChainbut only up to corresponding index of "a" for the "b" we are inserting
		//	std:: cout << "Main chain before insertion: ";
		//	for (const auto& val : mainChain)
		//		std::cout << val << " ";
		//	std::cout << std::endl;
		//	std::cout << "Looking for the position to insert the chunk of subChain at index: " << insertIndexSub << " with value: " << subChain[insertIndexSub] << std::endl;
			//we need to find the position in the mainChain where we can insert the subChain element
			auto insertionIt = mainChain.end(); // default to end in case nothing is greater
			for (auto it = mainChain.begin(); it + pairSize - 1 < mainChain.end(); it += pairSize) {
				if (*(it + pairSize - 1) >= subChain[insertIndexSub]) {
					insertionIt = it;
					break;
				}
			}
			//auto it = std::lower_bound(mainChain.begin(), mainChain.end(), subChain[insertIndexSub]);
		//	std::cout << "The iterator points to: " << *insertionIt << std::endl;
			//now we insert the whole pairSize chunk of subChain into the mainChain at the found position with offset of pairSize
			//mainChain.insert(it - pairSize + 1, subChain.begin() + insertIndexSub - pairSize + 1, subChain.begin() + insertIndexSub + 1);
			mainChain.insert(insertionIt, subChain.begin() + insertIndexSub - pairSize + 1, subChain.begin() + insertIndexSub + 1);

		//	std::cout << "Main chain after insertion of the chunk: ";
		//	for (const auto& val : mainChain)
		//		std::cout << val << " ";
		//	std::cout << std::endl;
			inserted[insertIndexSub] = true;

			insertions--;
			indexOffset += pairSize;
		}

		jacobsthalIndex++;
		jacobsthalPrev = jacobsthalCurr;
		jacobsthalCurr = jacobsthalCalc(jacobsthalIndex);
		insertions = jacobsthalCurr - jacobsthalPrev;
		insertions = jacobsthalCurr - jacobsthalPrev;
	//	std::cout << "Next jacobsthal number is: " << jacobsthalCurr << std::endl;
	//	std::cout << "Next insertions count is: " << insertions << std::endl;
	}
	
			for (int i = subChain.size() - 1; i >= static_cast<int>(pairSize - 1); i -= pairSize) 
			{
			if (inserted[i])
				continue;
		
			// Insert chunk ending at index i
			auto insertionIt = mainChain.end();
			for (auto it = mainChain.begin(); it + pairSize - 1 < mainChain.end(); it += pairSize) {
				if (*(it + pairSize - 1) >= subChain[i]) {
					insertionIt = it;
					break;
				}
			}
			mainChain.insert(insertionIt, subChain.begin() + i - pairSize + 1, subChain.begin() + i + 1);
			//std::cout << "after Jacobstal ran out, Main chain after inserting the chunk ending at index " << i << ": ";
			//for (const auto& val : mainChain)
			//	std::cout << val << " ";
			//std::cout << std::endl;
		}
	// int isRemainder = -1;
	// //remainder will also be the size of the pairSize pairs
	// if (n % (pairSize * 2) != 0)
	// 	isRemainder = 1;
	// //print out the remainder which may include multiple numbers based on pairSize so FOR loop is calculated based on pairSize
	//  std::cout << "Remainder: ";
	// if (isRemainder == 1)
	// {
	// 	for (size_t i = n - n % (pairSize * 2); i < n; ++i)
	// 	{
	// 		if (i % pairSize == 0 && i != 0)
	// 			std::cout << " ";
	// 		std::cout << _dequeSorted[i] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	// else
	// 	std::cout << "No remainder" << std::endl;


	// for (size_t i = 0; pairSize * 2 + i < n; ++i)
	// {
	// 	if (i % pairSize == 0 && i != 0)
	// 		std::cout << " ";
	// 	std::cout << _dequeSorted[i] << " ";
	// }
	// std::cout << std::endl;

	
	
	
	
	// jacobsthalInsertion(mainChain, subChain);
	// if (remainder != -1)
	// {
	// 	auto it = std::lower_bound(mainChain.begin(), mainChain.end(), remainder);
	// 	mainChain.insert(it, remainder);
	// }

	for (size_t i = 0; i < mainChain.size(); ++i)
			_dequeSorted[i] = mainChain[i];
	//std::cout << "-------------------------------------------" << std::endl;
	//std::cout << "Sorted deque after dequeSort: ";
	//for (const auto& val : _dequeSorted)
	//	std::cout << val << " ";
//std::cout << std::endl;
	//std::cout << "-------------------------------------------" << std::endl;

	return;
}

// std::deque<int> PmergeMe::dequeSort(std::deque<int> deque)
// {
// 	//implement the ford johnson merge insertion sort algorithm here
// 	std::deque<int> mainChain;
// 	std::deque<int> subChain;
// 	std::deque<std::pair<int, int>> pairs;
// 	int remainder = -1;

// 	if (deque.size() < 2)
// 		return deque;

// 	size_t i = 0;
// 	while (i + 1 < deque.size())
// 	{
// 		int first = deque[i];
// 		int second = deque[i + 1];
// 		if (first < second)
// 			pairs.push_back(std::make_pair(first, second));
// 		else
// 			pairs.push_back(std::make_pair(second, first));

// 		i += 2;
// 	}
// 	//check for remainder
// 	if (deque.size() % 2 == 1)
// 		remainder = deque.back();

// 	//now that we have the pairs, we can sort them, first we push the biggest number of the pair to the main chain
// 	for (size_t i = 0; i < pairs.size(); ++i)
// 		mainChain.push_back(pairs[i].second);
// 	//std::cout << "Main chain: ";
// 	// for (size_t i = 0; i < mainChain.size(); ++i)
// 	// 	std::cout << mainChain[i] << " ";
// 	// std::cout << std::endl;
	
// 	for (size_t i = 0; i < pairs.size(); ++i)
// 		subChain.push_back(pairs[i].first);
// 	//now the recursive call 
// 	mainChain = dequeSort(mainChain);
	
// 	//std::cout << "Sub chain: ";
// 	// for (size_t i = 0; i < subChain.size(); ++i)
// 	// 	std::cout << subChain[i] << " ";
// 	// std::cout << std::endl;
// 	jacobsthalInsertion(mainChain, subChain);
// 	if (remainder != -1)
// 	{
// 		auto it = std::lower_bound(mainChain.begin(), mainChain.end(), remainder);
// 		mainChain.insert(it, remainder);
// 	}
// 	//std::cout << "Sorted main chain: ";
// 	// for (size_t i = 0; i < mainChain.size(); ++i)
// 	// 	std::cout << mainChain[i] << " ";
// 	// std::cout << std::endl;
// 	return mainChain;
// }

void PmergeMe::jacobsthalInsertionDeq(std::deque<int> &mainChain, std::deque<int> &subChain)
{
	//implement the jacobsthal insertion here, placeholder for now
	for (size_t i = 0; i < subChain.size(); ++i)
	{
		auto it = std::lower_bound(mainChain.begin(), mainChain.end(), subChain[i]);
		mainChain.insert(it, subChain[i]);
	}
}