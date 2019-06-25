// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "AndroidController.h"

AAndroidController::AAndroidController(const FObjectInitializer& ObjectInitializer) : AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogHolodeck, Log, TEXT("Android Controller Initialized"));
}

AAndroidController::~AAndroidController() {}

void AAndroidController::OnPossess(APawn* PawnParam) {
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
