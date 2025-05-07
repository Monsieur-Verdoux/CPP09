/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:08:23 by akovalev          #+#    #+#             */
/*   Updated: 2025/05/07 18:08:23 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <stdexcept>
#include <cctype> // for std::isdigit

class RPN
{
	private:
		std::stack<int> _stack; // stack to store the operands
		std::string _inputString; 
		int calculateRPN(); // function to calculate the RPN expression
	public:
		RPN(); 
		RPN(const std::string &inputString); 
		RPN(const RPN &orighinal); // copy constructor
		RPN &operator=(const RPN &original); 
		~RPN(); 
};