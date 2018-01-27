#include "Holodeck.h"
#include "LocationSensor.h"

ULocationSensor::ULocationSensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void ULocationSensor::BeginPlay() {
	Super::BeginPlay();
	//You need to get the pointer to the object you are attached to. 
	Parent = this->GetAttachParent();

	UnitsPerMeter = GetWorld()->GetWorldSettings()->WorldToMeters;
}

void ULocationSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		FVector Location = Parent->GetComponentLocation();
		float* FloatBuffer = static_cast<float*>(Buffer);
		FloatBuffer[0] = Location.X / UnitsPerMeter;
		FloatBuffer[1] = Location.Y / UnitsPerMeter;
		FloatBuffer[2] = Location.Z / UnitsPerMeter;
	}
}
