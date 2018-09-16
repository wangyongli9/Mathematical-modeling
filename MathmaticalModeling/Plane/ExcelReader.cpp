#include "ExcelReader.h"

std::vector<std::vector<std::string>> ExcelReader::Read(const std::string & Filename)
{
	std::ifstream In(Filename, std::ios::in);

	if (!In)
	{
		return std::vector<std::vector<std::string>>();
	}

	std::vector<std::vector<std::string>> Table;
	char Buffer[2048];

	while (In)
	{
		In.getline(Buffer, 2048);
		char* Ptr = std::strtok(Buffer, ",");
		std::vector<std::string> Row;
		while (Ptr != nullptr)
		{
			Row.push_back(Ptr);
			Ptr = std::strtok(nullptr, ",");
		}
		if (Row.size() != 0)
		{
			Table.push_back(Row);
		}
	}

	return Table;
}

std::vector<Pucks> ExcelReader::ParsePucks(std::vector<std::vector<std::string>> & Table)
{
	std::vector<Pucks> Data;

	if (Table.size() == 0 || Table[0].size() != 12)
	{
		return Data;
	}

	for (auto & Row : Table)
	{
		Pucks Puck;

		Puck.Key = atoi(Row[0].substr(2).c_str());
		Puck.FerryRecordNumber = Row[0];

		Puck.ArrivingDate.Day = atoi(Row[1].substr(0, 2).c_str());
		Puck.ArrivingDate.Month = 1;
		Puck.ArrivingDate.Year = 2018;

		Puck.ArrivingTime.Hour = atoi(Row[2].substr(0, 2).c_str());
		Puck.ArrivingTime.Minutes = atoi(Row[2].substr(3, 2).c_str());

		Puck.ArrivingPlane = Row[3];

		Puck.ArrivingType = Row[4] == "D" ? PLANE_TYPE::DOMESTIC : PLANE_TYPE::INTERNATIONAL;

		Puck.ModelNumber = Row[5];

		Puck.DepartureDate.Day = atoi(Row[6].substr(0, 2).c_str());
		Puck.DepartureDate.Month = 1;
		Puck.DepartureDate.Year = 2018;

		Puck.DepartureTime.Hour = atoi(Row[7].substr(0, 2).c_str());
		Puck.DepartureTime.Minutes = atoi(Row[7].substr(3, 2).c_str());

		Puck.DeparturePlane = Row[8];

		Puck.DepartureType = Row[9] == "D" ? PLANE_TYPE::DOMESTIC : PLANE_TYPE::INTERNATIONAL;

		Puck.OnlineAirport = Row[10];
		Puck.OfflineAirport = Row[11];

		Puck.LandingTime = Minus(Puck.DepartureDate, Puck.DepartureTime, Puck.ArrivingDate, Puck.ArrivingTime);

		Data.push_back(Puck);
	}

	return Data;
}

std::vector<Tickets> ExcelReader::ParseTickets(std::vector<std::vector<std::string>>& Table)
{
	std::vector<Tickets> Data;

	if (Table.size() == 0 || Table[0].size() != 6)
	{
		return Data;
	}

	for (auto & Row : Table)
	{
		Tickets Ticket;

		Ticket.Key = atoi(Row[0].substr(1).c_str());
		Ticket.PassengerRecordNumber = Row[0];

		Ticket.PassengerNumber = atoi(Row[1].c_str());

		Ticket.ArrivingPlane = Row[2];

		Ticket.ArrivingDate.Day = atoi(Row[3].substr(0, 2).c_str());
		Ticket.ArrivingDate.Month = 1;
		Ticket.ArrivingDate.Year = 2018;

		Ticket.DeparturePlane = Row[4];

		Ticket.DepartureDate.Day = atoi(Row[5].substr(0, 2).c_str());
		Ticket.DepartureDate.Month = 1;
		Ticket.DepartureDate.Year = 2018;

		Data.push_back(Ticket);
	}

	return Data;
}

std::vector<Gates> ExcelReader::ParseGates(std::vector<std::vector<std::string>> & Table)
{
	std::vector<Gates> Data;

	if (Table.size() == 0 || Table[0].size() != 6)
	{
		return Data;
	}

	for (auto & Row : Table)
	{
		Gates Gate;

		Gate.Key = atoi(Row[0].substr(1).c_str());

		if (Row[0].substr(0, 1) == "S")
		{
			Gate.Key += 28;
		}

		Gate.BoardingGate = Row[0];

		Gate.Terminal = Row[1] == "T" ? TERMINAL::T : TERMINAL::S;

		if (Row[2] == "North")
		{
			Gate.Area = AREA::NORTH;
		}
		else if (Row[2] == "Center")
		{
			Gate.Area = AREA::CENTER;
		}
		else if (Row[2] == "South")
		{
			Gate.Area = AREA::SOUTH;
		}
		else if (Row[2] == "East")
		{
			Gate.Area = AREA::EAST;
		}

		Gate.ArrivingType = Row[3] == "D" ? PLANE_TYPE::DOMESTIC : (Row[3] == "I" ? PLANE_TYPE::INTERNATIONAL : PLANE_TYPE::ALL);
		Gate.DepartureType = Row[4] == "D" ? PLANE_TYPE::DOMESTIC : (Row[4] == "I" ? PLANE_TYPE::INTERNATIONAL : PLANE_TYPE::ALL);

		Gate.BodyType = Row[5] == "N" ? BODY_TYPE::N : BODY_TYPE::W;

		Data.push_back(Gate);
	}

	return Data;
}
