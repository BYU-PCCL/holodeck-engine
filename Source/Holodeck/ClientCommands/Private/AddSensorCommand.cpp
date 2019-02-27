// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "AddSensorCommand.h"

void UAddSensorCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("v::Add sensor"));

	if (StringParams.size() != 2 || NumberParams.size() != 1) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in v. Command not executed."));
		return;
	}

	AHolodeckGameMode* GameTarget = static_cast<AHolodeckGameMode*>(Target);
	if (GameTarget == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("UCommand::Target is not a UHolodeckGameMode*. UAddSensorCommand::Sensor not added."));
		return;
	}

	UWorld* World = Target->GetWorld();
	if (World == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("RGBCameraRateCommand::Execute found world as nullptr. Sensor not added."));
		return;
	}

	FString AgentName = StringParams[0].c_str();
	FString SensorName = StringParams[1].c_str();
	AHolodeckAgent* Agent = GetAgent(AgentName);

	USceneComponent* createdComp = ConstructObject<USceneComponent>(YourComponentsActualClass, Agent, SensorName);
	if (createdComp)
	{
		createdComp->RegisterComponent();
		createdComp->AttachTo(Agent, SensorName);

		UHolodeckSensor* Sensor = Cast<UHolodeckSensor>(createdComp);
		Sensor->SetAgentAndController(HolodeckController, AgentName);
		Sensor->InitializeSensor();
		Agent->SensorMap.Add(Sensor->SensorName, Sensor);
	}
}
