// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckNavAgentController.h"

AHolodeckNavAgentController::AHolodeckNavAgentController(const FObjectInitializer& ObjectInitializer)
	: AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogHolodeck, Log, TEXT("NavAgent Controller Initialized"));
}

AHolodeckNavAgentController::~AHolodeckNavAgentController() {}

void AHolodeckNavAgentController::Possess(APawn* PawnParam) {
	Super::Possess(PawnParam);
}
