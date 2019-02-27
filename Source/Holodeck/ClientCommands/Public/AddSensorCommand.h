#pragma once

#include "Holodeck.h"

#include "HolodeckSensor.h"
#include "Command.h"
#include "AddSensorCommand.generated.h"

/**
* AddSensorCommand
* Command used to add a sensor to an agent
*
* StringParameters expect two arguments, the agent and sensor name.
*/
UCLASS()
class HOLODECK_API UAddSensorCommand : public UCommand
{
	GENERATED_BODY()

public:
	//See UCommand for the documentation of this overridden function.
	void Execute() override;
};
