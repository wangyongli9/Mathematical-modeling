#pragma once

#include <string>

enum PLANE_TYPE
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

struct Pucks
{
	int Key;
	std::string FerryRecordNumber;
	
	struct
	{
		int Year;
		int Month;
		int Day;
	} ArrivingDate;
	struct
	{
		int Hour;
		int Minutes;
	} ArrivingTime;
	std::string ArrivingPlane;
	PLANE_TYPE ArrivingType;

	std::string ModelNumber;

	struct
	{
		int Year;
		int Month;
		int Day;
	} DepartureDate;
	struct
	{
		int Hour;
		int Minutes;
	} DepartureTime;
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
	struct
	{
		int Year;
		int Month;
		int Day;
	} ArrivingDate;

	std::string DeparturePlane;
	struct
	{
		int Year;
		int Month;
		int Day;
	} DepartureDate;
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