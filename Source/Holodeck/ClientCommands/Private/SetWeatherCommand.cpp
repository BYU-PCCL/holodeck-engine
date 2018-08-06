// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "SetWeatherCommand.h"

USetWeatherCommand::USetWeatherCommand() {
	//AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	//AWeatherMaster* WeatherMaster = Game->WeatherMaster;
	//WeatherMaster->MakeRain();
	//FunctionMap.insert("hello ", &AWeatherMaster::MakeCloudy);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
}

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
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message 2!"));
	//success = WeatherMaster->MakeRain();
	if (type == "rain") {
		success = WeatherMaster->MakeRain();
		//FunctionMap["hello"]();
	}
	else if (type == "cloudy") 
		success = WeatherMaster->MakeCloudy();
}
