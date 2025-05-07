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
		std::string _inputString; 
		void validateInput() const; // validate the input string
	public:
		RPN(); 
		RPN(const std::string &inputString); 
		RPN(const RPN &original); // copy constructor
		RPN &operator=(const RPN &original); 
		~RPN(); 
		int calculateRPN() const; // calculate the RPN expression, const is added to indicate that this method does not modify the object
};