// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "DayTimeCommand.h"

void UDayTimeCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("DayTimeCommand::Execute change day time"));

	if (StringParams.size() != 0 || NumberParams.size() != 1) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in UDayTimeCommand. Command not executed."));
		return;
	}

	int32 hour = (int32) NumberParams[0];

	if (hour > 23 || hour < 0) {
		UE_LOG(LogHolodeck, Error, TEXT("DayTimeCommand was not given an hour between 0 and 23. Command not executed."));
		return;
	}

	int32 angle = HourToAngle(hour);

	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	AWeatherMaster* WeatherMaster = Game->WeatherMaster;
	bool success = WeatherMaster->ChangeSunHeight(angle);
	if(!success)
		UE_LOG(LogHolodeck, Error, TEXT("DayTimeCommand did not behave correctly. There is probably a missing skysphere or skylight"));
}

int32 UDayTimeCommand::HourToAngle(int32 hour) {
	// Hour to Angle cheat-sheet:
	// 6  =    6am    =  0*
	// 0  =  midnight =  90*
	// 18 =    6pm    =  180*
	// 12 =    noon   =  270*

	hour += hour_offset;
	if (hour > 23) hour -= 24;
	return hour * degrees_per_hour;
}
