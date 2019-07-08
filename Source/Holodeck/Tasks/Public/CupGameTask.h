// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskSensor.h"
#include "CupGameTask.generated.h"

/**
* UCupGameTask
* Inherits from the TaskSensor class.
* Initializes the cup game in the world and calculates reward based off of which cup is selected
* A reward of 50 is given if the correct cup is touched and no other cups are touched, and a reward
* of 100 and terminal is given when the ball itself is touched and no incorrect cups are touched.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API UCupGameTask : public UTaskSensor
{
	GENERATED_BODY()

public:

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;

	/**
	* Allows parameters to be set dynamically
	*/
	virtual void ParseSensorParms(FString ParmsJson) override;

protected:
	// Checkout HolodeckSensor.h for the documentation for this overridden function.
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
