#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "SetWeatherCommand.generated.h"

/**
* SetWeatherCommand
* The command used to change the state of the weather in the world
*
* StringParameters expect 1 argument that should be either "rain", "cloudy".
* NumberParameters are expected to be empty.
*
* If there is no SkySphere_BP or Skylight there might be complications
*/
UCLASS()
class HOLODECK_API USetWeatherCommand : public UCommand
{
	GENERATED_BODY()
	
public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;

private:
	std::map<std::string, bool (*)()> FunctionMap;
};
