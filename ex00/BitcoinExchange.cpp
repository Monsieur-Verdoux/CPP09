/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:02:45 by akovalev          #+#    #+#             */
/*   Updated: 2025/04/30 17:02:57 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(const std::string &inputFilename)
{
	_inputFile.open(inputFilename.c_str());
	if (!_inputFile.is_open())
		throw std::runtime_error("Error: could not open input file.");
	_dataFile.open("data.csv");
	if (!_dataFile.is_open())
		throw std::runtime_error("Error: could not open data file.");
	// Parse the data file and store the data in the map
	parseDataFile();
	// Parse the input file and store the data in the multimap
	parseInputFile();
	// Calculate the exchange rate for each date in the input file
	calculateExchangeRate();
}

void BitcoinExchange::parseDataFile()
{
	std::string line;
	//check that the first line is "date,exchange_rate"
	std::getline(_dataFile, line);
	if (line != "date,exchange_rate")
		throw std::runtime_error("Error while parsing CSV file:: invalid data file format.");
	// Parse the data file and store the data in the map
	while (std::getline(_dataFile, line))
	{
		char comma = ',';
		if (line.find(comma) == std::string::npos)
			std::cerr << "Error while parsing CSV file:: no comma delimiter found in line: " << line << std::endl;
		else
		{
			std::string date = line.substr(0, line.find(comma));
			std::string rate = line.substr(line.find(comma) + 1);
			std::optional<float> rateValue;
			try 
			{
				rateValue = std::stof(rate);
			}
			catch (const std::invalid_argument &e)
			{
				std::cerr << "Error while parsing CSV file:: invalid exchange rate value: " << rate << std::endl;
				rateValue = std::nullopt;
				continue;
			}
			catch (const std::out_of_range &e)
			{
				std::cerr << "Error while parsing CSV file:: exchange rate value out of range: " << rate << std::endl;
				rateValue = std::nullopt;
				continue;
			}
			_dataFileMap.insert(std::make_pair(date, rateValue));
		}
	}
	
}

void BitcoinExchange::parseInputFile()
{
	std::string line;
	//check that the first line is "date | value"
	std::getline(_inputFile, line);
	if (line != "date | value")
		throw std::runtime_error("Error: invalid input file format.");
	// Parse the input file and store the data in the multimap
	while (std::getline(_inputFile, line))
	{
		char pipe = '|';
		if (line.find(pipe) == std::string::npos)
			std::cerr << "Error: no pipe delimiter found in line: " << line << std::endl;
		else
		{
			std::string date = line.substr(0, line.find(pipe));
			//removing leading and trailing spaces
			date.erase(0, date.find_first_not_of(" \t"));
			date.erase(date.find_last_not_of(" \t") + 1);
			std::string value = line.substr(line.find(pipe) + 1);
			std::optional<float> valueFloat;
			try 
			{
				valueFloat = std::stof(value);
			}
			catch (const std::invalid_argument &e)
			{
				std::cerr << "Error: invalid value: " << value << std::endl;
				valueFloat = std::nullopt;
				continue;
			}
			catch (const std::out_of_range &e)
			{
				std::cerr << "Error: value out of range: " << value << std::endl;
				valueFloat = std::nullopt;
				continue;
			}
			_inputFileMap.insert(std::make_pair(date, valueFloat));
		}
	}
}

void BitcoinExchange::calculateExchangeRate()
{
	if (_inputFileMap.empty())
	{
		std::cerr << "Error: no data in input file." << std::endl;
		return;
	}
	if (_dataFileMap.empty())
	{
		std::cerr << "Error: no data in data file." << std::endl;
		return;
	}
	for (auto iterator = _inputFileMap.begin(); iterator != _inputFileMap.end(); ++iterator)
	{
		// Check if the date is valid
		if (!isDateValid(iterator->first))
		{
			std::cerr << "Error: bad input: " << iterator->first << std::endl;
			continue;
		}
		// Check if the value is valid
		if (iterator->second < 0 || iterator->second > 1000)
		{
			std::cerr << "Error: too large a number: " << *iterator->second << std::endl;
			continue;
		}
		// Find the closest date in the data file that is less than or equal to the input date
		//
		// Calculate the exchange rate
		
	}
}

bool BitcoinExchange::isDateValid(const std::string &date) const
{
	// Check if the date is in the format YYYY-MM-DD
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	// Check if the year, month, and day are valid numbers
	for (size_t i = 0; i < date.length(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (!isdigit(date[i]))
			return false;
	}
	int year = std::stoi(date.substr(0, 4));
	int month = std::stoi(date.substr(5, 2));
	int day = std::stoi(date.substr(8, 2));
	if (month < 1 || month > 12)
		return false;
	if (day < 1 || day > 31)
		return false;
	if (month == 2)
	{
		if (day > 29)
			return false;
		if (day == 29 && (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0))) //if a year is divisible by 4, it is a leap year, but if it is divisible by 100, it is not a leap year unless it is also divisible by 400
			return false;
	}
	else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
		return false;
	if (year < 2009)
		return false;
	if (year == 2009 && month == 1 && day < 2)
		return false; // The first exchange rate is on 2009-01-02
	return true;
}

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::~BitcoinExchange()
{
	if (_inputFile.is_open())
		_inputFile.close();
	if (_dataFile.is_open())
		_dataFile.close();
}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	*this = other;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
	{
		_dataFileMap = other._dataFileMap;
		_inputFileMap = other._inputFileMap;
	}
	return *this;
}

