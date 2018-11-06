#pragma once

#include "Holodeck.h"

#include "HolodeckCamera.h"
#include "RGBCamera.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API URGBCamera : public UHolodeckCamera {
	GENERATED_BODY()

public:
	URGBCamera();

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;

protected:
	//Checkout HolodeckSensor.h for the documentation for this overridden function.
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	//Check out HolodeckSensor.h for definitions for this overridden functions. 
	virtual FString GetDataKey() { return "RGBCamera"; };
	virtual int GetNumItems() { return CaptureWidth * CaptureHeight; };
	virtual int GetItemSize() { return sizeof(float); };

	UPROPERTY(EditAnywhere)
	int TicksPerCapture = 1;

private:
	int TickCounter = 0;

};
