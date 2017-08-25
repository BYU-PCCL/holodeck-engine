// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "PressureSensor.h"


// Sets default values for this component's properties
UPressureSensor::UPressureSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPressureSensor::BeginPlay()
{
	Super::BeginPlay();

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
void UPressureSensor::TickSensorComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{/*
	//build json from hitsmap
	FString DataString = "";
	for (auto& BoneHitArray : HitsMap)
	{
		DataString += "{\"" + BoneHitArray.Key + "\":[";
		for (int i = 0; i < BoneHitArray.Value.Num(); i++) {
			DataString += BoneHitArray.Value[i];
			if (i+1 < BoneHitArray.Value.Num())
				DataString += ",";
		}
		DataString += "]},";
	}
	if (DataString.Len() > 0)
		DataString.RemoveAt(DataString.Len() - 1);
	DataString.InsertAt(0, "[");
	DataString += "]";
	ResultData.Data = DataString;

	//Empty the hits map
	HitsMap.Empty();*/
}

void UPressureSensor::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector hit_world_location;
	FVector hit_bone_location;
	FRotator hit_bone_rotation;

	hit_world_location = Hit.Location;
	FRotator hit_world_rotation = SkeletalMeshComponent->GetSocketRotation(Hit.BoneName);

	SkeletalMeshComponent->TransformToBoneSpace(Hit.BoneName, hit_world_location, hit_world_rotation, hit_bone_location, hit_bone_rotation);

	FString HitInformation = "{\"x\":";
	HitInformation += FString::SanitizeFloat(hit_bone_location.X);
	HitInformation += ",";

	HitInformation += "\"y\":";
	HitInformation += FString::SanitizeFloat(hit_bone_location.Y);
	HitInformation += ",";

	HitInformation += "\"z\":";
	HitInformation += FString::SanitizeFloat(hit_bone_location.Z);
	HitInformation += ",";

	HitInformation += "\"force\":";
	float force = NormalImpulse.Size();
	HitInformation += FString::SanitizeFloat(force);
	HitInformation += "}";

	HitsMap.FindOrAdd(Hit.BoneName.ToString()).Add(HitInformation);
}