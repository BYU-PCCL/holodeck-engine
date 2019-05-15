#pragma once

#include "Holodeck.h"

#include "TaskSensor.h"

#include "LocationTask.generated.h"

/**
* ULocationTask
* Inherits from the TaskSensor class.
* Calculates a sparse distance reward.
* Maximizes Distance if Maximize distance is set to true.
* Location is used when actor is null.
* Terminal is set to true when the agent is within its GoalDistance.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API ULocationTask : public UTaskSensor
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	ULocationTask() : MaximizeDistance(false), GoalDistance(1), DistanceActor(nullptr), 
		DistanceLocation(this->GetComponentLocation()) {}

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;

	/**
	* Allows parameters to be set dynamically
	*/
	virtual void ParseSensorParms(FString ParmsJson) override;

	// Set to true to maximize instead of minimize distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool MaximizeDistance;

	// Required proximity for terminal
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GoalDistance;

	// Goal actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* DistanceActor;

	// Location (used if actor is null)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DistanceLocation;

protected:
	//Checkout HolodeckSensor.h for the documentation for this overridden function.
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
