#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "SpawnAgentCommand.h"

void USpawnAgentCommand::Execute() {

	UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand::Execute spawning agent"));
	verifyf(StringParams.size() == 2 && NumberParams.size() == 8, TEXT("%s: Bad Arguments"), *FString(__func__));

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
	float MaxHeight = (NumberParams[6] * 100);
	bool IsMainAgent = (bool) NumberParams[7];

	Location = ConvertLinearVector(Location, ClientToUE);

	// SpawnAgent command is defined in the HolodeckGameMode blueprint class and can only be edited/seen in the blueprint
	AHolodeckAgent* SpawnedAgent = GameTarget->SpawnAgent(AgentType, Location, Rotation, AgentName, IsMainAgent);
	verifyf(SpawnedAgent, TEXT("%s SpawnAgentCommand did not spawn a new Agent."), *FString(__func__));

	AHolodeckPawnController* SpawnedController = nullptr;

	SpawnedAgent->AgentName = AgentName;
	SpawnedAgent->MainAgent = IsMainAgent;
	SpawnedAgent->MaxHeight = MaxHeight;
	SpawnedAgent->SpawnDefaultController();
	SpawnedController = static_cast<AHolodeckPawnController*>(SpawnedAgent->Controller);
	SpawnedController->SetServer(GameTarget->GetAssociatedServer());
	SpawnedAgent->InitializeAgent();

	UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand spawned a new Agent."));

}