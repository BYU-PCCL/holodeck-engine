#pragma once

#include "Holodeck.h"

#include <vector>

#include "Command.h"
#include "SpawnAgentCommand.h"
#include "CommandFactory.generated.h"

class AHolodeckGameMode;

UCLASS(ClassGroup = (Custom), abstract)
class HOLODECK_API UCommandFactory : public UObject {
	GENERATED_BODY()

public:

	static UCommand* MakeCommand(std::string Name, std::vector<float> NumberParameters, std::vector<std::string> StringParameters, AHolodeckGameMode* ParameterGameMode);

private:
	UCommandFactory() {};
};
