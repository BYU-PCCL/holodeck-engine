// Written by joshgreaves.

#include "Holodeck.h"
#include "HolodeckSensor.h"

UHolodeckSensor::UHolodeckSensor() {
	PrimaryComponentTick.bCanEverTick = true;
	//Sensors should tick after physics is processed, so that the data
	//they collect is current. 
	PrimaryComponentTick.TickGroup = TG_PostPhysics; 
	bOn = true;
}

// Called when the game starts
void UHolodeckSensor::BeginPlay() {
	Super::BeginPlay();

	Controller = (AHolodeckPawnController*)(this->GetAttachmentRootActor()->GetInstigator()->Controller);

	if (bOn && Controller != nullptr) {
		AActor* Agent = (AActor*)(Controller->GetPawn());
		if (Agent != nullptr) {
			IHolodeckAgentInterface* AgentInterface = Cast<IHolodeckAgentInterface>(Agent);
			AgentName = AgentInterface->Execute_GetAgentName(Agent);
		}
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
