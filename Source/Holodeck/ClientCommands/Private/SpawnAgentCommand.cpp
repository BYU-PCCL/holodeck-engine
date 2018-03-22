#include "Holodeck.h"
#include "SpawnAgentCommand.h"

const static FString UAVReference = "Blueprint'/Game/HolodeckContent/Agents/UAV/UAVBlueprint.UAVBlueprint'";
const static FString AndroidReference = "Blueprint'/Game/HolodeckContent/Agents/Android/AndroidBlueprint.AndroidBlueprint'";
const static FString SphereRobotReference = "Blueprint'/Game/HolodeckContent/Agents/SphereRobot/SphereRobotBlueprint.SphereRobotBlueprint'";
TSubclassOf<class AUAV> USpawnAgentCommand::UAVBlueprint = nullptr;
TSubclassOf<class AAndroid> USpawnAgentCommand::AndroidBlueprint = nullptr;
TSubclassOf<class ASphereRobot> USpawnAgentCommand::SphereRobotBlueprint = nullptr;
bool USpawnAgentCommand::bFirstInstance = true;

USpawnAgentCommand::USpawnAgentCommand() {
	//This is used to find the blueprints for the spawnable agents only the first time a spawnagetncommand is instantiated. 
	if (bFirstInstance) {
		bFirstInstance = false;
		ConstructorHelpers::FObjectFinder<UBlueprint> UAVBlueprintVar(*UAVReference);
		ConstructorHelpers::FObjectFinder<UBlueprint> AndroidBlueprintVar(*AndroidReference);
		ConstructorHelpers::FObjectFinder<UBlueprint> SphereRobotBlueprintVar(*SphereRobotReference);
		if (UAVBlueprintVar.Object) {
			UAVBlueprint = (UClass*)UAVBlueprintVar.Object->GeneratedClass;
		}
		else {
			UE_LOG(LogHolodeck, Warning, TEXT("SpawnAgentCommand unable to find UAV blueprint"));
		}
		if (AndroidBlueprintVar.Object) {
			AndroidBlueprint = (UClass*)AndroidBlueprintVar.Object->GeneratedClass;
		}
		else {
			UE_LOG(LogHolodeck, Warning, TEXT("SpawnAgentCommand unable to find Android blueprint"));
		}
		if (SphereRobotBlueprintVar.Object) {
			SphereRobotBlueprint = (UClass*)SphereRobotBlueprintVar.Object->GeneratedClass;
		}
		else {
			UE_LOG(LogHolodeck, Warning, TEXT("SpawnAgentCommand unable to find SphereRobot blueprint"));
		}
	}
}

void USpawnAgentCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand::Execute spawning agent"));
	//Program should throw an error if any of these aren't the correct size. They should always be this size.
	if (StringParams.size() != 2 || NumberParams.size() != 3) {
		UE_LOG(LogHolodeck, Warning, TEXT("Unexpected argument length found in USpawnAgentCommand. Agent not spawned."));
		return;
	}
	AHolodeckGameMode* GameTarget = static_cast<AHolodeckGameMode*>(Target);
	if (GameTarget == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("UCommand::Target is not a UHolodeckGameMode*. SpawnAgentCommand::Execute Cannot spawn agent without this pointer!"));
		return;
	}

	//if you can't get the world, then you can't spawn any agents
	UWorld* World = Target->GetWorld();
	if (World == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("SpawnAgentCommand::Execute found world as nullptr. Unable to spawn agent!"));
		return;
	}

	//set up the variables that will be the same for all of the agents.
	FString AgentType = StringParams[0].c_str();
	FName Name = FName(StringParams[1].c_str());
	FVector Location = FVector(NumberParams[0], NumberParams[1], NumberParams[2]);
	FRotator Rotation = FRotator();
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.Name = Name;
	AHolodeckAgent* SpawnedAgent = nullptr;
	AHolodeckPawnController* SpawnedController = nullptr;

	//find out which agent was requested, then spawn that agent at that location!
	//This is where you should put the spawn command for new agents you are putting into the code. 
	if (AgentType == UAV) {
		SpawnedAgent = World->SpawnActor<AUAV>(this->UAVBlueprint, Location, Rotation, SpawnParams);
	} else
	if (AgentType == Android) {
		SpawnedAgent = World->SpawnActor<AAndroid>(this->AndroidBlueprint, Location, Rotation, SpawnParams);
	} else
	if (AgentType == SphereRobot) {
		SpawnedAgent = World->SpawnActor<ASphereRobot>(this->SphereRobotBlueprint, Location, Rotation, SpawnParams);
	}

	//Finalize the initialization of the agent. 
	if (SpawnedAgent) {
		SpawnedAgent->AgentName = StringParams[1].c_str();
		SpawnedAgent->SpawnDefaultController();
		SpawnedController = static_cast<AHolodeckPawnController*>(SpawnedAgent->Controller);
		SpawnedController->SetServer(GameTarget->GetAssociatedServer());
		SpawnedAgent->InitializeController();
		UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand spawned a new Agent. Sanity check."));
	}
	else {
		UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand did not spawn a new Agent. Sanity check."));
	}
}
