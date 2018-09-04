// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckPawnController.h"
#include "HolodeckAgent.h" //Must forward declare this so that you can access its teleport function. 

const FString CONTROL_SCHEME_KEY = "control_scheme";
const FString TELEPORT_BOOL_KEY = "teleport_flag";
const FString TELEPORT_COMMAND_KEY = "teleport_command";
const FString ROTATE_COMMAND_KEY = "rotation_command";
const FString HYPERPARAMETERS_KEY = "hyperparameters";


AHolodeckPawnController::AHolodeckPawnController(const FObjectInitializer& ObjectInitializer)
		: AHolodeckPawnControllerInterface(ObjectInitializer) {
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
	ControlledAgent = static_cast<AHolodeckAgentInterface*>(InPawn);
	if (ControlledAgent == nullptr)
		UE_LOG(LogHolodeck, Error, TEXT("HolodeckPawnController attached to non-HolodeckAgent!"));

	UE_LOG(LogHolodeck, Log, TEXT("Pawn Possessed: %s, Controlled by: %s"), *InPawn->GetHumanReadableName(), *this->GetClass()->GetName());
	UpdateServerInfo();
	if (Server == nullptr)
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckPawnController couldn't find server..."));

	URawControlScheme* RawControlScheme = NewObject<URawControlScheme>();
	RawControlScheme->Agent = ControlledAgent;
	ControlSchemes.Add(RawControlScheme);
	AddControlSchemes();
}

void AHolodeckPawnController::UnPossess() {
	Super::UnPossess();
}

void AHolodeckPawnController::Tick(float DeltaSeconds) {
	static int val = 0;
	Super::Tick(DeltaSeconds);

	if (ShouldTeleportBuffer && *ShouldTeleportBuffer) {
		ExecuteTeleport();
	}

	unsigned int index = *ControlSchemeIdBuffer % ControlSchemes.Num();
	ControlSchemes[index]->Execute(ControlledAgent->GetRawActionBuffer(), ActionBuffer, DeltaSeconds);
}

void* AHolodeckPawnController::Subscribe(const FString& AgentName, const FString& SensorName, int NumItems, int ItemSize) {
	UpdateServerInfo();
	UE_LOG(LogHolodeck, Log, TEXT("Subscribing sensor %s for %s"), *SensorName, *AgentName);
	if (Server == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("Sensor could not find server..."));
		return nullptr;
	} else {
		return Server->Malloc(UHolodeckServer::MakeKey(AgentName, SensorName), NumItems * ItemSize);
	}
}

void AHolodeckPawnController::UpdateServerInfo() {
	if (Server != nullptr) return;
	UHolodeckGameInstance* Instance = static_cast<UHolodeckGameInstance*>(GetGameInstance());
	if (Instance != nullptr)
		Server = Instance->GetServer();
	else
		UE_LOG(LogHolodeck, Warning, TEXT("Game Instance is not UHolodeckGameInstance."));
}

void AHolodeckPawnController::AllocateBuffers(const FString& AgentName) {
	UpdateServerInfo();
	if (Server != nullptr) {
		if (!ControlledAgent) {
			// LOG HERE
			return;
		}

		unsigned int MaxControlSize = 0;
		for (UHolodeckControlScheme* const ControlScheme : ControlSchemes) {
			if (ControlScheme->GetControlSchemeSizeInBytes() > MaxControlSize)
				MaxControlSize = ControlScheme->GetControlSchemeSizeInBytes();
		}

		void* TempBuffer;
		ActionBuffer = Server->Malloc(TCHAR_TO_UTF8(*AgentName),
									  MaxControlSize);

		TempBuffer = Server->Malloc(UHolodeckServer::MakeKey(AgentName, CONTROL_SCHEME_KEY),
											   sizeof(uint8));
		ControlSchemeIdBuffer = static_cast<uint8*>(TempBuffer);

		TempBuffer = Server->Malloc(UHolodeckServer::MakeKey(AgentName, TELEPORT_BOOL_KEY),
											  SINGLE_BOOL * sizeof(bool));
		ShouldTeleportBuffer = static_cast<uint8*>(TempBuffer);

		TempBuffer = Server->Malloc(UHolodeckServer::MakeKey(AgentName, TELEPORT_COMMAND_KEY),
										TELEPORT_COMMAND_SIZE * sizeof(float));
		TeleportBuffer = static_cast<float*>(TempBuffer);

		TempBuffer = Server->Malloc(UHolodeckServer::MakeKey(AgentName, ROTATE_COMMAND_KEY),
										ROTATE_COMMAND_SIZE * sizeof(float));
		RotationBuffer = static_cast<float*>(TempBuffer);

		TempBuffer = Server->Malloc(UHolodeckServer::MakeKey(AgentName, HYPERPARAMETERS_KEY),
			ControlledAgent->GetHyperparameterCount() * sizeof(float));
		HyperparameterBuffer = static_cast<float*>(TempBuffer);
		ControlledAgent->SetHyperparameterAddress(HyperparameterBuffer);
	}
}

void AHolodeckPawnController::ExecuteTeleport() {
	UE_LOG(LogHolodeck, Log, TEXT("Executing teleport"));
	AHolodeckAgent* PawnVar = Cast<AHolodeckAgent>(this->GetPawn());
	if (PawnVar == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("Couldn't get reference to controlled HolodeckAgent"));
		return;
	}

	FVector TeleportLocation;
	if (*ShouldTeleportBuffer & 0x1) {
		float* FloatPtr = static_cast<float*>(TeleportBuffer);
		TeleportLocation = FVector(FloatPtr[0], FloatPtr[1], FloatPtr[2]);
	} else {
		TeleportLocation = PawnVar->GetActorLocation();
	}

	FRotator NewRotation;
	if (*ShouldTeleportBuffer & 0x2) {
		float* FloatPtr = static_cast<float*>(RotationBuffer);
		NewRotation = FRotator(FloatPtr[0], FloatPtr[1], FloatPtr[2]);
	} else {
		NewRotation = PawnVar->GetActorRotation();
	}

	PawnVar->Teleport(TeleportLocation, NewRotation);
	*ShouldTeleportBuffer = 0;
}

bool AHolodeckPawnController::CheckBoolBuffer(void* Buffer) {
	bool* BoolPtr = static_cast<bool*>(Buffer);
	if (BoolPtr && *BoolPtr) {
		*BoolPtr = false;
		return true;
	}
	return false;
}

void AHolodeckPawnController::SetServer(UHolodeckServer* const ServerParam) {
	this->Server = ServerParam;
}

void AHolodeckPawnController::RestoreDefaultHyperparameters() {
	AHolodeckAgent* HolodeckPawn = static_cast<AHolodeckAgent*>(this->GetPawn());
	if(HolodeckPawn)
		FMemory::Memcpy(HyperparameterBuffer, HolodeckPawn->GetDefaultHyperparameters(), HolodeckPawn->GetHyperparameterCount() * sizeof(float));
}
