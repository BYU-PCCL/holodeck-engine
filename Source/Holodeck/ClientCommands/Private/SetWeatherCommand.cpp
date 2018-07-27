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

	if (type != "Rain") {
		UE_LOG(LogHolodeck, Error, TEXT("SetWeatherCommand was not given a valid weather type. Command not executed."));
		return;
	}
	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	bool success = Game->MakeRain();
}
