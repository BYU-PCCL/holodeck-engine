// MIT License (c) 2019 BYU PCCL see LICENSE file

#pragma once

#include "CoreMinimal.h"
#include "HolodeckCore/Public/HolodeckSensor.h"
#include "RangeFinderSensor.generated.h"

/**
 * URangeFinderSensor
 * The RangeFinderSensor gets distances to first collision.
 * By default the sensor retruns one number that represents the distance to
 * the first collision in the agent's forward direction. Increasing LazerCount
 * will create an array of sensors evenly distributed 360 degrees in a plane 
 * around the agent. The LazerAngle offsets each of these and transforms the
 * plane into a cone. The default MaxDistance is 1000.
 */
UCLASS()
class HOLODECK_API URangeFinderSensor : public UHolodeckSensor
{
	GENERATED_BODY()
	
public:
	/*
	* Default Constructor
	*/
	URangeFinderSensor();

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
	//See HolodeckSensor for the documentation of these overridden functions.
	int GetNumItems() override { return LazerCount; };
	int GetItemSize() override { return sizeof(float); };
	void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	int LazerCount = 1;

	UPROPERTY(EditAnywhere)
	int LazerAngle = 0;

	UPROPERTY(EditAnywhere)
	int LazerMaxDistance = 1000;

	UPROPERTY(EditAnywhere)
	bool LazerDebug = false;

private:
	/*
	 * Parent
	 * After initialization, Parent contains a pointer to whatever the sensor is attached to.
	 */
	AActor* Parent;
};
