#include "Holodeck.h"
#include "SpawnAgentCommand.h"


USpawnAgentCommand::USpawnAgentCommand() {

}

USpawnAgentCommand::USpawnAgentCommand(std::vector<float> NumberParams, std::vector<std::string> StringParams) : UCommand(NumberParams, StringParams){

}

void USpawnAgentCommand::Execute() {
	//Program should throw an error if any of these aren't the correct size. They should always be this size.
	if (StringParams.size() != 2 || NumberParams.size() != 3) {
		UE_LOG(LogHolodeck, Warning, TEXT("Unexpected argument length found in USpawnAgentCommand. Agent not spawned."));
		return;
	}

	FString AgentType = StringParams[0].c_str();
	FString Name = StringParams[1].c_str();
	FVector Location = FVector(NumberParams[0], NumberParams[1], NumberParams[2]);
	//find out which agent was requested, then spawn that agent at that location, then give it the requested name!
	if (Name == UAV) {

	}
	if (Name == Android) {

	}
	if (Name == SphereRobot) {

	}
}