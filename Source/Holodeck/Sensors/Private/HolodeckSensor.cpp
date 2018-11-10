// Written by joshgreaves.

#include "Holodeck.h"
#include "HolodeckSensor.h"

UHolodeckSensor::UHolodeckSensor() {
	PrimaryComponentTick.bCanEverTick = true;
	//Sensors should tick after physics is processed, so that the data
	//they collect is current. 
	PrimaryComponentTick.TickGroup = TG_PostPhysics; 
	bOn = true;

	Controller = (AHolodeckPawnController*)(this->GetAttachmentRootActor()->GetInstigator()->Controller);
	if (bOn && Controller != nullptr) {
		SensorName = GetDataKey();

		AHolodeckAgent* Agent = (AHolodeckAgent*)(Controller->GetPawn());
		if (Agent != nullptr) {
			AgentName = Agent->AgentName;
			Agent->SensorMap.Add(SensorName, this);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Getting buffer of size %d"), GetNumItems() * GetItemSize());
	Buffer = Controller->Subscribe(AgentName, SensorName, GetNumItems(), GetItemSize());
}

// Called when the game starts
void UHolodeckSensor::InitializeSensor(){
	int i = 0;
}

void UHolodeckSensor::BeginPlay() {
	Super::BeginPlay();
	InitializeSensor();
}

void UHolodeckSensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bOn && Buffer != nullptr)
		TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}
