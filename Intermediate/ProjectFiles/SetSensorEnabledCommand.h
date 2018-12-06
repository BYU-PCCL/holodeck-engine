#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "SetSensorEnabledCommand.generated.h"

/**
* SwitchSensorCommand
* Command used to add or remove a sensor from an agent
*
* StringParameters expect two arguments, the agent and sensor names.
* NumberParameters expect one arguemnt, a 1 or 0 indicating whether the sensor should be added or removed
*/
UCLASS()
class HOLODECK_API USetSensorEnabledCommand : public UCommand
{
	GENERATED_BODY()

public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;
};
