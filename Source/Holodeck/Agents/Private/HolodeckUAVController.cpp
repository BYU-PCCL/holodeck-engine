// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckUAVController.h"

AHolodeckUAVController::AHolodeckUAVController(const FObjectInitializer& ObjectInitializer)
		: AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("UAV Controller Initialized"));
}

AHolodeckUAVController::~AHolodeckUAVController() {}
