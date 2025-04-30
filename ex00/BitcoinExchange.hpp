/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:02:42 by akovalev          #+#    #+#             */
/*   Updated: 2025/04/30 18:33:17 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <iostream>
#include <fstream>

class BitcoinExchange
{
	private:
		std::map<std::string, double> _ratesData; // map to store date and value pairs
		std::multimap<std::string, double> _inputFileData; // multimap to store date and value pairs
		std::string _filename; // filename of the CSV file
		std::string _inputFilename; // filename of the input file
	
		bool isDateValid(const std::string &date) const;	//validate the date format

	public:
		BitcoinExchange();
		BitcoinExchange(const std::string &filename);	
		~BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);

};