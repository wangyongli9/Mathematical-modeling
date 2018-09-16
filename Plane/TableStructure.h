#pragma once

#include <string>

enum class PLANE_TYPE
{
	INTERNATIONAL,
	DOMESTIC,
	ALL
};

enum class TERMINAL
{
	T, S
};

enum class AREA
{
	NORTH, CENTER, SOUTH, EAST
};

enum class BODY_TYPE
{
	N, W
};

struct Date
{
	int Year;
	int Month;
	int Day;
};

struct Time
{
	int Hour;
	int Minutes;
};

struct Pucks
{
	int Key;
	std::string FerryRecordNumber;
	
	Date ArrivingDate;
	Time ArrivingTime;
	std::string ArrivingPlane;
	PLANE_TYPE ArrivingType;

	std::string ModelNumber;

	Date DepartureDate;
	Time DepartureTime;
	std::string DeparturePlane;
	PLANE_TYPE DepartureType;

	std::string OnlineAirport;
	std::string OfflineAirport;
};

struct Tickets
{
	int Key;
	std::string PassengerRecordNumber;
	int PassengerNumber;

	std::string ArrivingPlane;
	Date ArrivingDate;

	std::string DeparturePlane;
	Date DepartureDate;
};

struct Gates
{
	int Key;
	std::string BoardingGate;
	TERMINAL Terminal;
	AREA Area;
	PLANE_TYPE ArrivingType;
	PLANE_TYPE DepartureType;
	BODY_TYPE BodyType;
};

inline bool MatchType(PLANE_TYPE Plane, PLANE_TYPE Gate)
{
	return Gate == PLANE_TYPE::ALL ? true : Plane == Gate;
}

inline BODY_TYPE GetBodyType(const std::string & ModelNumber)
{
	if (ModelNumber == "332" || 
		ModelNumber == "333" || 
		ModelNumber == "33E" || 
		ModelNumber == "33H" ||
		ModelNumber == "33L" ||
		ModelNumber == "773")
	{
		return BODY_TYPE::W;
	}
	else
	{
		return BODY_TYPE::N;
	}
}

inline bool CanPlaneLand(Pucks Plane, Gates Gate)
{
	BODY_TYPE BodyType = GetBodyType(Plane.ModelNumber);
	if (Gate.BodyType != BodyType)
	{
		return false;
	}

	if (MatchType(Plane.ArrivingType, Gate.ArrivingType) &&
		MatchType(Plane.DepartureType, Gate.DepartureType))
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline int TimeMinus(Time T1, Time T2)
{
	int H1 = T1.Hour, M1 = T1.Minutes, H2 = T2.Hour, M2 = T2.Minutes;

	if (H1 < H2)
	{
		return -1;
	}

	if (H1 == H2)
	{
		if (M1 < M2)
		{
			return -1;
		}
		else
		{
			return M1 - M2;
		}
	}

	return M1 + (H1 - H2) * 60 - M2;
}

inline int Minus(Date Date1, Time Time1, Date Date2, Time Time2)
{
	int Y1 = Date1.Year, Y2 = Date2.Year;
	int M1 = Date1.Month, M2 = Date2.Month;
	int D1 = Date1.Day, D2 = Date2.Day;

	if (Y1 != Y2) { return -1; }
	if (M1 != M2) { return -1; }
	if (D1 < D2) { return -1; }

	if (D1 == D2)
	{
		return TimeMinus(Time1, Time2);
	}
	else
	{
		return (Time1.Hour * 60 + Time1.Minutes) + (D1 - D2) * 24 * 60 - (Time2.Hour * 60 + Time2.Minutes);
	}
}