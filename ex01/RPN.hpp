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
#include <stack>

class RPN
{
	private:
		std::string _inputString; 
		void validateInput() const;
	public:
		RPN(); 
		RPN(const std::string &inputString); 
		RPN(const RPN &original);
		RPN &operator=(const RPN &original); 
		~RPN(); 
		int calculateRPN() const;
};