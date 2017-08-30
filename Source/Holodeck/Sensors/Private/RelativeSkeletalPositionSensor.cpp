// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "RelativeSkeletalPositionSensor.h"

URelativeSkeletalPositionSensor::URelativeSkeletalPositionSensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void URelativeSkeletalPositionSensor::BeginPlay() {
	Super::BeginPlay();

	AActor* Android = this->GetOwner();
	TArray<USkeletalMeshComponent*> Components;
	Android->GetComponents<USkeletalMeshComponent>(Components);
	SkeletalMeshComponent = Components[0];

	//Save arrays of bones and parent bones
	Bones = SkeletalMeshComponent->GetAllSocketNames();
	ParentBones.Reserve(Bones.Num());
	for (int i = 0; i < Bones.Num(); i++) {
		ParentBones.Insert(SkeletalMeshComponent->GetParentBone(Bones[i]), i);
	}
}

void URelativeSkeletalPositionSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	float* FloatBuffer = static_cast<float*>(Buffer);
	for (int i = 0; i < Bones.Num(); i++) {
		FQuat BoneQ = SkeletalMeshComponent->GetBoneQuaternion(Bones[i], EBoneSpaces::WorldSpace);
		FQuat ParentQ = SkeletalMeshComponent->GetBoneQuaternion(ParentBones[i], EBoneSpaces::WorldSpace);
		FQuat Quat = ParentQ.Inverse() * BoneQ;
		FloatBuffer[4 * i] = Quat.X;
		FloatBuffer[4 * i + 1] = Quat.Y;
		FloatBuffer[4 * i + 2] = Quat.Z;
		FloatBuffer[4 * i + 3] = Quat.W;
	}
}
