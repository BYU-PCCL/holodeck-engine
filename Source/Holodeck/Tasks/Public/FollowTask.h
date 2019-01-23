#pragma once

#include "Holodeck.h"

#include "HolodeckTask.h"

#include "FollowTask.generated.h"

/**
* UFollowTask
* Inherits from the HolodeckTask class.
* Calculates follow reward based on distance and line of sight.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API UFollowTask : public UHolodeckTask
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	UFollowTask() : MaxScore(100) {}

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;

	// Actor to follow
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* ToFollow;

	// Only give reward if target is in sight
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool OnlyWithinSight;

	// Defines the agent's field of view
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FOVRadians;

	// Defines the minimum distance to recieve positive reward
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinDistance;

	// Defines the agent's height
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AgentHeight;
	
protected:
	//Checkout HolodeckSensor.h for the documentation for this overridden function.
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Scales score between 0-1 to 0-100
	int MaxScore;
};
