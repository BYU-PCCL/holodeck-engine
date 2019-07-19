// MIT License (c) 2019 BYU PCCL see LICENSE file

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "RotateSensorCommand.h"

void URotateSensorCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("URotateSensorCommand::Rotate sensor"));

	if (StringParams.size() != 2 || NumberParams.size() != 3) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in URotateSensorCommand. Command not executed."));
		return;
	}

	FString AgentName = StringParams[0].c_str();
	FString SensorName = StringParams[1].c_str();

	AHolodeckAgent* Agent = GetAgent(AgentName);
	UHolodeckSensor* Sensor = Agent->SensorMap[SensorName];

	float RotationPitch = NumberParams[0];
	float RotationYaw = NumberParams[1];
	float RotationRoll = NumberParams[2];

	Sensor->SetRelativeRotation(FRotator(RotationPitch, RotationYaw, RotationRoll));
    
}
