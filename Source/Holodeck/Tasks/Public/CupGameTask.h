// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskSensor.h"
#include "CupGameTask.generated.h"

/**
* UCupGameTask
* Inherits from the TaskSensor class.
* Initializes the cup game in the world and calculates reward based off of which cup is selected
* Cup selection occurs when the 
* Terminal is always false.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API UCupGameTask : public UTaskSensor
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	UCupGameTask() : BallLocation(0) {}

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;

	/**
	* Allows parameters to be set dynamically
	*/
	virtual void ParseSensorParms(FString ParmsJson) override;

	// location of the ball under the cup (0-2)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BallLocation;

protected:
	// Checkout HolodeckSensor.h for the documentation for this overridden function.
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
