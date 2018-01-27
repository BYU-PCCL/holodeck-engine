// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Holodeck.h"

#include "HolodeckSensor.h"

#include "LocationSensor.generated.h"

/**
* LocationSensor
* Inherits from the HolodeckSensor class
* Check out the parent class for documentation on all of the overridden functions.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API ULocationSensor : public UHolodeckSensor {
	GENERATED_BODY()

public:
	/*
	* Default Constructor
	*/
	ULocationSensor();

	/*
	* BeginPlay
	* Called at the start of the game.
	*/
	void BeginPlay() override;

protected:
	//See HolodeckSensor for the documentation of these overridden functions.
	FString GetDataKey() override { return "LocationSensor"; };
	int GetNumItems() override { return 3; };
	int GetItemSize() override { return sizeof(float); };
	void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/*
	 * Parent
	 * After initialization, Parent contains a pointer to whatever the sensor is attached to.
	 */
	USceneComponent* Parent;
	float UnitsPerMeter;
};
