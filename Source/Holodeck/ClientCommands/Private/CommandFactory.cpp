#include "Holodeck.h"
#include "CommandFactory.h"
#include "HolodeckGameMode.h"

const std::string SPAWN_AGENT = "SpawnAgent";

UCommand* UCommandFactory::MakeCommand(std::string Name, std::vector<float> NumberParameters, std::vector<std::string> StringParameters, AHolodeckGameMode* ParameterGameMode) {
	UCommand* ToReturn = nullptr;
	if (Name == SPAWN_AGENT) {
		ToReturn = NewObject<USpawnAgentCommand>();
		ToReturn->Init(NumberParameters, StringParameters);
	}

	return ToReturn;
}