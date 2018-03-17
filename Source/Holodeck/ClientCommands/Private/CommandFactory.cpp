#include "Holodeck.h"
#include "CommandFactory.h"

const std::string SPAWN_AGENT = "SpawnAgent";

std::unique_ptr<UCommand> UCommandFactory::MakeCommand(std::string Name, std::vector<float> NumberParameters, std::vector<std::string> StringParameters) {
	UE_LOG(LogHolodeck, Log, TEXT("You made it to UcommandFactory::MakeCommand()"));
	std::unique_ptr<UCommand> ToReturn;
	if (Name == SPAWN_AGENT) {
		UE_LOG(LogHolodeck, Log, TEXT("entered if statement"));
		std::unique_ptr<UCommand> SomePtr = std::make_unique<USpawnAgentCommand>();
	}

	UE_LOG(LogHolodeck, Log, TEXT("returning from UCommandFactory::MakeCommand()"));
	return ToReturn;
}