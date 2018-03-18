#include "Holodeck.h"
#include "Command.h"
//#include "HolodeckGameMode.h"

UCommand::UCommand() {
	
}

UCommand::UCommand(std::vector<float> NumberParameters, std::vector<std::string> StringParameters) : NumberParams(NumberParameters), StringParams(StringParameters) {

}

//std::unique_ptr<UCommand> UCommand::CommandFactory(std::string Name, std::vector<float> NumberParameters, std::vector<std::string> StringParameters) {
//	return std::make_unique<USpawnAgentCommand>();
//}

void UCommand::Init(std::vector<float> NumberParameters, std::vector<std::string> StringParameters/*, AHolodeckGameMode* ParameterGameMode*/) {
	this->NumberParams = NumberParameters;
	this->StringParams = StringParameters;
	//this->GameMode = ParameterGameMode;
}