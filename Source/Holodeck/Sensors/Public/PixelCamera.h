#pragma once

#include "Holodeck.h"

#include "HolodeckCamera.h"
#include "PixelCamera.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API UPixelCamera : public UHolodeckCamera {
	GENERATED_BODY()

public:
		UPixelCamera();

		virtual void BeginPlay() override;

protected:
	//Check out HolodeckSensor.h for definitions for this overridden functions. 
	virtual FString GetDataKey() { return "PixelCamera"; };
	virtual int GetNumItems() { return CaptureWidth*CaptureHeight; };
	virtual int GetItemSize() { return sizeof(float); };


private:
	

};
