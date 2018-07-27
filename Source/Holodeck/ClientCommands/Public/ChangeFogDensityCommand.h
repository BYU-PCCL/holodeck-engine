#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "ChangeFogDensityCommand.generated.h"

/**
* ChangeFogDensityCommand
* The command used to alter the density of the exponential height fog in the world
*
* StringParameters are expected to be empty.
* NumberParameters are expected to be a density value between 0 and 1.
*
* If there is no exponential height fog in the world, it will be spawned automatically.
*/
UCLASS(ClassGroup = (Custom))
class HOLODECK_API UChangeFogDensityCommand : public UCommand
{
	GENERATED_BODY()
	
public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;
};
