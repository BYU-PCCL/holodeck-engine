// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Holodeck.h"

#include "HolodeckSensor.h"

#include "TaskSensor.generated.h"

/**
  * UTaskSensor
  * A base class for tasks within Holodeck.
  * This class is a Holodeck Sensor.
  * This class chooses a HolodeckAgent which is trying to perform the task.
  * The task logic then sets the reward and terminal each tick.
  * The child class must remember to call the parent tick class at the end of
  * setting the reward and terminal. This allows the parent class to set those
  * variables in the shared memory.
  */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API UTaskSensor : public UHolodeckSensor {
	GENERATED_BODY()

public:
	/**
	  * Default Constructor.
	  */
	UTaskSensor();

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;

protected:
	// See HolodeckSensor.h for the documentation of these overridden functions.
	void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Child tasks should not need to override these three methods
	FString GetDataKey() override { return "TaskSensor"; };
	int GetNumItems() override { return 2; };
	int GetItemSize() override { return sizeof(float); };

	/*
	 * Parent
	 * After initialization, Parent contains a pointer to whatever the sensor is attached to.
	 */
	AActor* Parent;
	float Reward;
	bool Terminal;

};
