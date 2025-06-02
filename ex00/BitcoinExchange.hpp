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
#include <stdexcept>
#include <sstream>

class BitcoinExchange
{
	private:
		std::map<std::string, float> _dataFileMap; // map to store date and exchange rate pairs
		std::ifstream _inputFile; // input file stream
		std::ifstream _dataFile; // file stream for the CSV file
	
		bool isDateValid(const std::string &date) const;
		void parseDataFile(); //parse the CSV file and store the data in the map
		void parseInputAndPrint(); 
		void calculateExchangeRate(std::string date, float value); //calculate the exchange rate for each date in the input file
		float improvedStof(const std::string &str, bool& valid) const; //convert string to float with error handling
	public:
		BitcoinExchange();
		BitcoinExchange(const std::string &inputFilename);	
		~BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);

};