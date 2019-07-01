#pragma once

#include "Holodeck.h"

#include "Android.h"
#include "HolodeckPawnController.h"
#include "AndroidControlSchemeMaxTorque.h"
#include "PhysicsEngine/ConstraintInstance.h"

#include "AndroidController.generated.h"

UCLASS()
class HOLODECK_API AAndroidController : public AHolodeckPawnController
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	AAndroidController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* Default Destructor
	*/
	~AAndroidController();

	/**
	* OnPossess
	* Called when the controller possesses the pawn.
	* @param Pawn the pawn being possessed.
	*/
	void OnPossess(APawn* Pawn) override;

	void AddControlSchemes() {
		UAndroidControlSchemeMaxTorque* ControlScheme = NewObject<UAndroidControlSchemeMaxTorque>();
		ControlScheme->SetController(this);
		ControlSchemes.Add(ControlScheme);
	}

private:

	USkeletalMeshComponent* SkeletalMeshComponent;
	float* ActionBufferFloatPtr;
	AHolodeckAgent* ControlledAndroid;
};
