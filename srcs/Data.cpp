#include "Data.hpp"
#include <fstream>
#include <sstream>

Data::Data()
{
	loadData("data/subjects.csv", this->_subjects);
	loadData("data/verbs.csv", this->_verbs);
	loadData("data/cc.csv", this->_co);
	loadData("data/co.csv", this->_cc);
	loadData("data/forbidden_words.csv", this->_forbidden_words);
}

Data::~Data()
{
}

void trimSpaces(std::string &line)
{
	line.erase(0, line.find_first_not_of(" \t\r"));
	line.erase(line.find_last_not_of(" \t\r") + 1);
}


void Data::loadData(std::string csv_name, std::vector<std::string> vec)
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
	// std::cout << csv_name << std::endl;
	// for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
	// 	std::cout << *it << std::endl;
}