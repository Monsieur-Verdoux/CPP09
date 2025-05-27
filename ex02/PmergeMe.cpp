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

PmergeMe::PmergeMe(char **argv, int argc) : _argc(argc)
{
	validateInput(argv, argc);

	_vecSorted = _vecUnsorted;
	_dequeSorted = _dequeUnsorted;
	vectorSort(1);
	std::cout << "Unsorted vector: ";
	for (size_t i = 0; i < _vecUnsorted.size(); ++i)
		std::cout << _vecUnsorted[i] << " ";
	std::cout << std::endl;
	std::cout << "Sorted vector: ";
	for (size_t i = 0; i < _vecSorted.size(); ++i)
		std::cout << _vecSorted[i] << " ";
	//check that the vector is sorted
	for (size_t i = 0; i < _vecSorted.size() - 1; ++i)
	{
		if (_vecSorted[i] > _vecSorted[i + 1])
			throw std::runtime_error("Error: vector is not sorted.");
	}
	std::cout << std::endl;
}

void PmergeMe::validateInput(char **argv, int argc)
{
	//rewriting it to take argv and argc
	//check for empty input
	if (argc < 2)
		throw std::runtime_error("Error: please provide a string of integers.");
	//check for invalid characters
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error("Error: input string contains invalid characters.");
		//since there should now be only numbers in a sequence and no spaces there will only be one token so no need to call iss >> token
		try 
		{
			int num = std::stoi(arg);
			if (std::find(_vecUnsorted.begin(), _vecUnsorted.end(), num) != _vecUnsorted.end())
				throw std::runtime_error("Error: duplicate numbers are not allowed.");
			_vecUnsorted.push_back(num);
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

void PmergeMe::vectorSort(unsigned int pairSize) 
{
	size_t n = _vecSorted.size();
	if (pairSize * 2 > _vecSorted.size())
		return;
	for (size_t i = 0; i + 2 * pairSize - 1 < n; i += 2 * pairSize)
	{
		auto firstPairStart = _vecSorted.begin() + i; //auto replaces std::vector<int>::iterator here
		auto secondPairStart = firstPairStart + pairSize;

		int lastOfFirstPair = *(firstPairStart + pairSize - 1); //dereferencing the iterator to get the last element of the first pair
		int lastOfSecondPair = *(secondPairStart + pairSize - 1);

		if (lastOfFirstPair > lastOfSecondPair) 
			std::swap_ranges(firstPairStart, secondPairStart, secondPairStart); 
	}

	std::cout << "vecSorted: ";
	for (const auto& val : _vecSorted)
		std::cout << val << " ";
	std::cout << std::endl;

	vectorSort(pairSize * 2);

	//now we will create the main and sub chains and will put into them the pairSize pairs of numbers
	//we will also check for the remainder here
	std::vector<int> mainChain;
	std::vector<int> subChain;

	//first the main chain is initialized with elements which may include multiple integers based on pairSize, which will be elements b1 and the rest of a, while sub chain is initialized with all the b
	// first we push the very first pair of pairSize into the main chain and then we loop putting only every second element (the a's) into the main chain


	//push the first pair of pairSize into the main chain
	auto firstPairStart = _vecSorted.begin(); //auto replaces std::vector<int>::iterator here
	for (size_t i = 0; i < pairSize; ++i)
	{
		mainChain.push_back(*(firstPairStart + i));
	}
	std::cout << "Main chain after first pair: ";
	for (const auto& val : mainChain)
		std::cout << val << " ";
	std::cout << std::endl;


	//now the as are pushed into the main chain
	for (size_t i = pairSize; i - 1 < n; i += pairSize * 2)
	{
		auto pairStart = _vecSorted.begin() + i;
		if (i + pairSize - 1 >= n)
			break;
		for (size_t j = 0; j < pairSize; ++j)
		{
			mainChain.push_back(*(pairStart + j));
		}
	}
	
	std::cout << "Main chain after inserting the rest of as: ";
	for (const auto& val : mainChain)
		std::cout << val << " ";
	std::cout << std::endl;

	//now the sub chain starting with b2 and then the rest of bs
	for (size_t i = pairSize * 2 ; i - 1 < n; i += pairSize * 2)
	{
		auto pairStart = _vecSorted.begin() + i;
		//std::cout << "i is currently: " << i << " and the vector size is: " << n << " and i + pairSize * 2 - 1 is: " << i + pairSize * 2 - 1 << std::endl;
		//std ::cout << "pairStart: " << *pairStart << std::endl;
		if (i + pairSize - 1 >= n)
			break;
		for (size_t j = 0; j < pairSize; ++j)
		{
			subChain.push_back(*(pairStart + j));
		}
	}

	std::cout << "Sub chain: ";	
	for (const auto& val : subChain)
		std::cout << val << " ";
	std::cout << std::endl;	
	
	unsigned int jacobsthalPrev = jacobsthalCalc(2);
	unsigned int jacobsthalCurr = jacobsthalCalc(3);
	int insertions = jacobsthalCurr - jacobsthalPrev;

	while (insertions > 0)
	{
		if (insertions <= 0)
			break;
		if ((size_t)insertions > subChain.size())
			break;
		std::cout << "Insertions: " << insertions << " and the size of subChain is: " << subChain.size() << std::endl;
		int indexOffset = 0;
		while (insertions > 0)
		{
			int insertIndexSub = (jacobsthalCurr - 1) * pairSize - 1 - indexOffset;
			std::cout << "Pair size: " << pairSize << std::endl;
			std::cout << "Insert index sub: " << insertIndexSub << std::endl;
			if (insertIndexSub < 0 || insertIndexSub >= (int)subChain.size())
			{
				std::cout << "Insert index sub is out of bounds, breaking the loop." << std::endl;
				break;
			}
			std::cout << "The biggest number in the inserted chunk is : " << subChain[insertIndexSub] << std::endl;
			//now we find where to insert the subChain element into the mainChain
			//for that we need to look at the biggest numbers each pairSize element in the mainChainbut only up to corresponding index of "a" for the "b" we are inserting
			std:: cout << "Main chain before insertion: ";
			for (const auto& val : mainChain)
				std::cout << val << " ";
			std::cout << std::endl;
			std::cout << "Looking for the position to insert the chunk of subChain at index: " << insertIndexSub << " with value: " << subChain[insertIndexSub] << std::endl;
			//we need to find the position in the mainChain where we can insert the subChain element
			auto insertionIt = mainChain.end(); // default to end in case nothing is greater
			for (auto it = mainChain.begin(); it + pairSize - 1 < mainChain.end(); it += pairSize) {
				if (*(it + pairSize - 1) >= subChain[insertIndexSub]) {
					insertionIt = it;
					break;
				}
			}
			//auto it = std::lower_bound(mainChain.begin(), mainChain.end(), subChain[insertIndexSub]);
			std::cout << "The iterator points to: " << *insertionIt << std::endl;
			//now we insert the whole pairSize chunk of subChain into the mainChain at the found position with offset of pairSize
			//mainChain.insert(it - pairSize + 1, subChain.begin() + insertIndexSub - pairSize + 1, subChain.begin() + insertIndexSub + 1);
			mainChain.insert(insertionIt, subChain.begin() + insertIndexSub - pairSize + 1, subChain.begin() + insertIndexSub + 1);

			std::cout << "Main chain after insertion of the chunk: ";
			for (const auto& val : mainChain)
				std::cout << val << " ";
			std::cout << std::endl;
			insertions--;
			indexOffset += pairSize;
		}
		jacobsthalPrev = jacobsthalCurr;
		jacobsthalCurr = jacobsthalCalc(jacobsthalCurr + 1);
		insertions = jacobsthalCurr - jacobsthalPrev;
		std::cout << "Next jacobsthal number is: " << jacobsthalCurr << std::endl;
		std::cout << "Next insertions count is: " << insertions << std::endl;
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
	// 		std::cout << _vecSorted[i] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	// else
	// 	std::cout << "No remainder" << std::endl;


	// for (size_t i = 0; pairSize * 2 + i < n; ++i)
	// {
	// 	if (i % pairSize == 0 && i != 0)
	// 		std::cout << " ";
	// 	std::cout << _vecSorted[i] << " ";
	// }
	// std::cout << std::endl;

	
	
	
	
	// jacobsthalInsertion(mainChain, subChain);
	// if (remainder != -1)
	// {
	// 	auto it = std::lower_bound(mainChain.begin(), mainChain.end(), remainder);
	// 	mainChain.insert(it, remainder);
	// }

	for (size_t i = 0; i < mainChain.size(); ++i)
			_vecSorted[i] = mainChain[i];
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "Sorted vector after vectorSort: ";
	for (const auto& val : _vecSorted)
		std::cout << val << " ";
	std::cout << std::endl;
	std::cout << "-------------------------------------------" << std::endl;

	return;
}

// std::vector<int> PmergeMe::vectorSort(std::vector<int> vec)
// {
// 	//implement the ford johnson merge insertion sort algorithm here
// 	std::vector<int> mainChain;
// 	std::vector<int> subChain;
// 	std::vector<std::pair<int, int>> pairs;
// 	int remainder = -1;

// 	if (vec.size() < 2)
// 		return vec;

// 	size_t i = 0;
// 	while (i + 1 < vec.size())
// 	{
// 		int first = vec[i];
// 		int second = vec[i + 1];
// 		if (first < second)
// 			pairs.push_back(std::make_pair(first, second));
// 		else
// 			pairs.push_back(std::make_pair(second, first));

// 		i += 2;
// 	}
// 	//check for remainder
// 	if (vec.size() % 2 == 1)
// 		remainder = vec.back();

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
// 	mainChain = vectorSort(mainChain);
	
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

void PmergeMe::jacobsthalInsertion(std::vector<int> &mainChain, std::vector<int> &subChain)
{
	//implement the jacobsthal insertion here, placeholder for now
	for (size_t i = 0; i < subChain.size(); ++i)
	{
		auto it = std::lower_bound(mainChain.begin(), mainChain.end(), subChain[i]);
		mainChain.insert(it, subChain[i]);
	}
}

unsigned int PmergeMe::jacobsthalCalc(unsigned int n) const
{
	//implement the jacobsthal calculation here, placeholder for now
	if (n == 0)
		return 0;
	else if (n == 1)
		return 1;
	else
		return jacobsthalCalc(n - 1) + 2 * jacobsthalCalc(n - 2);
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