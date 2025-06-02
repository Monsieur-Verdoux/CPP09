/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:31:08 by akovalev          #+#    #+#             */
/*   Updated: 2025/05/20 16:31:08 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <chrono>
#include <unordered_set>
#include <algorithm>

class PmergeMe
{
	private:
		std::vector<int> _vectorSorted;
		std::vector<int> _vectorUnsorted; 
		std::deque<int> _dequeSorted;
		std::deque<int> _dequeUnsorted; 
		void validateInput(char **argv, int argc); 
		unsigned int jacobsthalCalc(unsigned int n) const; 
		void vectorSort(unsigned int pairSize); 
		void dequeSort(unsigned int pairSize); 
		std::vector<int>::iterator binaryFindInsertionVector(std::vector<int> &mainChain, int value, size_t maxPairsIndex, unsigned int pairSize);	
		std::deque<int>::iterator binaryFindInsertionDeque(std::deque<int> &mainChain, int value, size_t maxPairsIndex, unsigned int pairSize);
	public:
		PmergeMe(); 
		PmergeMe(char **argv, int argc);
		PmergeMe(const PmergeMe &original);
		PmergeMe &operator=(const PmergeMe &original);
		~PmergeMe();
};