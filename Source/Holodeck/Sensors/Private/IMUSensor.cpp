// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "IMUSensor.h"
#include <string>
#include <sstream>

// Sets default values for this component's properties
UIMUSensor::UIMUSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UIMUSensor::BeginPlay() {
	Super::BeginPlay();

	// Cache important variables
	Parent = Cast<UPrimitiveComponent>(this->GetAttachParent());

	World = Parent->GetWorld();
	WorldSettings = World->GetWorldSettings(false, false);
	WorldToMetersRatio = WorldSettings->WorldToMeters;
	WorldGravity = WorldSettings->GetGravityZ() / WorldToMetersRatio;

	VelocityThen = FVector();
	VelocityNow = FVector();
	LinearAccelerationVector = FVector();

	AngularVelocityVector = FVector();
}


void UIMUSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		CalculateAccelerationVector(DeltaTime);
		CalculateAngularVelocityVector();

		float* float_buffer = static_cast<float*>(buffer);
		float_buffer[0] = LinearAccelerationVector.X;
		float_buffer[1] = LinearAccelerationVector.Y;
		float_buffer[2] = LinearAccelerationVector.Z;
		float_buffer[3] = AngularVelocityVector.X;
		float_buffer[4] = AngularVelocityVector.Y;
		float_buffer[5] = AngularVelocityVector.Z;
	}
}

void UIMUSensor::CalculateAccelerationVector(float DeltaTime) {
	// Calculate Acceleration Vector
	VelocityThen = VelocityNow;
	VelocityNow = Parent->GetPhysicsLinearVelocity();

	RotationNow = this->GetAttachParent()->GetComponentRotation();

	LinearAccelerationVector = VelocityNow - VelocityThen;
	LinearAccelerationVector /= DeltaTime;

	LinearAccelerationVector += FVector(0.0, 0.0, -WorldGravity * WorldToMetersRatio);
	LinearAccelerationVector = RotationNow.UnrotateVector(LinearAccelerationVector); //changes world axis to local axis
	LinearAccelerationVector /= WorldToMetersRatio;
}

void UIMUSensor::CalculateAngularVelocityVector() {
	AngularVelocityVector = Parent->GetPhysicsAngularVelocity();

	AngularVelocityVector.X = -FMath::DegreesToRadians(AngularVelocityVector.X);
	AngularVelocityVector.Y = -FMath::DegreesToRadians(AngularVelocityVector.Y);
	AngularVelocityVector.Z = FMath::DegreesToRadians(AngularVelocityVector.Z);

	AngularVelocityVector = RotationNow.UnrotateVector(AngularVelocityVector); //Rotate from world angles to local angles.
}

FVector UIMUSensor::GetAccelerationVector() {
	return LinearAccelerationVector;
}

FVector UIMUSensor::GetAngularVelocityVector() {
	return AngularVelocityVector;
}

FString UIMUSensor::GetDataKey() {
	return "IMUSensor";
}

int UIMUSensor::GetNumItems() {
	return 6;
}

int UIMUSensor::GetItemSize() {
	return sizeof(float);
}