// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "PressureSensor.h"

// Sets default values for this component's properties
UPressureSensor::UPressureSensor() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPressureSensor::BeginPlay() {
	Super::BeginPlay();

	Android = Cast<AAndroid>(this->GetOwner());
	InitJointMap();

	//set hit delegate
	FScriptDelegate hitDelegate;
	hitDelegate.BindUFunction(this, TEXT("OnHit"));
	this->GetAttachmentRootActor()->OnActorHit.AddUnique(hitDelegate);

	//assign skeletal mesh component
	TArray<USkeletalMeshComponent*> components;
	this->GetAttachmentRootActor()->GetComponents<USkeletalMeshComponent>(components);
	SkeletalMeshComponent = components[0];
}

// Called every frame
void UPressureSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	float* FloatBuffer = static_cast<float*>(Buffer);
	memcpy(FloatBuffer, PrivateData, GetNumItems() * sizeof(float));
	memset(PrivateData, 0, GetNumItems() * sizeof(float));
}

void UPressureSensor::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	float* FloatBuffer = static_cast<float*>(Buffer);

	FVector HitWorldLocation;
	FVector HitBoneLocation;
	FRotator HitBoneRotation;

	HitWorldLocation = Hit.Location;
	FRotator HitWorldRotation = SkeletalMeshComponent->GetSocketRotation(Hit.BoneName);

	SkeletalMeshComponent->TransformToBoneSpace(Hit.BoneName, HitWorldLocation, HitWorldRotation, HitBoneLocation, HitBoneRotation);
	float force = NormalImpulse.Size();

	if(JointMap.Contains(Hit.BoneName.ToString()))
		AddHitToBuffer(Hit.BoneName.ToString(), HitBoneLocation, force, PrivateData);
}

float* UPressureSensor::AddHitToBuffer(FString BoneName,FVector HitBoneLocation, float force, float* Data) {

	int JointInd = JointMap[BoneName] * 4;

	Data[JointInd] = HitBoneLocation.X;
	Data[JointInd+1] = HitBoneLocation.Y;
	Data[JointInd+2] = HitBoneLocation.Z;
	Data[JointInd+3] = force;

	return Data;
}

void UPressureSensor::InitJointMap() {
	const FName* Joints = Android->Joints;

	for (int JointInd = 0; JointInd < Android->NUM_JOINTS; JointInd++) {
		FString Joint = Joints[JointInd].ToString();
		JointMap.Add(Joint, JointInd);
	}
}
