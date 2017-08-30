// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "IMUSensor.h"

UIMUSensor::UIMUSensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

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

		float* FloatBuffer = static_cast<float*>(Buffer);
		FloatBuffer[0] = LinearAccelerationVector.X;
		FloatBuffer[1] = LinearAccelerationVector.Y;
		FloatBuffer[2] = LinearAccelerationVector.Z;
		FloatBuffer[3] = AngularVelocityVector.X;
		FloatBuffer[4] = AngularVelocityVector.Y;
		FloatBuffer[5] = AngularVelocityVector.Z;
	}
}

void UIMUSensor::CalculateAccelerationVector(float DeltaTime) {
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
