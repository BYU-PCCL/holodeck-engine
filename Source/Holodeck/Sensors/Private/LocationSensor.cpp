#include "Holodeck.h"
#include "LocationSensor.h"

ULocationSensor::ULocationSensor() {
	PrimaryComponentTick.bCanEverTick = true;
}
void ULocationSensor::BeginPlay() {
	Super::BeginPlay();

	//You need to get the pointer to the object you are attached to. 
	Parent = this->GetAttachParent();
}
FVector ULocationSensor::GetLocationVector() {
	FVector toReturn;
	if (Parent != NULL) {
		toReturn = Parent->GetComponentLocation();
	}
	return toReturn;
}
void ULocationSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

}