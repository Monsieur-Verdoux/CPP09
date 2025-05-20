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

PmergeMe::PmergeMe(const std::string &inputString) : _inputString(inputString), _argc(0)
{
	validateInput();

	_vecSorted = _vecUnsorted;
	_dequeSorted = _dequeUnsorted;
	_vecSorted = vectorSort(_vecSorted);
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

void PmergeMe::validateInput()
{
	if (_inputString.empty())
		throw std::runtime_error("Error: empty input string.");
	if (_inputString.find_first_not_of("0123456789 ") != std::string::npos)
		throw std::runtime_error("Error: input string contains invalid characters.");
	std::istringstream iss(_inputString);
	std::string token;
	while (iss >> token)
	{
		try 
		{
			int num = std::stoi(token);
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
std::vector<int> PmergeMe::vectorSort(std::vector<int> vec)
{
	//implement the ford johnson merge insertion sort algorithm here
	std::vector<int> mainChain;
	std::vector<int> subChain;
	std::vector<std::pair<int, int>> pairs;
	int remainder = -1;

	if (vec.size() < 2)
		return vec;

	size_t i = 0;
	while (i + 1 < vec.size())
	{
		int first = vec[i];
		int second = vec[i + 1];
		if (first < second)
			pairs.push_back(std::make_pair(first, second));
		else
			pairs.push_back(std::make_pair(second, first));

		i += 2;
	}
	//check for remainder
	if (vec.size() % 2 == 1)
		remainder = vec.back();

	//now that we have the pairs, we can sort them, first we push the biggest number of the pair to the main chain
	for (size_t i = 0; i < pairs.size(); ++i)
		mainChain.push_back(pairs[i].second);
	//std::cout << "Main chain: ";
	// for (size_t i = 0; i < mainChain.size(); ++i)
	// 	std::cout << mainChain[i] << " ";
	// std::cout << std::endl;
	
	for (size_t i = 0; i < pairs.size(); ++i)
		subChain.push_back(pairs[i].first);
	//now the recursive call 
	mainChain = vectorSort(mainChain);
	
	//std::cout << "Sub chain: ";
	// for (size_t i = 0; i < subChain.size(); ++i)
	// 	std::cout << subChain[i] << " ";
	// std::cout << std::endl;
	jacobsthalInsertion(mainChain, subChain);
	if (remainder != -1)
	{
		auto it = std::lower_bound(mainChain.begin(), mainChain.end(), remainder);
		mainChain.insert(it, remainder);
	}
	//std::cout << "Sorted main chain: ";
	// for (size_t i = 0; i < mainChain.size(); ++i)
	// 	std::cout << mainChain[i] << " ";
	// std::cout << std::endl;
	return mainChain;
}

void PmergeMe::jacobsthalInsertion(std::vector<int> &mainChain, std::vector<int> &subChain)
{
	//implement the jacobsthal insertion here, placeholder for now
	for (size_t i = 0; i < subChain.size(); ++i)
	{
		auto it = std::lower_bound(mainChain.begin(), mainChain.end(), subChain[i]);
		mainChain.insert(it, subChain[i]);
	}
}

PmergeMe::PmergeMe() : _inputString(""), _argc(0)
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