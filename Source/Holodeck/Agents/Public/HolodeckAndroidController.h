#pragma once

#include "Holodeck.h"

#include "Android.h"
#include "HolodeckPawnController.h"
#include "PhysicsEngine/ConstraintInstance.h"

#include "HolodeckAndroidController.generated.h"

UCLASS()
class HOLODECK_API AHolodeckAndroidController : public AHolodeckPawnController
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	AHolodeckAndroidController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* Default Destructor
	*/
	~AHolodeckAndroidController();

	/**
	* Possess
	* Called when the controller possesses the pawn.
	* @param Pawn the pawn being possessed.
	*/
	void Possess(APawn* Pawn) override;

	void AddControlSchemes() override {
		// No other control schemes
	};

private:

	USkeletalMeshComponent* SkeletalMeshComponent;
	float* ActionBufferFloatPtr;
	AHolodeckAgent* ControlledAndroid;
};
