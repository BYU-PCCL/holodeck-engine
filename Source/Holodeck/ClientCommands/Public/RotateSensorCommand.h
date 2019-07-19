#pragma once

#include "Holodeck.h"

#include "HolodeckSensor.h"
#include "Command.h"
#include "RotateSensorCommand.generated.h"

/**
 * 
 */
UCLASS()
class HOLODECK_API URotateSensorCommand : public UCommand
{
	GENERATED_BODY()
	
	public:
	//See UCommand for the documentation of this overridden function.
	void Execute() override;
	
	
};
