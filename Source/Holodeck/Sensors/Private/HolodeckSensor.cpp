// Written by joshgreaves.

#include "Holodeck.h"
#include "HolodeckSensor.h"

UHolodeckSensor::UHolodeckSensor() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	bOn = true;
}

// Called when the game starts
void UHolodeckSensor::BeginPlay() {
	Super::BeginPlay();

	Controller = (AHolodeckPawnController*)(this->GetAttachmentRootActor()->GetInstigator()->Controller);

	if (bOn && Controller != nullptr) {
		AHolodeckAgent* Agent = (AHolodeckAgent*)(Controller->GetPawn());
		if (Agent != nullptr)
			AgentName = Agent->AgentName;
		SensorName = GetDataKey();

		UE_LOG(LogTemp, Warning, TEXT("Getting buffer of size %d"), GetNumItems() * GetItemSize());
		Buffer = Controller->Subscribe(AgentName, SensorName, GetNumItems(), GetItemSize());
	}
}

void UHolodeckSensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bOn && Buffer != nullptr)
		TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}
