// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "SetWeatherCommand.h"

void USetWeatherCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("SetWeatherCommand::Execute weather change"));

	if (StringParams.size() != 1 || NumberParams.size() != 0) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in USetWeatherCommand. Command not executed."));
		return;
	}

	std::string type = StringParams[0];

	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	AWeatherMaster* WeatherMaster = Game->WeatherMaster;
	
	bool success = false;
	if (type == "rain") {
		success = WeatherMaster->MakeRain();
	}
	else if (type == "cloudy") 
		success = WeatherMaster->MakeCloudy();

	if (!success)
		UE_LOG(LogHolodeck, Error, TEXT("SetWeatherCommand did not behave correctly. There is probably a missing skysphere or skylight."));
}
