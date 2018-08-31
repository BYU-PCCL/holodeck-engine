#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "DayCycleCommand.generated.h"

/**
* DayCycleCommand
* The command used to start or stop the day cycle. 
*
* StringParameters are expected to have one entry, "start" or "stop".
* NumberParameters are expected to have one entry greater than 0 IF the string param is "start".
* Otherwise they are expected to be empty.
*
* This command may not work properly if there is no skysphere or directional light in the world, but this should not cause a crash.
*/
UCLASS()
class HOLODECK_API UDayCycleCommand : public UCommand
{
	GENERATED_BODY()
	
public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;
	
	
};
