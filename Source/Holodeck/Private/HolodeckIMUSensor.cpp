// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckIMUSensor.h"
#include <string>
#include <sstream>

// Sets default values for this component's properties
UHolodeckIMUSensor::UHolodeckIMUSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	OnCalculateCustomPhysics.BindUObject(this, &UHolodeckIMUSensor::SubstepTick);
}

// Called when the game starts
void UHolodeckIMUSensor::BeginPlay()
{
	Super::BeginPlay();

	Controller = (AHolodeckPawnController*)(this->GetAttachmentRootActor()->GetInstigator()->Controller);

	Parent = Cast<UPrimitiveComponent>(this->GetAttachParent());

	RootMesh = Cast<UStaticMeshComponent>(this->GetAttachParent());

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
void UHolodeckIMUSensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if (RootMesh) {
	//	RootMesh->GetBodyInstance()->AddCustomPhysics(OnCalculateCustomPhysics);
	//}

	
	if (Parent != NULL) {
		CalculateAccelerationVector(DeltaTime);
		CalculateAngularVelocityVector();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ERROR: Failed to cast 'this->GetAttachParent()' to UPrimitiveComponent");
	}

	PublishSensorMessage();
	
}

void UHolodeckIMUSensor::SubstepTick(float DeltaTime, FBodyInstance* BodyInstance)
{
	if (Parent != NULL) {
		CalculateAccelerationVector(DeltaTime);
		CalculateAngularVelocityVector();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ERROR: Failed to cast 'this->GetAttachParent()' to UPrimitiveComponent");
	}

	PublishSensorMessage();
}

void UHolodeckIMUSensor::CalculateAccelerationVector(float DeltaTime) {
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

void UHolodeckIMUSensor::CalculateAngularVelocityVector() {
	AngularVelocityVector = Parent->GetPhysicsAngularVelocity();

	AngularVelocityVector.X = -FMath::DegreesToRadians(AngularVelocityVector.X);
	AngularVelocityVector.Y = -FMath::DegreesToRadians(AngularVelocityVector.Y);
	AngularVelocityVector.Z = FMath::DegreesToRadians(AngularVelocityVector.Z);

	AngularVelocityVector = RotationNow.UnrotateVector(AngularVelocityVector); //Rotate from world angles to local angles.
}

FVector UHolodeckIMUSensor::GetAccelerationVector()
{
	return LinearAccelerationVector;
}

FVector UHolodeckIMUSensor::GetAngularVelocityVector()
{
	return AngularVelocityVector;
}

void UHolodeckIMUSensor::PublishSensorMessage() {
	FHolodeckSensorData data = FHolodeckSensorData();
	data.Type = "IMUSensor";

	data.Data =
		"[" + FString::SanitizeFloat(LinearAccelerationVector.X) + // x_accel
		"," + FString::SanitizeFloat(LinearAccelerationVector.Y) + // y_accel
		"," + FString::SanitizeFloat(LinearAccelerationVector.Z) + // z_accel
		"," + FString::SanitizeFloat(AngularVelocityVector.X) +    // roll_vel
		"," + FString::SanitizeFloat(AngularVelocityVector.Y) +    // pitch_vel
		"," + FString::SanitizeFloat(AngularVelocityVector.Z) + "]"; // yaw_vel
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, data.Data);

	Controller->Publish(data);

}
