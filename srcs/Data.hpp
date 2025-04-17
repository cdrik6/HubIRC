#include <iostream>
#include <vector>
#include <string>

class Data
{
	private:
		std::vector<std::string> _subjects;
		std::vector<std::string> _verbs;
		std::vector<std::string> _co;
		std::vector<std::string> _cc;
		std::vector<std::string> _forbidden_words;
	
	public:
		Data();
		~Data();
		void loadData(std::string csv_name, std::vector<std::string> vec);
};