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

	
	
	
	// jacobsthalInsertion(mainChain, subChain);
	// if (remainder != -1)
	// {
	// 	auto it = std::lower_bound(mainChain.begin(), mainChain.end(), remainder);
	// 	mainChain.insert(it, remainder);
	// }

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