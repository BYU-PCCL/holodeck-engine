// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckUAVController.h"

AHolodeckUAVController::AHolodeckUAVController(const FObjectInitializer& ObjectInitializer)
		: AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("UAV Controller Initialized"));
}

AHolodeckUAVController::~AHolodeckUAVController() {}

void AHolodeckUAVController::ExecuteCommand() {
	float* FloatPtr = static_cast<float*>(ActionBuffer);
	AUAV* UAVPawn = Cast<AUAV>(GetPawn());
	if (UAVPawn != nullptr && FloatPtr != nullptr) {
		UAVPawn->DesiredRoll = FloatPtr[0]; // Command.Roll;
		UAVPawn->DesiredPitch = FloatPtr[1]; // Command.Pitch;
		UAVPawn->DesiredYawRate = FloatPtr[2]; // Command.YawRate;
		UAVPawn->DesiredAltitude = FloatPtr[3]; // Command.Altitude;

	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("UAV controller could not find the UAV."));
	}
}
