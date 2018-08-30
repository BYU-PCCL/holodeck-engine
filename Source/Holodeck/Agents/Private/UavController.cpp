// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "UavController.h"

AUavController::AUavController(const FObjectInitializer& ObjectInitializer)
		: AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("UAV Controller Initialized"));
}

AUavController::~AUavController() {}
