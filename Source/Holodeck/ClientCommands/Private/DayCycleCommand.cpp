// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "DayCycleCommand.h"

void UDayCycleCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("DayCycleCommand::Execute day cycle command"));

	if (StringParams.size() != 1) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in DayCycleCommand. Command not executed."));
		return;
	}

	std::string command = StringParams[0];

	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	AWeatherMaster* WeatherMaster = Game->WeatherMaster;

	if (command == "start") {
		if (NumberParams.size() == 1) {
			int32 day_length = (int32)NumberParams[0];
			WeatherMaster->StartDayCycle(day_length);
		}
		else {
			UE_LOG(LogHolodeck, Error, TEXT("Day length argument not found in DayCycleCommand. Command not executed."));
		}
	}
	else {
		WeatherMaster->StopDayCycle();
	}
}
