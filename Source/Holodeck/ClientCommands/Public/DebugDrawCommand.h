#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "DrawDebugHelpers.h"
#include "DebugDrawCommand.generated.h"

/**
* DayTimeCommand
* The command used to change the lighting and sky sphere in the world.
*
* StringParameters are expected to be empty.
* NumberParameters are expected to be an hour between 0-23 in military time.
*
* This command may not work properly if there is no skysphere or directional light in the world, but this should not cause a crash.
*/
UCLASS(ClassGroup = (Custom))
class HOLODECK_API UDebugDrawCommand : public UCommand
{
	GENERATED_BODY()


public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;

private:

};
