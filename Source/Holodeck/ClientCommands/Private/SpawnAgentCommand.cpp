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
	AActor* SpawnedAgent = nullptr;

	//find out which agent was requested, then spawn that agent at that location, and give it the requested name!
	if (AgentType == UAV) {
		SpawnedAgent = World->SpawnActor<AUAV>(this->UAVBlueprint, Location, Rotation, SpawnParams);
	}
	if (AgentType == Android) {
		SpawnedAgent = World->SpawnActor<AAndroid>(this->AndroidBlueprint, Location, Rotation, SpawnParams);
	}
	if (AgentType == SphereRobot) {
		SpawnedAgent = World->SpawnActor<ASphereRobot>(this->SphereRobotBlueprint, Location, Rotation, SpawnParams);
	}
	if (SpawnedAgent) {
		UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand spawned a new Agent. Sanity check."));
	}
	else {
		UE_LOG(LogHolodeck, Log, TEXT("SpawnAgentCommand did not spawn a new Agent. Sanity check."));
	}
}
