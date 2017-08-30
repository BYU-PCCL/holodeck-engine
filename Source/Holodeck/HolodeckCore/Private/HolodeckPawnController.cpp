// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckPawnController.h"

AHolodeckPawnController::AHolodeckPawnController(const FObjectInitializer& ObjectInitializer)
	: AAIController(ObjectInitializer) { }

AHolodeckPawnController::~AHolodeckPawnController() { }

void AHolodeckPawnController::Possess(APawn* InPawn) {
	Super::Possess(InPawn);
	UE_LOG(LogHolodeck, Warning, TEXT("Pawn Possessed: %s, Controlled by: %s"), *InPawn->GetHumanReadableName(), *this->GetClass()->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("Pawn Possessed: %s, Controlled by: %s"), *InPawn->GetHumanReadableName(), *this->GetClass()->GetName()));
	GetServer();

	if (Server == nullptr)
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckPawnController couldn't find server..."));
}

void AHolodeckPawnController::UnPossess() {
	Super::UnPossess();
}

void* AHolodeckPawnController::Subscribe(const FString& AgentName, const FString& SensorName, int NumItems, int ItemSize) {
	GetServer();

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

void AHolodeckPawnController::GetActionBuffer(const FString& AgentName) {
	if (Server != nullptr)
		ActionBuffer = Server->SubscribeActionSpace(TCHAR_TO_UTF8(*AgentName), GetActionSpaceDimension() * sizeof(float));
}
