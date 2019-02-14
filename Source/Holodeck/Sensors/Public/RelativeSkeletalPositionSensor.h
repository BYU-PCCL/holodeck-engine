// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "Components/SceneComponent.h"
#include "HolodeckSensor.h"

#include "RelativeSkeletalPositionSensor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API URelativeSkeletalPositionSensor : public UHolodeckSensor {
	GENERATED_BODY()

public:	
	/**
	  * Default Constructor.
	  */
	URelativeSkeletalPositionSensor();

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;
	
protected:
	// See HolodeckSensor for documentation on these functions.
	int GetNumItems() override { return 67 * 4; };
	int GetItemSize() override { return sizeof(float); };
	void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	USkeletalMeshComponent* SkeletalMeshComponent;
	TArray<FName> Bones;
	TArray<FName> ParentBones;
};
