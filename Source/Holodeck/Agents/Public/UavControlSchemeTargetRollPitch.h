#pragma once

#include "Holodeck.h"

#include "HolodeckPawnController.h"
#include "Uav.h"
#include "HolodeckControlScheme.h"
#include "SimplePID.h"
#include <math.h>

#include "UavControlSchemeTargetRollPitch.generated.h"

/**
* UUavControlSchemeTargetRollPitch
*/
UCLASS()
class HOLODECK_API UUavControlSchemeTargetRollPitch : public UHolodeckControlScheme {
public:
	GENERATED_BODY()

	UUavControlSchemeTargetRollPitch(const FObjectInitializer& ObjectInitializer);

	void Execute(void* const CommandArray, void* const InputCommand, float DeltaSeconds) override;

	unsigned int GetControlSchemeSizeInBytes() const override {
		return 4 * sizeof(float);
	}

	void SetController(AHolodeckPawnController* const Controller) { UavController = Controller; };

private:
	FVector RotatorToEulerInZYX(const FRotator& Rotator) const;

	AHolodeckPawnController* UavController;
	AUav* Uav;

	// PID Controllers
	SimplePID RollController;
	SimplePID PitchController;
	SimplePID YawController;
	SimplePID AltitudeController;


};
