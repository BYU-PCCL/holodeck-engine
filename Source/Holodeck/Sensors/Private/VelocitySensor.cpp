#include "Holodeck.h"
#include "VelocitySensor.h"

UVelocitySensor::UVelocitySensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UVelocitySensor::BeginPlay() {
	Super::BeginPlay();
	//You need to get the pointer to the object the sensor is attached to. 
	Parent = this->GetAttachmentRootActor();

	UnitsPerMeter = GetWorld()->GetWorldSettings()->WorldToMeters;
}

void UVelocitySensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		FVector Velocity = Parent->GetVelocity();
		float* FloatBuffer = static_cast<float*>(Buffer);
		FloatBuffer[0] = Velocity.X / UnitsPerMeter;
		FloatBuffer[1] = Velocity.Y / UnitsPerMeter;
		FloatBuffer[2] = Velocity.Z / UnitsPerMeter;
	}
}
