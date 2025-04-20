/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:42:24 by alexandm          #+#    #+#             */
/*   Updated: 2025/04/20 23:03:16 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Data.hpp"
#include <fstream>
#include <sstream>

Data::Data()
{
	loadData("data/subjects.csv", this->_subjects);
	loadData("data/verbs.csv", this->_verbs);
	loadData("data/adj.csv", this->_adj);
	loadData("data/cc.csv", this->_cc);	
	srand(static_cast<unsigned int>(time(NULL)));
}

Data::~Data()
{
}

void trimSpaces(std::string &line)
{
	line.erase(0, line.find_first_not_of(" \t\r"));
	line.erase(line.find_last_not_of(" \t\r") + 1);
}


void Data::loadData(std::string csv_name, std::vector<std::string> &vec)
{
	std::ifstream	dataFile(csv_name.c_str());
	std::string line;

	if (!dataFile)
		throw (std::runtime_error("Data.cpp : Couldn't open file " + csv_name));
	while (std::getline(dataFile, line))
	{
		trimSpaces(line);
		if (line.empty())
			continue;
		vec.push_back(line);
	}
	if (vec.empty())
		throw (std::runtime_error("Data.cpp : the file is empty " + csv_name));

	// std::cout << csv_name << std::endl;
	// for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
	// 	std::cout << *it << std::endl;
}

std::string Data::get_adj(void)
{
	if (_adj.size() != 0)
		return (_adj.at(rand()%_adj.size()));
	return("");
}

std::string Data::get_cc(void)
{
	if (_cc.size() != 0)
		return (_cc.at(rand()%_cc.size()));	
	return("");	
}

std::string Data::get_verbs(void)
{
	if (_verbs.size() != 0)
		return (_verbs.at(rand()%_verbs.size()));
	return("");
}

std::string Data::get_subjects(void)
{
	if (_subjects.size() != 0)
		return (_subjects.at(rand()%_subjects.size()));
	return("");
}