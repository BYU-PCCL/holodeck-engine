// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAndroidController.h"

AHolodeckAndroidController::AHolodeckAndroidController(const FObjectInitializer& ObjectInitializer) : AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("Android Controller Initialized"));
}

AHolodeckAndroidController::~AHolodeckAndroidController() {}

void AHolodeckAndroidController::Possess(APawn* PawnParam) {
	Super::Possess(PawnParam);

	TArray<USkeletalMeshComponent*> Components;
	PawnParam->GetComponents<USkeletalMeshComponent>(Components);
	SkeletalMeshComponent = Components[0];
}

void AHolodeckAndroidController::ExecuteCommand() {

	float* FloatPtr = static_cast<float*>(ActionBuffer);

	AAndroid* AndroidPawn = Cast<AAndroid>(GetPawn());
	if (AndroidPawn != nullptr && FloatPtr != nullptr) {
		AndroidPawn->CommandArray = FloatPtr;
	}
}