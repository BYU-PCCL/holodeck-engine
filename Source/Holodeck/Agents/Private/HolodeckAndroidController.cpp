// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAndroidController.h"

AHolodeckAndroidController::AHolodeckAndroidController(const FObjectInitializer& ObjectInitializer) : AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogHolodeck, Log, TEXT("Android Controller Initialized"));
}

AHolodeckAndroidController::~AHolodeckAndroidController() {}

void AHolodeckAndroidController::Possess(APawn* PawnParam) {
	Super::Possess(PawnParam);
	UE_LOG(LogHolodeck, Log, TEXT("Android Controller possessing pawn"));

	TArray<USkeletalMeshComponent*> Components;
	PawnParam->GetComponents<USkeletalMeshComponent>(Components);

	if (Components.Num() < 1) {
		UE_LOG(LogHolodeck, Error, TEXT("Couldn't find USkeletelMeshComponent for PawnParam"));
	} else {
		SkeletalMeshComponent = Components[0];
	}

	ActionBufferFloatPtr = static_cast<float*>(ActionBuffer);
}
