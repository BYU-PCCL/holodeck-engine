#include "Holodeck.h"
#include "SpawnAgentCommand.h"

void USpawnAgentCommand::Execute() {

	UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand::Execute spawning agent"));
	//Program should throw an error if any of these params aren't the correct size. They should always be this size.
	if (StringParams.size() != 2 || NumberParams.size() != 3) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in USpawnAgentCommand. Agent not spawned."));
		return;
	}

	AHolodeckGameMode* GameTarget = static_cast<AHolodeckGameMode*>(Target);
	if (GameTarget == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("UCommand::Target is not a UHolodeckGameMode*. SpawnAgentCommand::Execute Cannot spawn agent without this pointer!"));
		return;
	}

	UWorld* World = Target->GetWorld();
	if (World == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("SpawnAgentCommand::Execute found world as nullptr. Unable to spawn agent!"));
		return;
	}

	FString AgentType = StringParams[0].c_str();
	FVector Location = FVector(NumberParams[0], NumberParams[1], NumberParams[2]) * UnitsPerMeter;

	// SpawnAgent command is defined in the HolodeckGameMode blueprint class and can only be edited/seen in the blueprint
	AHolodeckAgent* SpawnedAgent = GameTarget->SpawnAgent(AgentType, Location);
	AHolodeckPawnController* SpawnedController = nullptr;

	if (SpawnedAgent) {
		SpawnedAgent->AgentName = StringParams[1].c_str();
		SpawnedAgent->SpawnDefaultController();
		SpawnedController = static_cast<AHolodeckPawnController*>(SpawnedAgent->Controller);
		SpawnedController->SetServer(GameTarget->GetAssociatedServer());
		SpawnedAgent->InitializeController();
		SpawnedAgent->InitializeAgent();
		
		UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand spawned a new Agent."));
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("SpawnAgentCommand did not spawn a new Agent."));
	}
}
