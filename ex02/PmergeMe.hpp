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
#include <sstream>
#include <stdexcept>
#include <algorithm> // for std::find

class PmergeMe
{
	private:
		int _argc; // number of arguments
		std::vector<int> _vecSorted; // vector to store integers
		std::vector<int> _vecUnsorted; // vector to store integers
		std::deque<int> _dequeSorted; // deque to store integers
		std::deque<int> _dequeUnsorted; // deque to store integers
		void validateInput(char **argv, int argc); // validate the input string
		unsigned int jacobsthalCalc(unsigned int n) const; // calculate the Jacobsthal number
		void jacobsthalInsertion(std::vector<int> &mainChain, std::vector<int> &subChain); // perform the Jacobsthal insertion
		//std::vector<int> vectorSort(std::vector<int> vec); // sort the vector using the Jacobsthal number
		void vectorSort(unsigned int pairSize); // sort the vector using the Jacobsthal number
	public:
		PmergeMe(); // default constructor
		//rewrite the constructor to take argv and argc
		PmergeMe(char **argv, int argc);
		PmergeMe(const PmergeMe &original); // copy constructor
		PmergeMe &operator=(const PmergeMe &original); // assignment operator
		~PmergeMe(); // destructor
};