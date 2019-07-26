// MIT License (c) 2019 BYU PCCL see LICENSE file

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "SetSensorEnabledCommand.h"

void USetSensorEnabledCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("USetSensorEnabledCommand::Add or remove sensor"));

	if (StringParams.size() != 2 || NumberParams.size() != 1) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in USetSensorEnabledCommand. Command not executed."));
		return;
	}

	AHolodeckGameMode* GameTarget = static_cast<AHolodeckGameMode*>(Target);
	if (GameTarget == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("UCommand::Target is not a UHolodeckGameMode*. USetSensorEnabledCommand::Sensor not switched."));
		return;
	}

	UWorld* World = Target->GetWorld();
	if (World == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("RGBCameraRateCommand::Execute found world as nullptr. Sensor not switched."));
		return;
	}

	FString AgentName = StringParams[0].c_str();
	FString SensorName = StringParams[1].c_str();
	int Enabled = NumberParams[0];

	AHolodeckAgent* Agent = GetAgent(AgentName);
	UHolodeckSensor* Sensor = Agent->SensorMap[SensorName];
	Sensor->bOn = Enabled == 1;
}
