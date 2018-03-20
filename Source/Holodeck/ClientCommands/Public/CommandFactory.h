#pragma once

#include "Holodeck.h"

#include <vector>

#include "Command.h"
#include "SpawnAgentCommand.h"
#include "CommandFactory.generated.h"

class AHolodeckGameMode;

/**
  * UCommandFactory
  * This is the class that should be used to instantiate UCommand objects. Feed it the name of the command along with
  * the parameters that the command will need to execute. If the parameters are not needed, then give it nullptr or 
  * empty vectors and it will work fine. 
  * The purpose of this was to make separate knowledge of specific commands from the command center, to remove circular
  * dependencies, and to give an easy way of spawning commands. 
  */
UCLASS(ClassGroup = (Custom), abstract)
class HOLODECK_API UCommandFactory : public UObject {
	GENERATED_BODY()

public:

	static UCommand* MakeCommand(std::string Name, std::vector<float> NumberParameters, std::vector<std::string> StringParameters, AActor* ParameterGameMode);

private:
	UCommandFactory() {};
};
