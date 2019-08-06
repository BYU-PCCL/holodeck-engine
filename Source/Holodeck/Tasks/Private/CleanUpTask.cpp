// MIT License (c) 2019 BYU PCCL see LICENSE file

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "CleanUpTask.h"

void UCleanUpTask::InitializeSensor() {
	Super::InitializeSensor();
}

void UCleanUpTask::ParseSensorParms(FString ParmsJson) {
	Super::ParseSensorParms(ParmsJson);
}

void UCleanUpTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	AActor* Target = GetWorld()->GetAuthGameMode();
	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	if (TotalTrash == 0) {
		// By default the total trash is zero, so we must keep updating it until it is not zero
		TotalTrash = (int)Game->GetWorldNum("TotalTrash");
	}
	
	int TrashInCan = (int)Game->GetWorldNum("TrashInCan");
	// If trash is added, the reward will be positive and vice versa
	Reward = TrashInCan - PrevTick_TrashInCan;
	PrevTick_TrashInCan = TrashInCan;

	if (TrashInCan == TotalTrash) {
		Terminal = 1;
	}


	UTaskSensor::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}