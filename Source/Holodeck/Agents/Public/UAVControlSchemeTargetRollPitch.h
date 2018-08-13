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
	UUAVControlSchemeTargetRollPitch() {};

	UUAVControlSchemeTargetRollPitch(AUAV* ControlledUav);

	void Execute(void* const CommandArray, void* const InputCommand) const override;

	unsigned int GetControlSchemeSizeInBytes() const override {
		return 4 * sizeof(float);
	}

private:
	FVector RotatorToEulerInZYX(const FRotator& Rotator);

	float UEUnitsToMeters(float ValueInUnrealUnits);

	AUAV* UAV;

	// State
	float CurrentPositionX;
	float CurrentPositionY;
	float CurrentPositionZ;

	float CurrentRollTorque;
	float CurrentPitchTorque;
	float CurrentYawTorque;
	float CurrentThrust;

	float CurrentRoll;
	float CurrentPitch;
	float CurrentYaw;

	float CurrentGlobalVelocityZ;
	float CurrentYawRate;

	// PID Controllers
	SimplePID RollController;
	SimplePID PitchController;
	SimplePID YawController;
	SimplePID AltitudeController;


};
