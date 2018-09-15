#include "Solution1.h"

int Solution1(
	const std::vector<Pucks> & Puck,
	const std::vector<Tickets> & Ticket,
	const std::vector<Gates> & Gate
)
{
	/*[Departure time of the plane]*/
	std::vector<std::pair<Date, Time>> GatesStates(Gate.size(), std::make_pair(Date{2018, 1, 1},  Time{ 0, 0 }));
	std::vector<bool> GatesUsed(Gate.size(), false);

	for (size_t i = 0; i < Puck.size(); i++)
	{
		for (size_t j = 0; j < Gate.size(); j++)
		{
			if (CanPlaneLand(Puck[i], Gate[j]))
			{
				auto LandedPlaneDepartureDate = GatesStates[j].first;
				auto LandedPlaneDepartureTime = GatesStates[j].second;

				auto ArrivingDate = Puck[i].ArrivingDate;
				auto ArrivingTime = Puck[i].ArrivingTime;

				if (DateMinus(ArrivingDate, LandedPlaneDepartureDate) != -1 &&
					TimeMinus(ArrivingTime, LandedPlaneDepartureTime) != -1)
				{
					GatesStates[j].first = Puck[i].DepartureDate;
					GatesStates[j].second = Puck[i].DepartureTime;
					GatesUsed[j] = true;
					break;
				}
			}
		}
	}

	int UsedGateNumber = 0;
	for (auto IsUsed : GatesUsed)
	{
		if (IsUsed)
		{
			UsedGateNumber++;
		}
	}

	return UsedGateNumber;
}
