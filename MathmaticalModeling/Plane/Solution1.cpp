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
	std::ofstream GateUsageFile("GateUsage.csv", std::ios::out);
	std::ofstream PlaneLandedFile("PlaneLanded.txt", std::ios::out);
	std::ofstream LandedPlaneNumberNFile("LandedPlaneNumberN.csv", std::ios::out);
	std::ofstream LandedPlaneNumberWFile("LandedPlaneNumberW.csv", std::ios::out);
	std::ofstream LandedPlaneRatioNFile("LandedPlaneRatioN.csv", std::ios::out);
	std::ofstream LandedPlaneRatioWFile("LandedPlaneRatioW.csv", std::ios::out);

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
	std::vector<Pucks> BackupGate;

	// For counting...
	int SNum = 0, TNum = 0;
	std::map<int, std::vector<Pucks>> GatesPlanes;
	std::vector<int> TimeUsedGates;

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

				if (DeltaTime >= 45)
				{
					GatesStates[GateIndex].first = Puck[i].DepartureDate;
					GatesStates[GateIndex].second = Puck[i].DepartureTime;
					GatesUsed[GateIndex] = true;
					IsFindGate = true;

					if (DeltaTime > 20000)
					{
						if (Gate[GateIndex].Terminal == TERMINAL::S) SNum++;
						if (Gate[GateIndex].Terminal == TERMINAL::T) TNum++;
					}
					GatesPlanes[GateIndex].push_back(Puck[i]);

					char Buffer[2048];
					sprintf(Buffer, "Puck[%s] landed in Gate[%s].\n", Puck[i].FerryRecordNumber.c_str(), Gate[GateIndex].BoardingGate.c_str());
					PlaneLandedFile << Buffer;

					goto JumpOut;
				}
			}
		}

	JumpOut:

		if (!IsFindGate)
		{
			bool IsFindBackupGate = false;
			for (auto & PuckInBackupGate : BackupGate)
			{
				if (Minus(Puck[i].ArrivingDate, Puck[i].ArrivingTime, PuckInBackupGate.DepartureDate, PuckInBackupGate.DepartureTime) >= 45)
				{
					BackupGate.erase(std::remove(BackupGate.begin(), BackupGate.end(), PuckInBackupGate), BackupGate.end());
					BackupGate.push_back(Puck[i]);
					IsFindBackupGate = true;
					char Buffer[2048];
					sprintf(Buffer, "Puck[%s] landed in a spare backup gate.\n", Puck[i].FerryRecordNumber.c_str());
					PlaneLandedFile << Buffer;
					break;
				}
			}

			if (!IsFindBackupGate)
			{
				BackupGate.push_back(Puck[i]);
				char Buffer[2048];
				sprintf(Buffer, "Puck[%s] landed in a new backup gate.\n", Puck[i].FerryRecordNumber.c_str());
				PlaneLandedFile << Buffer;
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

	for (auto & PlanesInGate : GatesPlanes)
	{
		int Time = 0;
		for (auto & Plane : PlanesInGate.second)
		{
			Time += Plane.LandingTime;
		}
		TimeUsedGates.push_back(Time);
	}

	for (size_t i = 0; i < TimeUsedGates.size(); i++)
	{
		GateUsageFile << Gate[i].BoardingGate << ", " << TimeUsedGates[i] << std::endl;
	}

	char Buffer[2048];
	sprintf(Buffer, "Number of gate in the terminal : %d.\n", UsedGateNumber);
	PlaneLandedFile << Buffer;
	sprintf(Buffer, "Number of backup gate : %zd.\n", BackupGate.size());
	PlaneLandedFile << Buffer;

	Date StartDate = Puck[0].ArrivingDate;
	Time StartTime = Puck[0].ArrivingTime;
	Date EndDate = { 2018, 1, 21 };
	Time EndTime = { 13, 45 };
	Date CurrDate = StartDate;
	Time CurrTime = StartTime;
	std::vector<int> NumberNList;
	std::vector<int> NumberWList;
	std::vector<int> BackupNumberNList;
	std::vector<int> BackupNumberWList;

	for (;CurrTime != EndTime || CurrDate != EndDate;)
	{
		int NumberN = 0, NumberW = 0;
		int BackupNumberN = 0, BackupNumberW = 0;

		for (auto & PlanesInGate : GatesPlanes)
		{
			for (auto & Plane : PlanesInGate.second)
			{
				if (Minus(CurrDate, CurrTime, Plane.DepartureDate, Plane.DepartureTime) == -1 &&
					Minus(CurrDate, CurrTime, Plane.ArrivingDate, Plane.ArrivingTime) > 0)
				{
					if (GetBodyType(Plane.ModelNumber) == BODY_TYPE::N)
					{
						NumberN++;
					}
					else
					{
						NumberW++;
					}
				}
			}
		}

		for (auto & Plane : BackupGate)
		{
			if (Minus(CurrDate, CurrTime, Plane.DepartureDate, Plane.DepartureTime) == -1 &&
				Minus(CurrDate, CurrTime, Plane.ArrivingDate, Plane.ArrivingTime) > 0)
			{
				if (GetBodyType(Plane.ModelNumber) == BODY_TYPE::N)
				{
					BackupNumberN++;
				}
				else
				{
					BackupNumberW++;
				}
			}
		}

		NumberNList.push_back(NumberN);
		NumberWList.push_back(NumberW);
		BackupNumberNList.push_back(BackupNumberN);
		BackupNumberWList.push_back(BackupNumberW);

		float LandedPlaneRatioN = static_cast<float>(NumberN) / static_cast<float>(NumberN + BackupNumberN);
		float LandedPlaneRatioW = static_cast<float>(NumberW) / static_cast<float>(NumberW + BackupNumberW);
		LandedPlaneRatioN = NumberN == 0 ? 0.0f : LandedPlaneRatioN;
		LandedPlaneRatioW = NumberW == 0 ? 0.0f : LandedPlaneRatioW;

		LandedPlaneNumberNFile << CurrDate.Year << "-" << CurrDate.Month << "-" << CurrDate.Day << ", " << CurrTime.Hour << ":" << CurrTime.Minutes << ", " << NumberN << std::endl;
		LandedPlaneNumberWFile << CurrDate.Year << "-" << CurrDate.Month << "-" << CurrDate.Day << ", " << CurrTime.Hour << ":" << CurrTime.Minutes << ", " << NumberW << std::endl;
		LandedPlaneRatioNFile << CurrDate.Year << "-" << CurrDate.Month << "-" << CurrDate.Day << ", " << CurrTime.Hour << ":" << CurrTime.Minutes << ", " << LandedPlaneRatioN << std::endl;
		LandedPlaneRatioWFile << CurrDate.Year << "-" << CurrDate.Month << "-" << CurrDate.Day << ", " << CurrTime.Hour << ":" << CurrTime.Minutes << ", " << LandedPlaneRatioW << std::endl;

		if (CurrTime.Minutes + 5 < 60) { CurrTime.Minutes += 5; }
		else if (CurrTime.Hour + 1 < 24) { CurrTime.Hour += 1; CurrTime.Minutes = 5 - (60 - CurrTime.Minutes); }
		else { CurrDate.Day += 1; CurrTime.Hour = 0; CurrTime.Minutes = CurrTime.Minutes = 5 - (60 - CurrTime.Minutes); }
	}


	GateUsageFile.close();
	PlaneLandedFile.close();
	LandedPlaneNumberNFile.close();
	LandedPlaneNumberWFile.close();
	LandedPlaneRatioNFile.close();
	LandedPlaneRatioWFile.close();

	return UsedGateNumber + BackupGate.size();
}

int Solution1MaxValue(
	const std::vector<Pucks> & Puck,
	const std::vector<Tickets> & Ticket,
	const std::vector<Gates> & Gate
)
{
	std::pair<PLANE_TYPE, PLANE_TYPE> II = std::make_pair(PLANE_TYPE::INTERNATIONAL, PLANE_TYPE::INTERNATIONAL);
	std::pair<PLANE_TYPE, PLANE_TYPE> DD = std::make_pair(PLANE_TYPE::DOMESTIC, PLANE_TYPE::DOMESTIC);
	std::pair<PLANE_TYPE, PLANE_TYPE> ID = std::make_pair(PLANE_TYPE::INTERNATIONAL, PLANE_TYPE::DOMESTIC);
	std::pair<PLANE_TYPE, PLANE_TYPE> DI = std::make_pair(PLANE_TYPE::DOMESTIC, PLANE_TYPE::DOMESTIC);

	int Table[8] = { 0 };

	std::vector<Pucks> IIN;
	std::vector<Pucks> DDN;
	std::vector<Pucks> IDN;
	std::vector<Pucks> DIN;
	std::vector<Pucks> IIW;
	std::vector<Pucks> DDW;
	std::vector<Pucks> IDW;
	std::vector<Pucks> DIW;

	for (auto & Puck : Puck)
	{
		std::pair<PLANE_TYPE, PLANE_TYPE> Type = std::make_pair(Puck.ArrivingType, Puck.DepartureType);

		if (Type == II && GetBodyType(Puck.ModelNumber) == BODY_TYPE::N)
		{
			bool IsFindGate = false;

			for (auto & LandedPuck : IIN)
			{
				if (Minus(Puck.ArrivingDate, Puck.ArrivingTime, LandedPuck.DepartureDate, LandedPuck.DepartureTime) >= 45)
				{
					IIN.erase(std::remove(IIN.begin(), IIN.end(), LandedPuck), IIN.end());
					IsFindGate = true;
					break;
				}
			}

			if (!IsFindGate)
			{
				Table[0]++;
			}

			IIN.push_back(Puck);
		}
		else if (Type == DD && GetBodyType(Puck.ModelNumber) == BODY_TYPE::N)
		{
			bool IsFindGate = false;

			for (auto & LandedPuck : DDN)
			{
				if (Minus(Puck.ArrivingDate, Puck.ArrivingTime, LandedPuck.DepartureDate, LandedPuck.DepartureTime) >= 45)
				{
					DDN.erase(std::remove(DDN.begin(), DDN.end(), LandedPuck), DDN.end());
					IsFindGate = true;
					break;
				}
			}

			if (!IsFindGate)
			{
				Table[1]++;
			}

			DDN.push_back(Puck);
		}
		else if (Type == ID && GetBodyType(Puck.ModelNumber) == BODY_TYPE::N)
		{
			bool IsFindGate = false;

			for (auto & LandedPuck : IDN)
			{
				if (Minus(Puck.ArrivingDate, Puck.ArrivingTime, LandedPuck.DepartureDate, LandedPuck.DepartureTime) >= 45)
				{
					IDN.erase(std::remove(IDN.begin(), IDN.end(), LandedPuck), IDN.end());
					IsFindGate = true;
					break;
				}
			}

			if (!IsFindGate)
			{
				Table[2]++;
			}

			IDN.push_back(Puck);
		}
		else if (Type == DI && GetBodyType(Puck.ModelNumber) == BODY_TYPE::N)
		{
			bool IsFindGate = false;

			for (auto & LandedPuck : DIN)
			{
				if (Minus(Puck.ArrivingDate, Puck.ArrivingTime, LandedPuck.DepartureDate, LandedPuck.DepartureTime) >= 45)
				{
					DIN.erase(std::remove(DIN.begin(), DIN.end(), LandedPuck), DIN.end());
					IsFindGate = true;
					break;
				}
			}

			if (!IsFindGate)
			{
				Table[3]++;
			}

			DIN.push_back(Puck);
		}
		else if (Type == II && GetBodyType(Puck.ModelNumber) == BODY_TYPE::W)
		{
			bool IsFindGate = false;

			for (auto & LandedPuck : IIW)
			{
				if (Minus(Puck.ArrivingDate, Puck.ArrivingTime, LandedPuck.DepartureDate, LandedPuck.DepartureTime) >= 45)
				{
					IIW.erase(std::remove(IIW.begin(), IIW.end(), LandedPuck), IIW.end());
					IsFindGate = true;
					break;
				}
			}

			if (!IsFindGate)
			{
				Table[4]++;
			}
	
			IIW.push_back(Puck);
		}
		else if (Type == DD && GetBodyType(Puck.ModelNumber) == BODY_TYPE::W)
		{
			bool IsFindGate = false;

			for (auto & LandedPuck : DDW)
			{
				if (Minus(Puck.ArrivingDate, Puck.ArrivingTime, LandedPuck.DepartureDate, LandedPuck.DepartureTime) >= 45)
				{
					DDW.erase(std::remove(DDW.begin(), DDW.end(), LandedPuck), DDW.end());
					IsFindGate = true;
					break;
				}
			}

			if (!IsFindGate)
			{
				Table[5]++;
			}

			DDW.push_back(Puck);
		}
		else if (Type == ID && GetBodyType(Puck.ModelNumber) == BODY_TYPE::W)
		{
			bool IsFindGate = false;

			for (auto & LandedPuck : IDW)
			{
				if (Minus(Puck.ArrivingDate, Puck.ArrivingTime, LandedPuck.DepartureDate, LandedPuck.DepartureTime) >= 45)
				{
					IDW.erase(std::remove(IDW.begin(), IDW.end(), LandedPuck), IDW.end());
					IsFindGate = true;
					break;
				}
			}

			if (!IsFindGate)
			{
				Table[6]++;
			}

			IDW.push_back(Puck);
		}
		else if (Type == DI && GetBodyType(Puck.ModelNumber) == BODY_TYPE::W)
		{
			bool IsFindGate = false;

			for (auto & LandedPuck : DIW)
			{
				if (Minus(Puck.ArrivingDate, Puck.ArrivingTime, LandedPuck.DepartureDate, LandedPuck.DepartureTime) >= 45)
				{
					DIW.erase(std::remove(DIW.begin(), DIW.end(), LandedPuck), DIW.end());
					IsFindGate = true;
					break;
				}
			}

			if (!IsFindGate)
			{
				Table[7]++;
			}

			DIW.push_back(Puck);
		}
	}

	int size = 0;
	return size;
}
