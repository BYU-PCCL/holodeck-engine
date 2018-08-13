#pragma once

#include "Holodeck.h"

#include "HolodeckControlScheme.generated.h"

/**
  * UHolodeckControlScheme
  */
UCLASS()
class HOLODECK_API UHolodeckControlScheme : public UObject {
	GENERATED_BODY()

public:
	virtual void Execute(void* const CommandArray, void* const InputCommand) const;

	virtual unsigned int GetControlSchemeSizeInBytes() const;
};
