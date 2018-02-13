// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckNavAgentController.h"

AHolodeckNavAgentController::AHolodeckNavAgentController(const FObjectInitializer& ObjectInitializer)
	: AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("NavAgent Controller Initialized"));
}

AHolodeckNavAgentController::~AHolodeckNavAgentController() {}

void AHolodeckNavAgentController::Possess(APawn* Pawn) {
	Super::Possess(Pawn);
}

void AHolodeckNavAgentController::ExecuteCommand() {
	ANavAgent* NavPawn = static_cast<ANavAgent*>(this->GetPawn());
	float* FloatBuffer = static_cast<float*>(ActionBuffer);
	if (NavPawn != nullptr && FloatBuffer != nullptr) {
		NavPawn->SetTarget(FloatBuffer[0], FloatBuffer[1], FloatBuffer[2]);
	}
}
