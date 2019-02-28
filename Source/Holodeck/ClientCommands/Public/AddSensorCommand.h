#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "AddSensorCommand.generated.h"

/**
* AddSensorCommand
* Command used to add a sensor to an agent
*
* StringParameters expect four arguments, the agent name, the socket name, sensor name, and sensor class.
*/
UCLASS()
class HOLODECK_API UAddSensorCommand : public UCommand
{
	GENERATED_BODY()

public:
	//See UCommand for the documentation of this overridden function.
	void Execute() override;

};
