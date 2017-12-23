#pragma once
#include "Holodeck.h"
#include "HolodeckSensor.h"

/**
* LocationSensor
* Inherits from the HolodeckSensor class
* Check out the parent class for documentation on all of the overridden functions.


*/
class ULocationSensor : public UHolodeckSensor {

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
	/*
	* GetLocation
	* Returns the location of the object (its RootComponent) that the sensor is attached to.
	*/
	FVector GetLocationVector();

protected:
	//See HolodeckSensor for the documentation of these overridden functions.
	FString GetDataKey() override { return "LocationSensor"; };
	int GetNumItems() override { return 3; };
	int GetItemSize() override { return sizeof(float); };
	void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	USceneComponent* Parent;


};


