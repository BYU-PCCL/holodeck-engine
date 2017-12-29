#include "Holodeck.h"
#include "VelocitySensor.h"

UVelocitySensor::UVelocitySensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UVelocitySensor::BeginPlay() {
	Super::BeginPlay();
	//You need to get the pointer to the object the sensor is attached to. 
	Parent = this->GetAttachParent();
}

FVector UVelocitySensor::GetVelocity() {
	AActor* ActorParent = reinterpret_cast<AActor*>(Parent);

	return ActorParent->GetVelocity();
}

void UVelocitySensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		FVector Velocity = GetVelocity();
		float* FloatBuffer = static_cast<float*>(Buffer);
		FloatBuffer[0] = Velocity.X;
		FloatBuffer[1] = Velocity.Y;
		FloatBuffer[2] = Velocity.Z;
	}
}
