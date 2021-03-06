#include <algorithm>
#include "ExcelReader.h"
#include "Solution1.h"
#include "Solution2.h"
#include "Solution3.h"

int main()
{
	const std::string PucksCSVPath = "InputDataPucks.csv";
	const std::string TicketsCSVPath = "InputDataTickets.csv";
	const std::string GateCSVPath = "InputDataGates.csv";

	auto PucksTable = ExcelReader::Read(PucksCSVPath);
	auto TicketsTable = ExcelReader::Read(TicketsCSVPath);
	auto GatesTable = ExcelReader::Read(GateCSVPath);

	auto PuckDatas = ExcelReader::ParsePucks(PucksTable);
	auto TicketDatas = ExcelReader::ParseTickets(TicketsTable);
	auto GateDatas = ExcelReader::ParseGates(GatesTable);

	PuckDatas.erase(std::remove_if(PuckDatas.begin(), PuckDatas.end(), [](Pucks Puck)
	{
		return Puck.ArrivingDate.Day != 20 && Puck.DepartureDate.Day != 20;
	}), PuckDatas.end());

	TicketDatas.erase(std::remove_if(TicketDatas.begin(), TicketDatas.end(), [](Tickets Ticket)
	{
		return Ticket.ArrivingDate.Day != 20 && Ticket.DepartureDate.Day != 20;
	}), TicketDatas.end());

	// int UsedGateNumber1 = Solution1BruteForce(PuckDatas, TicketDatas, GateDatas);
	int UsedGateNumber2 = Solution1Weighted(PuckDatas, TicketDatas, GateDatas);
	// int UsedGateNumber3 = Solution1MaxValue(PuckDatas, TicketDatas, GateDatas);
	Solution2(PuckDatas, TicketDatas, GateDatas);
	Solution3(PuckDatas, TicketDatas, GateDatas);

    return 0;
}

