#pragma once

#include "Holodeck.h"

#include "HolodeckControlScheme.h"

#include "RawControlScheme.generated.h"

/**
* URawControlScheme
*/
UCLASS()
class HOLODECK_API URawControlScheme : public UHolodeckControlScheme {
	GENERATED_BODY()

	// Constructor required by engine. Shouldn't be used
	URawControlScheme() {};

public:
	URawControlScheme(AHolodeckAgent* const ControlledAgent);

	void Execute(void* const CommandArray, void* const InputCommand) const override;

	unsigned int GetControlSchemeSizeInBytes() const {
		return Agent->GetRawActionSizeInBytes();
	}

private:
	AHolodeckAgent* Agent;
};
