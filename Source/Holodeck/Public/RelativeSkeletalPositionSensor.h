// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "HolodeckPawnController.h"
#include "RelativeSkeletalPositionSensor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API URelativeSkeletalPositionSensor : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URelativeSkeletalPositionSensor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	FString getBonesRelativeOffset();

private:
	USkeletalMeshComponent* SkeletalMeshComponent;
	AHolodeckPawnController* Controller;
	TArray<FName> Bones;
	TArray<FName> ParentBones;
};
