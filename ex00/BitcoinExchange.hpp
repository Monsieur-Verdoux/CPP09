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
#include <optional>
#include <stdexcept>
#include <sstream>

//• Each line in this file must use the following format: "date | value".
//• A valid date will always be in the following format: Year-Month-Day.
//• A valid value must be either a float or a positive integer, between 0 and 1000.

class BitcoinExchange
{
	private:
		std::map<std::string, std::optional<float>> _dataFileMap; // map to store date and exchange rate pairs
		std::multimap<std::string, std::optional<float>> _inputFileMap; // multimap to store date and value pairs
		// std::string _dataFilename; // filename of the CSV file
		// std::string _inputFilename; // filename of the input file
		std::ifstream _inputFile; // input file stream
		std::ifstream _dataFile; // file stream for the CSV file
	
		bool isDateValid(const std::string &date) const;	//validate the date format
		void parseDataFile(); //parse the CSV file and store the data in the map
		void parseInputFile(); //parse the input file and store the data in the multimap
		void calculateExchangeRate(); //calculate the exchange rate for each date in the input file

	public:
		BitcoinExchange();
		BitcoinExchange(const std::string &inputFilename);	
		~BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);

};