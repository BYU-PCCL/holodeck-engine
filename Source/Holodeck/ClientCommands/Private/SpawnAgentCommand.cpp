#include "Holodeck.h"
#include "SpawnAgentCommand.h"


USpawnAgentCommand::USpawnAgentCommand() {
	static bool bFirstInstance = true;
	if (bFirstInstance) {
		bFirstInstance = false;
	}
}

USpawnAgentCommand::USpawnAgentCommand(std::vector<float> NumberParams, std::vector<std::string> StringParams) : UCommand(NumberParams, StringParams){

}

void USpawnAgentCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand::Execute was called"));

	//Program should throw an error if any of these aren't the correct size. They should always be this size.
	if (StringParams.size() != 2 || NumberParams.size() != 3) {
		UE_LOG(LogHolodeck, Warning, TEXT("Unexpected argument length found in USpawnAgentCommand. Agent not spawned."));
		return;
	}
	if (Target == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("SpawnAgentCommand::Target is nullptr. Cannot spawn agent without a target!"));

	}

	FString AgentType = StringParams[0].c_str();
	//FString TempString = StringParams[1].c_str();
	FName Name = FName(StringParams[1].c_str());
	FVector Location = FVector(NumberParams[0], NumberParams[1], NumberParams[2]);
	FRotator Rotation = FRotator();
	//find out which agent was requested, then spawn that agent at that location, then give it the requested name!
	if (AgentType == UAV) {
		FActorSpawnParameters ActorSpawnParameters = FActorSpawnParameters();
		ActorSpawnParameters.Name = Name;
		//Target->GetWorld()->SpawnActor<AUAV>(Location, Rotation, ActorSpawnParameters);
		UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand spawned a new UAV. Sanity check. "));

		return;
	}
	if (AgentType == Android) {

	}
	if (AgentType == SphereRobot) {

	}
}