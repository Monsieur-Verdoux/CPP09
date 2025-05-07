/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:08:17 by akovalev          #+#    #+#             */
/*   Updated: 2025/05/07 18:08:17 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN() : _inputString("") 
{
}

void RPN::validateInput() const
{
	if (_inputString.empty())
		throw std::runtime_error("Error: empty input string.");
	if (_inputString.find_first_not_of("0123456789+-*/ ") != std::string::npos)
		throw std::runtime_error("Error: input string contains invalid characters.");
	if (_inputString.find_first_of("+-*/") == std::string::npos)
		throw std::runtime_error("Error: input string does not contain any operators.");
	if (_inputString.find_first_of("0123456789") == std::string::npos)
		throw std::runtime_error("Error: input string does not contain any operands.");
}

int RPN::calculateRPN() const
{
	std::stack<int> rpnStack;
	std::istringstream iss(_inputString);
	std::string token;
	while (iss >> token)
	{
		if (token.length() != 1)
			throw std::runtime_error("Error: only single digit numbers and operators are allowed.");
		if (std::isdigit(token[0]))
			rpnStack.push(std::stoi(token));
		else
		{
			if (rpnStack.size() < 2)
				throw std::runtime_error("Error: the stack does not contain enough operands for the operation.");
			int y = rpnStack.top();
			rpnStack.pop();
			int x = rpnStack.top();
			rpnStack.pop();
			int result;
			switch (token[0])
			{
				case '+':
					result = x + y;
					break;
				case '-':
					result = x - y;
					break;
				case '*':
					result = x * y;
					break;
				case '/':
					if (y == 0)
						throw std::runtime_error("Error: division by zero.");
					result = x / y;
					break;
				default:
					throw std::runtime_error("Error: invalid operator.");
			}
			rpnStack.push(result); // push the result back to the stack
		}
	}
	// Check if there is exactly one element left in the stack
	if (rpnStack.size() != 1)
		throw std::runtime_error("Error: the stack does not contain exactly one element after evaluation.");
	return rpnStack.top(); // return the result
}

RPN::RPN(const std::string &inputString) : _inputString(inputString)
{
	validateInput(); 
}

RPN::RPN(const RPN &original)
{
	(void)original;
}

RPN &RPN::operator=(const RPN &original) 
{
	(void)original;
	return *this;
}

RPN::~RPN() 
{
}

