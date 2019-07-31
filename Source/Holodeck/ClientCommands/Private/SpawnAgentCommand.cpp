#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "SpawnAgentCommand.h"

void USpawnAgentCommand::Execute() {

	UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand::Execute spawning agent"));
	//Program should throw an error if any of these params aren't the correct size. They should always be this size.
	if (StringParams.size() != 2 || NumberParams.size() != 6) {
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
	FString AgentName = StringParams[1].c_str();
	FVector Location = FVector(NumberParams[0], NumberParams[1], NumberParams[2]);

	// Note that we have to re-order the parameters since FRotator takes pitch, roll, yaw
	// but the coordinates from the Python side com in roll, pitch, yaw order
	FRotator Rotation = FRotator(NumberParams[4], NumberParams[3], NumberParams[5]);
	Location = ConvertLinearVector(Location, ClientToUE);

	// SpawnAgent command is defined in the HolodeckGameMode blueprint class and can only be edited/seen in the blueprint
	AHolodeckAgent* SpawnedAgent = GameTarget->SpawnAgent(AgentType, Location, Rotation, AgentName);
	AHolodeckPawnController* SpawnedController = nullptr;

	if (SpawnedAgent) {
		SpawnedAgent->AgentName = AgentName;
		SpawnedAgent->SpawnDefaultController();
		SpawnedController = static_cast<AHolodeckPawnController*>(SpawnedAgent->Controller);
		SpawnedController->SetServer(GameTarget->GetAssociatedServer());
		SpawnedAgent->InitializeAgent();
		
		UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand spawned a new Agent."));
	} else {
		UE_LOG(LogHolodeck, Fatal, TEXT("SpawnAgentCommand did not spawn a new Agent."));
	}
}
