#pragma once

#include "Holodeck.h"

#include "HolodeckSensor.h"
#include "HolodeckViewportClient.h"
#include "HolodeckCamera.generated.h"

/**
* HolodeckCamera
* Abstract base class for cameras within holodeck
* A camera is anything that needs to access visual information.
* Two examples include a depth sensor and a standard camera. 
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), abstract)
class HOLODECK_API UHolodeckCamera : public UHolodeckSensor
{
	GENERATED_BODY()

public:
	/**
	  * Default Constructor
	  */
	UHolodeckCamera();

	/**
	  * BeginPlay
	  * Sets up the class, and gives a reference of itself to the viewport client. 
	  * Subclasses must call Super::BeginPlay()
	  */
	virtual void BeginPlay() override;

	/**
	  * Capture
	  * Must be overridden by subclasses
	  * Used to capture the desired pixel data and export it to the buffer.
	  */
	virtual void Capture() { check(0 && "You must override Capture"); };

protected:
	//Checkout HolodeckSensor.h for the documentation for this overridden function.
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {/*Do nothing*/};

	int CaptureWidth = 512;
	int CaptureHeight = 512;

private:

	UHolodeckViewportClient* ViewportClient;
};
