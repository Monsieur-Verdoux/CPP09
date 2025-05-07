/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:49:11 by akovalev          #+#    #+#             */
/*   Updated: 2025/05/07 18:49:11 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: please provide a single string as an argument." << std::endl;
		return 1;
	}
	try
	{
		RPN rpn(argv[1]);
		int result = rpn.calculateRPN();
		std::cout << "Result: " << result << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "Error: unknown error occurred." << std::endl;
		return 1;
	}
	return 0;
}