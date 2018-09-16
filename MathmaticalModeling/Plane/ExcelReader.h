#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "TableStructure.h"

class ExcelReader
{
public:
	static std::vector<std::vector<std::string>> Read(const std::string & Filename);

	static std::vector<Pucks> ParsePucks(std::vector<std::vector<std::string>> & Table);
	static std::vector<Tickets> ParseTickets(std::vector<std::vector<std::string>> & Table);
	static std::vector<Gates> ParseGates(std::vector<std::vector<std::string>> & Table);
};