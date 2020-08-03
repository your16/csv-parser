#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <array>
#include <string>
#include <unordered_map>

class csv_parser
{
private:
	const char _sep = 44;
	const char* _error = "NaN";
	const std::unordered_map<char, std::string(csv_parser::*)(const std::string&, const std::string&)> _funcs =
	{ {42, &csv_parser::mul}, {43, &csv_parser::add}, {45, &csv_parser::sub}, {47, &csv_parser::div} };

	std::unordered_map <std::string, std::string> _cells;
	std::string _view;

public:
	csv_parser() = default;
	csv_parser(const char* file);
	csv_parser(const csv_parser& obj) = delete;
	csv_parser(csv_parser&& obj) noexcept = delete;

	csv_parser& operator = (const csv_parser& obj) = delete;
	csv_parser& operator = (csv_parser&& obj) noexcept = delete;

	~csv_parser() = default;

	void open(const char* file);

	friend std::ostream& operator << (std::ostream& os, const csv_parser& obj);

private:
	bool is_operator(const char ch);
	bool is_adr(const std::string& str);
	bool is_expr(const std::string& str);

	void cache_table(std::ifstream& stream);
	void calc_table();

	std::string& calc_expr(std::string& expr, std::vector<std::string>& prev_exprs);

	std::string add(const std::string& arg1, const std::string& arg2);
	std::string sub(const std::string& arg1, const std::string& arg2);
	std::string mul(const std::string& arg1, const std::string& arg2);
	std::string div(const std::string& arg1, const std::string& arg2);
};