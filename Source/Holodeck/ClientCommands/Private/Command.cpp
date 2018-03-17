#include "Holodeck.h"
#include "Command.h"

UCommand::UCommand() {
	
}

UCommand::UCommand(std::vector<float> NumberParameters, std::vector<std::string> StringParameters) : NumberParams(NumberParameters), StringParams(StringParameters) {
	UE_LOG(LogHolodeck, Log, TEXT("UCOmmand was made"));

}

//std::unique_ptr<UCommand> UCommand::CommandFactory(std::string Name, std::vector<float> NumberParameters, std::vector<std::string> StringParameters) {
//	return std::make_unique<USpawnAgentCommand>();
//}