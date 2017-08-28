// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "RelativeSkeletalPositionSensor.h"


// TODO: delete this class entirely in it's own git commit

// Sets default values for this component's properties
URelativeSkeletalPositionSensor::URelativeSkeletalPositionSensor() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URelativeSkeletalPositionSensor::BeginPlay() {
	Super::BeginPlay();

	//set skeletal mesh component
	AActor* Android = this->GetOwner();
	TArray<USkeletalMeshComponent*> components;
	Android->GetComponents<USkeletalMeshComponent>(components);
	SkeletalMeshComponent = components[0];

	//Save arrays of bones and parent bones
	Bones = SkeletalMeshComponent->GetAllSocketNames();
	ParentBones.Reserve(Bones.Num());
	for (int i = 0; i < Bones.Num(); i++) {
		ParentBones.Insert(SkeletalMeshComponent->GetParentBone(Bones[i]), i);
	}
	
}


// Called every frame
void URelativeSkeletalPositionSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	float* float_buffer = static_cast<float*>(buffer);
	for (int i = 0; i < Bones.Num(); i++) {
		FQuat Bone_Q = SkeletalMeshComponent->GetBoneQuaternion(Bones[i], EBoneSpaces::WorldSpace);
		FQuat Parent_Q = SkeletalMeshComponent->GetBoneQuaternion(ParentBones[i], EBoneSpaces::WorldSpace);
		FQuat Quat = Parent_Q.Inverse() * Bone_Q;
		float_buffer[4 * i] = Quat.X;
		float_buffer[4 * i + 1] = Quat.Y;
		float_buffer[4 * i + 2] = Quat.Z;
		float_buffer[4 * i + 3] = Quat.W;
	}
}
