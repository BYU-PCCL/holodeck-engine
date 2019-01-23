// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "HolodeckSensor.h"

#include "HolodeckTask.generated.h"

/**
  * UHolodeckTask
  * A base class for tasks within Holodeck.
  * This class chooses a HolodeckAgent which is trying to perform the task.
  * The task logic then sets the reward and terminal each tick.
  * The child class must remember to call the parent tick class at the end of
  * setting the reward and terminal. This allows the parent class to set those
  * variables in the shared memory.
  */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API UHolodeckTask : public UHolodeckSensor {
	GENERATED_BODY()
	
public:	
	/**
	  * Default Constructor.
	  */
	UHolodeckTask();

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;

protected:
	//See HolodeckSensor for the documentation of these overridden functions.
	FString GetDataKey() override { return "HolodeckTask"; };
	int GetNumItems() override { return 2; };
	int GetItemSize() override { return sizeof(float); };
	void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float Reward;
	bool Terminal;

private:
	/*
	 * Parent
	 * After initialization, Parent contains a pointer to whatever the sensor is attached to.
	 */
	USceneComponent* Parent;
};
