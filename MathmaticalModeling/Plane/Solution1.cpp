#include <assert.h>

#include "Solution1.h"

static void Search(
	size_t PuckIdx,
	const std::vector<Pucks> & Puck,
	const std::vector<Gates> & Gate,
	std::vector<std::pair<Date, Time>> & GatesStates,
	std::vector<bool> & GatesUsed,
	int & MinUsedGateNumber
)
{
	if (PuckIdx == Puck.size())
	{
		int UsedGateNumber = 0;
		for (auto IsUsed : GatesUsed)
		{
			if (IsUsed)
			{
				UsedGateNumber++;
			}
		}

		if (UsedGateNumber < MinUsedGateNumber)
		{
			MinUsedGateNumber = UsedGateNumber;
		}

		return;
	}

	for (size_t GateIdx = 0; GateIdx < Gate.size(); GateIdx++)
	{
		if (CanPlaneLand(Puck[PuckIdx], Gate[GateIdx]))
		{
			auto LandedPlaneDepartureDate = GatesStates[GateIdx].first;
			auto LandedPlaneDepartureTime = GatesStates[GateIdx].second;

			auto ArrivingDate = Puck[PuckIdx].ArrivingDate;
			auto ArrivingTime = Puck[PuckIdx].ArrivingTime;

			int DeltaTime = Minus(ArrivingDate, ArrivingTime, LandedPlaneDepartureDate, LandedPlaneDepartureTime);

			if (DeltaTime >= 45)
			{
				auto OldDepartureDate = GatesStates[GateIdx].first;
				auto OldDepartureTime = GatesStates[GateIdx].second;
				auto OldGateUsed = GatesUsed[GateIdx];

				GatesStates[GateIdx].first = Puck[PuckIdx].DepartureDate;
				GatesStates[GateIdx].second = Puck[PuckIdx].DepartureTime;
				GatesUsed[GateIdx] = true;

				Search(PuckIdx + 1, Puck, Gate, GatesStates, GatesUsed, MinUsedGateNumber);

				GatesStates[GateIdx].first = OldDepartureDate;
				GatesStates[GateIdx].second = OldDepartureTime;
				GatesUsed[GateIdx] = OldGateUsed;
			}
		}
	}

}

int Solution1BruteForce(
	const std::vector<Pucks> & Puck,
	const std::vector<Tickets> & Ticket,
	const std::vector<Gates> & Gate
)
{
	/* [Departure time of the plane] */
	std::vector<std::pair<Date, Time>> GatesStates(Gate.size(), std::make_pair(Date{2018, 1, 1},  Time{ 0, 0 }));
	std::vector<bool> GatesUsed(Gate.size(), false);
	int UsedGateNumber = Gate.size() + 1;
	Search(0, Puck, Gate, GatesStates, GatesUsed, UsedGateNumber);
	return UsedGateNumber;
}

