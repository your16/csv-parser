#include "csv_parser.h"

std::ostream& operator << (std::ostream& os, const csv_parser& obj)
{
	return os << obj._view;
}

csv_parser::csv_parser(const char* file)
{
	open(file);
}

void csv_parser::open(const char* file)
{
	_cells.clear();
	_view.clear();
	std::ifstream ifs(file);
	if (ifs.good())
	{
		cache_table(ifs);
		calc_table();
	}
	ifs.close();
}

bool csv_parser::is_operator(const char ch)
{
	return _funcs.find(ch) != _funcs.end();
}

bool csv_parser::is_adr(const std::string& str)
{
	return _cells.find(str) != _cells.end();
}

bool csv_parser::is_expr(const std::string& str)
{
	return (!str.empty() && str.front() == '=');
}

void csv_parser::cache_table(std::ifstream& stream)
{
	int row = 0;
	std::string str;
	std::vector<std::string> cell_col;

	while (std::getline(stream, str))
	{
		int col = 0;
		std::string cell_row;
		std::string cell_data;
		std::stringstream ss(str);

		_view.append(str + '\n');
		std::getline(ss, cell_row, _sep);
		while (std::getline(ss, cell_data, _sep))
		{
			if (row)
				_cells.insert_or_assign(cell_col[col] + cell_row, cell_data);
			else
				cell_col.push_back(cell_data);
			col++;
		}
		row++;
	}
};

void csv_parser::calc_table()
{
	std::stringstream ss;
	std::string str;
	for (auto it = _view.begin(); it != _view.end(); it++)
	{
		if (*it != _sep && *it != '\n')
			str.push_back(*it);
		else
		{
			std::vector<std::string> temp;
			if (is_expr(str)) str = calc_expr(str, temp);
			str += *it;
			ss << str;
			str.clear();
		}
	}
	_view = ss.str();
	_view.pop_back();
}

std::string& csv_parser::calc_expr(std::string& expr, std::vector<std::string>& prev_exprs)
{
	char op = 0;
	std::string arg1;
	std::string arg2;

	prev_exprs.push_back(expr);
	if (std::count(prev_exprs.begin(), prev_exprs.end(), expr) > 1)
		return (expr = _error);

	for (auto it = ++expr.begin(); it != expr.end(); it++)
	{
		if (is_operator(*it)) { op = *it; continue; }
		if (!op) arg1 = arg1 + *it;
		if (op) arg2 = arg2 + *it;
	}

	arg1 = (is_expr(_cells[arg1])) ? calc_expr(_cells[arg1], prev_exprs) : _cells[arg1];
	arg2 = (is_expr(_cells[arg2])) ? calc_expr(_cells[arg2], prev_exprs) : _cells[arg2];

	expr = (arg1 == _error || arg2 == _error) ? _error : (*this.*_funcs.at(op)) (arg1, arg2);
	return expr;
}

std::string csv_parser::add(const std::string& arg1, const std::string& arg2)
{
	return std::to_string(std::stoi(arg1) + std::stoi(arg2));
}

std::string csv_parser::sub(const std::string& arg1, const std::string& arg2)
{
	return std::to_string(std::stoi(arg1) - std::stoi(arg2));
}

std::string csv_parser::mul(const std::string& arg1, const std::string& arg2)
{
	return std::to_string(std::stoi(arg1) * std::stoi(arg2));
}

std::string csv_parser::div(const std::string& arg1, const std::string& arg2)
{
	return std::to_string(std::stoi(arg1) / std::stoi(arg2));
}