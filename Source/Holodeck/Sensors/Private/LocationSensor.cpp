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
	if (Parent != nullptr) {
		toReturn = Parent->GetComponentLocation();
	}
	return toReturn;
}
void ULocationSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		FVector Location = GetLocationVector();
		float* FloatBuffer = static_cast<float*>(Buffer);
		FloatBuffer[0] = Location.X;
		FloatBuffer[1] = Location.Y;
		FloatBuffer[2] = Location.Z;
	}
}