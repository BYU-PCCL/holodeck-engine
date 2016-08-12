// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "RelativeSkeletalPositionSensor.h"


// TODO: delete this class entirely in it's own git commit

// Sets default values for this component's properties
URelativeSkeletalPositionSensor::URelativeSkeletalPositionSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URelativeSkeletalPositionSensor::BeginPlay()
{
	Super::BeginPlay();

	Controller = (AHolodeckPawnController*)(this->GetAttachmentRootActor()->GetInstigator()->Controller);

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
void URelativeSkeletalPositionSensor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FHolodeckSensorData data = FHolodeckSensorData();
	data.Type = "RelativeSkeletalPositionSensor";

	data.Data = getBonesRelativeOffset();

	Controller->Publish(data);
}

FString URelativeSkeletalPositionSensor::getBonesRelativeOffset() 
{

	FString Output = "[";

	UE_LOG(LogTemp, Log, TEXT("Relative"));

	//start at 1 to ignore root that has "None" parent
	for (int i = 1; i < Bones.Num(); i++) {
		Output += "{\"Bone\":\"" + Bones[i].ToString() + "\",";
		Output += "\"Parent_Bone\":\"" + ParentBones[i].ToString() + "\",";
		
		//component space
		//FQuat Quat = SkeletalMeshComponent->GetBoneQuaternion(Bones[i], EBoneSpaces::ComponentSpace);
		
		//local space
		FQuat Bone_Q = SkeletalMeshComponent->GetBoneQuaternion(Bones[i], EBoneSpaces::WorldSpace);
		FQuat Parent_Q = SkeletalMeshComponent->GetBoneQuaternion(ParentBones[i], EBoneSpaces::WorldSpace);
		FQuat Quat = Parent_Q.Inverse() * Bone_Q;
		
		Output += "\"Quaternion\":{\"X\":" + FString::SanitizeFloat(Quat.X) +
			",\"Y\":" + FString::SanitizeFloat(Quat.Y) +
			",\"Z\":" + FString::SanitizeFloat(Quat.Z) +
			",\"W\":" + FString::SanitizeFloat(Quat.W) +
			+ "} },";
	}
	Output.RemoveAt(Output.Len() - 1);
	Output += "]";

	return Output;

}