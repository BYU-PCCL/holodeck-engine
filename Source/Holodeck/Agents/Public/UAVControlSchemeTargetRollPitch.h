#pragma once

#include "Holodeck.h"

#include "UAV.h"
#include "HolodeckControlScheme.h"
#include "SimplePID.h"
#include <math.h>

#include "UAVControlSchemeTargetRollPitch.generated.h"

/**
* UUAVControlSchemeTargetRollPitch
*/
UCLASS()
class HOLODECK_API UUAVControlSchemeTargetRollPitch : public UHolodeckControlScheme {
public:
	GENERATED_BODY()

	// Required constructor, shouldn't be used.
	UUAVControlSchemeTargetRollPitch(const FObjectInitializer& ObjectInitializer);

	UUAVControlSchemeTargetRollPitch(AUAV* ControlledUav);

	void Execute(void* const CommandArray, void* const InputCommand) override;

	unsigned int GetControlSchemeSizeInBytes() const override {
		return 4 * sizeof(float);
	}

private:
	FVector RotatorToEulerInZYX(const FRotator& Rotator) const;

	float UEUnitsToMeters(float ValueInUnrealUnits) const;

	AUAV* UAV;

	// PID Controllers
	SimplePID RollController;
	SimplePID PitchController;
	SimplePID YawController;
	SimplePID AltitudeController;


};
