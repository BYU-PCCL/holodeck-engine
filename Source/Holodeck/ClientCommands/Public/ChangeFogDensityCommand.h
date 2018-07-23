#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "HolodeckGameMode.h"
#include "ChangeFogDensityCommand.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom))
class HOLODECK_API UChangeFogDensityCommand : public UCommand
{
	GENERATED_BODY()
	
public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;
};
