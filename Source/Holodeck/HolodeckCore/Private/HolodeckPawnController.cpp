// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAgent.h"
#include "HolodeckPawnController.h"
// TODO: Move publishing messages to a class that can be composed with actors who need additional controllers (like a PlayerController)
AHolodeckPawnController::AHolodeckPawnController(const FObjectInitializer& ObjectInitializer) : AAIController(ObjectInitializer) {
	Server = &HolodeckServer::getInstance();
}

AHolodeckPawnController::~AHolodeckPawnController() {
}

void AHolodeckPawnController::Possess(APawn* InPawn) {
	Super::Possess(InPawn);
	UE_LOG(LogHolodeck, Warning, TEXT("Pawn Possessed: %s, Controlled by: %s"), *InPawn->GetHumanReadableName(), *this->GetClass()->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("Pawn Possessed: %s, Controlled by: %s"), *InPawn->GetHumanReadableName(), *this->GetClass()->GetName()));
}

void AHolodeckPawnController::UnPossess() {
	Super::UnPossess();
}

void AHolodeckPawnController::Publish(FHolodeckSensorData& data) {
	Server->setSensor(data.AgentName, data.Key, TCHAR_TO_UTF8(*data.Data));
}

void AHolodeckPawnController::Subscribe(FHolodeckSensorData& Data, int Length) {
	Server->subscribeSensor(Data.AgentName, Data.Key, Length);
}