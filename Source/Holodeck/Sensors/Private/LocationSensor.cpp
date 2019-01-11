#include "Holodeck.h"
#include "LocationSensor.h"

ULocationSensor::ULocationSensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void ULocationSensor::InitializeSensor() {
	Super::InitializeSensor();
	//You need to get the pointer to the object you are attached to. 
	Parent = this->GetAttachParent();
}

void ULocationSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	//check if your parent pointer is valid, and if the sensor is on. Then get the location and buffer, then send the location to the buffer. 
	if (Parent != nullptr && bOn) {
		FVector Location = Parent->GetComponentLocation();
		float* FloatBuffer = static_cast<float*>(Buffer);
		FloatBuffer[0] = Location.X / UnitsPerMeter;
		FloatBuffer[1] = Location.Y / UnitsPerMeter;
		FloatBuffer[2] = Location.Z / UnitsPerMeter;
	}
}
