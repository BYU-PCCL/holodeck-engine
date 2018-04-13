#include "Holodeck.h"
#include "SpawnAgentCommand.h"

const static FString UAV_REFERENCE = "Blueprint'/Game/HolodeckContent/Agents/UAV/UAVBlueprint.UAVBlueprint'";
const static FString ANDROID_REFERENCE = "Blueprint'/Game/HolodeckContent/Agents/Android/AndroidBlueprint.AndroidBlueprint'";
const static FString SPHERE_ROBOT_REFERENCE = "Blueprint'/Game/HolodeckContent/Agents/SphereRobot/SphereRobotBlueprint.SphereRobotBlueprint'";
const static FString NAV_AGENT_REFERENCE = "Blueprint'/Game/HolodeckContent/Agents/NavAgent/NavAgentBlueprint.NavAgentBlueprint'";
const static std::string UAV = "UAV";
const static std::string SPHERE_ROBOT = "SphereRobot";
const static std::string NAV_AGENT = "NavAgent";
const static std::string ANDROID = "Android";
USpawnAgentCommand::BlueprintMapType USpawnAgentCommand::BlueprintMap;
USpawnAgentCommand::SpawnFunctionMapType USpawnAgentCommand::SpawnFunctionMap;

USpawnAgentCommand::USpawnAgentCommand() {
	static bool bFirstInstance = true;
	//This is used to find the blueprints for the spawnable agents only the first time a USpawnAgentCommand is instantiated. 
	if (bFirstInstance) {
		bFirstInstance = false;
		//initialize the SpawnFunctionMap
		SpawnFunctionMap[UAV] = &SpawnAgent<AUAV>;
		SpawnFunctionMap[SPHERE_ROBOT] = &SpawnAgent<ASphereRobot>;
		SpawnFunctionMap[ANDROID] = &SpawnAgent<AAndroid>;
		SpawnFunctionMap[NAV_AGENT] = &SpawnAgent<ANavAgent>;
		//Initialize the BlueprintMap
		BlueprintMap[UAV] = GetBlueprint(UAV_REFERENCE);
		BlueprintMap[SPHERE_ROBOT] = GetBlueprint(SPHERE_ROBOT_REFERENCE);
		BlueprintMap[ANDROID] = GetBlueprint(ANDROID_REFERENCE);
		BlueprintMap[NAV_AGENT] = GetBlueprint(NAV_AGENT_REFERENCE);
	}
}

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

	//instantiate variables to be used for spawning the agent.
	FString AgentType = StringParams[0].c_str();
	FVector Location = FVector(NumberParams[0], NumberParams[1], NumberParams[2]);
	AHolodeckAgent* SpawnedAgent = nullptr;
	AHolodeckPawnController* SpawnedController = nullptr;
	auto AgentSpawnFunction = SpawnFunctionMap[StringParams[0]];
	UClass* Blueprint = BlueprintMap[StringParams[0]];

	if (AgentSpawnFunction && Blueprint)
		SpawnedAgent = AgentSpawnFunction(Blueprint, Location, World);
	else
		UE_LOG(LogHolodeck, Warning, TEXT("Maps did not contain requested agent type."));

	//Finalize the initialization of the agent. 
	if (SpawnedAgent) {
		SpawnedAgent->AgentName = StringParams[1].c_str();
		SpawnedAgent->SpawnDefaultController();
		SpawnedController = static_cast<AHolodeckPawnController*>(SpawnedAgent->Controller);
		SpawnedController->SetServer(GameTarget->GetAssociatedServer());
		SpawnedAgent->InitializeController();
		UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand spawned a new Agent."));
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("SpawnAgentCommand did not spawn a new Agent."));
	}
}

template<typename T>
AHolodeckAgent* USpawnAgentCommand::SpawnAgent(UClass* const Blueprint, const FVector& Location, UWorld* const World){
	if (!World)
		return nullptr;
	return World->SpawnActor<T>(Blueprint, Location, FRotator(0, 0, 0), FActorSpawnParameters());
}
