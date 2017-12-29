#include "Holodeck.h"
#include "VelocitySensor.h"

UVelocitySensor::UVelocitySensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UVelocitySensor::BeginPlay() {
	Super::BeginPlay();
	//You need to get the pointer to the object the sensor is attached to. 
	Parent = this->GetAttachParent();
	if (Parent != nullptr) {
		CurrentLocation = Parent->GetComponentLocation();
	}
	
}

//TODO(mitch) : Figure out if Unreal has a function to get the absolute velocity of a USceneComponent, instead of manually calculating it each tick. 
FVector UVelocitySensor::GetVelocity() {
	FVector DeltaLocation = CurrentLocation - LocationAtPreviousTick;
	FVector Velocity = DeltaLocation / ChangeInTime;
	return Velocity;
}

void UVelocitySensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		//VelocitySensor needs to know its previous location, current location, and how much time has passed in order to calculate its velocity.
		this->ChangeInTime = DeltaTime;
		this->LocationAtPreviousTick = this->CurrentLocation;
		this->CurrentLocation = Parent->GetComponentLocation();
		FVector Velocity = GetVelocity();
		float* FloatBuffer = static_cast<float*>(Buffer);
		FloatBuffer[0] = Velocity.X;
		FloatBuffer[1] = Velocity.Y;
		FloatBuffer[2] = Velocity.Z;
	}
}
