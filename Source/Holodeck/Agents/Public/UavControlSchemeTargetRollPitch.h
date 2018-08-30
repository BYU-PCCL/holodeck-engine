#pragma once

#include "Holodeck.h"

#include "UAV.h"
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

	// Required constructor, shouldn't be used.
	UUavControlSchemeTargetRollPitch(const FObjectInitializer& ObjectInitializer);

	UUavControlSchemeTargetRollPitch(AUav* ControlledUav);

	void Execute(void* const CommandArray, void* const InputCommand) override;

	unsigned int GetControlSchemeSizeInBytes() const override {
		return 4 * sizeof(float);
	}

private:
	FVector RotatorToEulerInZYX(const FRotator& Rotator) const;

	float UEUnitsToMeters(float ValueInUnrealUnits) const;

	AUav* UAV;

	// PID Controllers
	SimplePID RollController;
	SimplePID PitchController;
	SimplePID YawController;
	SimplePID AltitudeController;


};
