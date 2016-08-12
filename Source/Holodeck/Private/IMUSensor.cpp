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
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UIMUSensor::BeginPlay()
{
	Super::BeginPlay();

	Controller = (AHolodeckPawnController*)(this->GetAttachmentRootActor()->GetInstigator()->Controller);

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


// Called every frame
void UIMUSensor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Parent != NULL) {
		CalculateAccelerationVector(DeltaTime);
		CalculateAngularVelocityVector();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ERROR: Failed to cast 'this->GetAttachParent()' to UPrimitiveComponent");
	}

	PublishSensorMessage();
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

FVector UIMUSensor::GetAccelerationVector()
{
	return LinearAccelerationVector;
}

FVector UIMUSensor::GetAngularVelocityVector()
{
	return AngularVelocityVector;
}

void UIMUSensor::PublishSensorMessage() {
	FHolodeckSensorData data = FHolodeckSensorData();
	data.Type = "IMUSensor";

	data.Data =
		"[" + FString::SanitizeFloat(LinearAccelerationVector.X) + // x_accel
		"," + FString::SanitizeFloat(LinearAccelerationVector.Y) + // y_accel
		"," + FString::SanitizeFloat(LinearAccelerationVector.Z) + // z_accel
		"," + FString::SanitizeFloat(AngularVelocityVector.X) +    // roll_vel
		"," + FString::SanitizeFloat(AngularVelocityVector.Y) +    // pitch_vel
		"," + FString::SanitizeFloat(AngularVelocityVector.Z) + "]"; // yaw_vel

	Controller->Publish(data);

}