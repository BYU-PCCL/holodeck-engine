#include "Holodeck.h"
#include "Command.h"

UCommand::UCommand() {
	
}

std::unique_ptr<UCommand> UCommand::CommandFactory(std::string Name, std::vector<float> NumberParameters, std::vector<std::string> StringParameters) {
	return std::make_unique<UCommand>();
}