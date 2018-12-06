#pragma once

#include "Holodeck.h"

#include "HolodeckSensor.h"
#include "Command.h"
#include "SetSensorEnabledCommand.generated.h"

/**
* SetSensorEnabledCommand
* Command used to enable or diable a sensor from an agent
*
* StringParameters expect two arguments, the agent and sensor names.
* NumberParameters expect one arguemnt, a 1 or 0 indicating whether the sensor should be enabled or disabled
*/
UCLASS()
class HOLODECK_API USetSensorEnabledCommand : public UCommand
{
	GENERATED_BODY()

public:
	//See UCommand for the documentation of this overridden function.
	void Execute() override;
};
