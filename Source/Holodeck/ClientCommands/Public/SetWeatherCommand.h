#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "SetWeatherCommand.generated.h"

/**
 * 
 */
UCLASS()
class HOLODECK_API USetWeatherCommand : public UCommand
{
	GENERATED_BODY()
	
public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;
	
};
