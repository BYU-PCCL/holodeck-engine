#include "Holodeck.h"
#include "Command.h"

UCommand::UCommand() {
	
}

void UCommand::Init(const std::vector<float>& NumberParameters, const std::vector<std::string>& StringParameters, AActor* const ParameterTarget) {
	this->NumberParams = NumberParameters;
	this->StringParams = StringParameters;
	this->Target = ParameterTarget;
}
