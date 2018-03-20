#include "Holodeck.h"
#include "Command.h"

UCommand::UCommand() {
	
}

void UCommand::Init(std::vector<float> NumberParameters, std::vector<std::string> StringParameters, AActor* ParameterTarget) {
	this->NumberParams = NumberParameters;
	this->StringParams = StringParameters;
	this->Target = ParameterTarget;
}