int Solution1Weighted(
	const std::vector<Pucks> & Puck,
	const std::vector<Tickets> & Ticket,
	const std::vector<Gates> & Gate
)
{
	using GateType = std::tuple<PLANE_TYPE, PLANE_TYPE>;
	std::vector<Gates> NarrowGates;
	std::vector<Gates> WideGates;
	
	for (auto & Gate : Gate)
	{
		if (Gate.BodyType == BODY_TYPE::N)
		{
			NarrowGates.push_back(Gate);
		}
		else
		{
			WideGates.push_back(Gate);
		}
	}

	std::map<GateType, std::vector<Gates>> NarrowGateIndex;
	std::map<GateType, std::vector<Gates>> WideGateIndex;
	
	for (auto & Gate : NarrowGates)
	{
		NarrowGateIndex[std::make_tuple(Gate.ArrivingType, Gate.DepartureType)].push_back(Gate);
	}

	for (auto & Gate : WideGates)
	{
		WideGateIndex[std::make_tuple(Gate.ArrivingType, Gate.DepartureType)].push_back(Gate);
	}

	static std::map<GateType, std::map<GateType, float>> GateTypeWeights
	{
		{
			std::make_tuple(PLANE_TYPE::DOMESTIC, PLANE_TYPE::DOMESTIC),
			{
				{ std::make_tuple(PLANE_TYPE::DOMESTIC,			PLANE_TYPE::DOMESTIC),			5.0f },
				{ std::make_tuple(PLANE_TYPE::DOMESTIC,			PLANE_TYPE::ALL),				2.1f },
				{ std::make_tuple(PLANE_TYPE::ALL,				PLANE_TYPE::DOMESTIC),			2.0f },
				{ std::make_tuple(PLANE_TYPE::ALL,				PLANE_TYPE::ALL),				1.0f }
			}
		},
		{
			std::make_tuple(PLANE_TYPE::INTERNATIONAL, PLANE_TYPE::INTERNATIONAL),
			{
				{ std::make_tuple(PLANE_TYPE::INTERNATIONAL,	PLANE_TYPE::INTERNATIONAL),		5.0f },
				{ std::make_tuple(PLANE_TYPE::INTERNATIONAL,	PLANE_TYPE::ALL),				2.1f },
				{ std::make_tuple(PLANE_TYPE::ALL,				PLANE_TYPE::INTERNATIONAL),		2.0f },
				{ std::make_tuple(PLANE_TYPE::ALL,				PLANE_TYPE::ALL),				1.0f }
			}
		},
		{
			std::make_tuple(PLANE_TYPE::DOMESTIC, PLANE_TYPE::INTERNATIONAL),
			{
				{ std::make_tuple(PLANE_TYPE::DOMESTIC,			PLANE_TYPE::ALL),				2.1f },
				{ std::make_tuple(PLANE_TYPE::ALL,				PLANE_TYPE::INTERNATIONAL),		2.0f },
				{ std::make_tuple(PLANE_TYPE::ALL,				PLANE_TYPE::ALL),				1.0f }
			}
		},
		{
			std::make_tuple(PLANE_TYPE::INTERNATIONAL, PLANE_TYPE::DOMESTIC),
			{
				{ std::make_tuple(PLANE_TYPE::INTERNATIONAL,	PLANE_TYPE::ALL),				2.0f },
				{ std::make_tuple(PLANE_TYPE::ALL,				PLANE_TYPE::DOMESTIC),			2.1f },
				{ std::make_tuple(PLANE_TYPE::ALL,				PLANE_TYPE::ALL),				1.0f }
			}
		}
	};

	std::vector<std::pair<Date, Time>> GatesStates(Gate.size(), std::make_pair(Date{ 2018, 1, 1 }, Time{ 0, 0 }));
	std::vector<bool> GatesUsed(Gate.size(), false);

	for (size_t i = 0; i < Puck.size(); i++)
	{
		BODY_TYPE BodyType = GetBodyType(Puck[i].ModelNumber);
		std::map<GateType, std::vector<Gates>> * CurrentGateIndex = nullptr;

		if (BodyType == BODY_TYPE::N)
		{
			CurrentGateIndex = &NarrowGateIndex;
		}
		else
		{
			CurrentGateIndex = &WideGateIndex;
		}

		auto * Weight = &GateTypeWeights[std::make_tuple(Puck[i].ArrivingType, Puck[i].DepartureType)];

		std::map<float, std::vector<Gates>> WeightSortedGates;

		for (auto Iter = Weight->begin(); Iter != Weight->end(); ++Iter)
		{
			if (CurrentGateIndex->find(Iter->first) != CurrentGateIndex->end())
			{
				WeightSortedGates[Iter->second] = CurrentGateIndex->at(Iter->first);
			}
		}

		auto ArrivingDate = Puck[i].ArrivingDate;
		auto ArrivingTime = Puck[i].ArrivingTime;

		int MinDelta = 99999999;

		bool IsFindGate = false;
		for (auto Iter = WeightSortedGates.begin(); Iter != WeightSortedGates.end(); ++Iter)
		{
			auto GateCandidate = Iter->second;
			for (size_t k = 0; k < GateCandidate.size(); k++)
			{
				int GateIndex = GateCandidate[k].Key - 1;

				auto LandedPlaneDepartureDate = GatesStates[GateIndex].first;
				auto LandedPlaneDepartureTime = GatesStates[GateIndex].second;

				int DeltaTime = Minus(ArrivingDate, ArrivingTime, LandedPlaneDepartureDate, LandedPlaneDepartureTime);

				if (DeltaTime != -1)
				{
					MinDelta = DeltaTime < MinDelta ? DeltaTime : MinDelta;
				}

				if (DeltaTime >= 45)
				{
					GatesStates[GateIndex].first = Puck[i].DepartureDate;
					GatesStates[GateIndex].second = Puck[i].DepartureTime;
					GatesUsed[GateIndex] = true;
					IsFindGate = true;
					goto JumpOut;
				}
			}
		}

		JumpOut:

		assert(IsFindGate && "Plane cannot find a gate to park!");
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
