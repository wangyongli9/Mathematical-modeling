#pragma once

#include <vector>
#include <stack>
#include <tuple>
#include <map>
#include <algorithm>
#include <fstream>
#include "TableStructure.h"

int Solution1BruteForce(
	const std::vector<Pucks> & Puck, 
	const std::vector<Tickets> & Ticket, 
	const std::vector<Gates> & Gate
);

int Solution1Weighted(
	const std::vector<Pucks> & Puck,
	const std::vector<Tickets> & Ticket,
	const std::vector<Gates> & Gate
);

int Solution1MaxValue(
	const std::vector<Pucks> & Puck,
	const std::vector<Tickets> & Ticket,
	const std::vector<Gates> & Gate
);