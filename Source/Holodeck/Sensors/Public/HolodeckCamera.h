#pragma once

#include "Holodeck.h"

#include "HolodeckSensor.h"
#include "HolodeckViewportClient.h"
#include "RenderRequest.h"
#include "HolodeckCamera.generated.h"

/**
* HolodeckCamera
* Abstract base class for cameras within holodeck
* A camera is anything that needs to access visual information.
* Two examples include a depth sensor and a standard camera.
*/
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

protected:
	//Checkout HolodeckSensor.h for the documentation for this overridden function.
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<USceneComponent> SceneCap;

	UPROPERTY()
	UTextureRenderTarget2D* TargetTexture;

	UPROPERTY()
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(EditAnywhere)
	int CaptureWidth = 256;

	UPROPERTY(EditAnywhere)
	int CaptureHeight = 256;

private:

	bool bPointerGivenToViewport = false;
	UHolodeckViewportClient* ViewportClient;
	FColor* Buffer;
	FRenderRequest RenderRequest;
};