// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "TurtleAgentController.h"

ATurtleAgentController::ATurtleAgentController(const FObjectInitializer& ObjectInitializer)
	: AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("TurtleAgent Controller Initialized"));
}

ATurtleAgentController::~ATurtleAgentController() {}

void ATurtleAgentController::OnPossess(APawn* PawnParam) {
	Super::Possess(PawnParam);
}
