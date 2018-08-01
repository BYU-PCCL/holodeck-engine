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
	USetWeatherCommand();
	void Execute() override;

private:
	std::map<std::string, bool (*)()> FunctionMap;
};
