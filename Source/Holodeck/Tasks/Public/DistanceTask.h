#pragma once

#include "Holodeck.h"

#include "HolodeckTask.h"

#include "DistanceTask.generated.h"

/**
* UDistanceTask
* Inherits from the HolodeckTask class.
* Calculates a distance based reward.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API UDistanceTask : public UHolodeckTask
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	UDistanceTask() : UseDistanceReward(true), Interval(1), GoalDistance(.1) {}

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;

	// Distance to next reward
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Interval;

	// Required proximity for terminal
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GoalDistance;

	// True: Reward is based on distance, False: Reward is scarce and +-1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UseDistanceReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* GoalObject;

protected:
	//Checkout HolodeckSensor.h for the documentation for this overridden function.
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/**
	* SetUnitReward
	* Sets the reward if UseDistanceReward is false.
	*/
	void SetUnitReward();

	/**
	* SetUnitReward
	* Sets the reward if UseDistanceReward is true.
	*/
	void SetDistanceReward();

	float NextDistance;
	float StartDistance;
	float LastDistance;
};
