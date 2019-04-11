#pragma once

#include "Holodeck.h"

#include "HolodeckPawnController.h"
#include "Android.h"
#include "HolodeckControlScheme.h"

#include "AndroidControlSchemeMaxTorque.generated.h"

/**
 * UAndroidControlSchemeMaxTorque
 */
UCLASS()
class HOLODECK_API UAndroidControlSchemeMaxTorque : public UHolodeckControlScheme
{
public:
	GENERATED_BODY()
	
	UAndroidControlSchemeMaxTorque(const FObjectInitializer& ObjectInitializer);

	void Execute(void* const CommandArray, void* const InputCommand, float DeltaSeconds) override;

	unsigned int GetControlSchemeSizeInBytes() const override {
		return 94 * sizeof(float);
	}

	void SetController(AHolodeckPawnController* const Controller) { AndroidController = Controller; };

private:
	AHolodeckPawnController* AndroidController;
	AAndroid* Android;

};
