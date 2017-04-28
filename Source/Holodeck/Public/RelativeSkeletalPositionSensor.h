// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "HolodeckSensor.h"
#include "RelativeSkeletalPositionSensor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API URelativeSkeletalPositionSensor : public UHolodeckSensor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URelativeSkeletalPositionSensor();

	// Called when the game starts
	virtual void BeginPlay() override;
	

protected:
	virtual void SetDataType() override { ResultData.Type = "RelativeSkeletalPositionSensor"; };
	// Called every frame
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FString getBonesRelativeOffset();

	USkeletalMeshComponent* SkeletalMeshComponent;
	TArray<FName> Bones;
	TArray<FName> ParentBones;
};
