#include "Holodeck.h"
#include "SpawnAgentCommand.h"

USpawnAgentCommand::USpawnAgentCommand() {

}

USpawnAgentCommand::USpawnAgentCommand(std::vector<float> NumberParams, std::vector<std::string> StringParams) : UCommand(NumberParams, StringParams){
	UE_LOG(LogHolodeck, Log, TEXT("USpawnAgentCommand::UspawnAgentCommand() called "));

}

void USpawnAgentCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("Dude your SpawnAgentCommand::Execute() was called!"));

}