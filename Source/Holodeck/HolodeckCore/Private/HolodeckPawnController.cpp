// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckPawnController.h"
#include "HolodeckAgent.h" //Must forward declare this so that you can access its teleport function. 

AHolodeckPawnController::AHolodeckPawnController(const FObjectInitializer& ObjectInitializer)
		: AAIController(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}

AHolodeckPawnController::~AHolodeckPawnController() { }

void AHolodeckPawnController::BeginPlay() {
	Super::BeginPlay();
	AddTickPrerequisiteActor(GetWorld()->GetAuthGameMode());
}

void AHolodeckPawnController::Possess(APawn* InPawn) {
	Super::Possess(InPawn);
	UE_LOG(LogHolodeck, Log, TEXT("Pawn Possessed: %s, Controlled by: %s"), *InPawn->GetHumanReadableName(), *this->GetClass()->GetName());
	GetServer();
	if (Server == nullptr)
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckPawnController couldn't find server..."));
}

void AHolodeckPawnController::UnPossess() {
	Super::UnPossess();
}

void AHolodeckPawnController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (CheckBoolBuffer(ShouldTeleportBuffer))
		ExecuteTeleport();
	ExecuteCommand();
}

void* AHolodeckPawnController::Subscribe(const FString& AgentName, const FString& SensorName, int NumItems, int ItemSize) {
	GetServer();
	UE_LOG(LogHolodeck, Log, TEXT("Subscribing sensor %s for %s"), *SensorName, *AgentName);
	if (Server == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("Sensor could not find server..."));
		return nullptr;
	} else {
		return Server->SubscribeSensor(TCHAR_TO_UTF8(*AgentName), TCHAR_TO_UTF8(*SensorName), NumItems * ItemSize);
	}
}

void AHolodeckPawnController::GetServer() {
	if (Server != nullptr) return;
	UHolodeckGameInstance* Instance = static_cast<UHolodeckGameInstance*>(GetGameInstance());
	if (Instance != nullptr)
		Server = Instance->GetServer();
	else
		UE_LOG(LogHolodeck, Warning, TEXT("Game Instance is not UHolodeckGameInstance."));
}

void AHolodeckPawnController::GetBuffers(const FString& AgentName) {
	if (Server != nullptr) {
		ActionBuffer = Server->SubscribeActionSpace(TCHAR_TO_UTF8(*AgentName), GetActionSpaceDimension() * sizeof(float));
		FString BoolString = AgentName + "_teleport_bool";
		ShouldTeleportBuffer = Server->SubscribeActionSpace(TCHAR_TO_UTF8(*BoolString), SINGLE_BOOL * sizeof(bool));
		FString CommandString = AgentName + "_teleport_command";
		TeleportBuffer = Server->SubscribeActionSpace(TCHAR_TO_UTF8(*CommandString), TELEPORT_COMMAND_SIZE * sizeof(float));
		FString HyperParameterBufferName = AgentName + "_hyperparameter";
		UE_LOG(LogHolodeck, Log, TEXT("Buffer name: %s"), *HyperParameterBufferName);
		AHolodeckAgent* HolodeckPawn = static_cast<AHolodeckAgent*>(this->GetPawn()); 
		if (HolodeckPawn) {
			this->HyperparameterBuffer = static_cast<float*>(Server->SubscribeSetting(TCHAR_TO_UTF8(*HyperParameterBufferName), HolodeckPawn->GetHyperparameterCount() * sizeof(bool)));
			HolodeckPawn->SetHyperparameterAddress(HyperparameterBuffer);
		}
	}
}

void AHolodeckPawnController::ExecuteTeleport() {
	float* FloatPtr = static_cast<float*>(TeleportBuffer);
	AHolodeckAgent* PawnVar = Cast<AHolodeckAgent>(this->GetPawn());
	if (PawnVar && FloatPtr) {
		FVector TeleportLocation = FVector(FloatPtr[0], FloatPtr[1], FloatPtr[2]);
		PawnVar->Teleport(TeleportLocation);
	}
}

bool AHolodeckPawnController::CheckBoolBuffer(void* Buffer) {
	bool* BoolPtr = static_cast<bool*>(Buffer);
	if (BoolPtr && *BoolPtr) {
		*BoolPtr = false;
		return true;
	}
	return false;
}

void AHolodeckPawnController::SetServer(UHolodeckServer* ServerParam) {
	this->Server = ServerParam;
}

void AHolodeckPawnController::RestoreDefaultHyperparameters(){
	AHolodeckAgent* HolodeckPawn = static_cast<AHolodeckAgent*>(this->GetPawn());
	if(HolodeckPawn)
		FMemory::Memcpy(HyperparameterBuffer, HolodeckPawn->GetDefaultHyperparameters(), HolodeckPawn->GetHyperparameterCount() * sizeof(float));
}
