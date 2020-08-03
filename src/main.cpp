#include <iostream>

#include "csv_parser.h"

int main()
{
	std::vector<std::string> tests = { "tdata/test_a.csv", "tdata/test_b.csv", 
										"tdata/test_c.csv", "tdata/test_d.csv" };
	csv_parser parser;
	for (size_t i = 0; i < tests.size(); i++)
	{
		std::cout << "Test " << i + 1 << "\n" << "---------------" << std::endl;
		parser.open(tests[i].c_str());
		std::cout << parser << "\n" << "---------------" << std::endl;
	}
	std::cout << "Finished, desu!" << std::endl;
	std::cin.get();
}
