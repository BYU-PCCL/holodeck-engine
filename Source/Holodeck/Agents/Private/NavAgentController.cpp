// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "NavAgentController.h"

ANavAgentController::ANavAgentController(const FObjectInitializer& ObjectInitializer)
	: AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogHolodeck, Log, TEXT("NavAgent Controller Initialized"));
}

ANavAgentController::~ANavAgentController() {}
