// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckSphereRobotController.h"

AHolodeckSphereRobotController::AHolodeckSphereRobotController(const FObjectInitializer& ObjectInitializer)
		: AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("SphereRobot Controller Initialized"));
}

AHolodeckSphereRobotController::~AHolodeckSphereRobotController() {}

void AHolodeckSphereRobotController::Possess(APawn* PawnParam) {
	Super::Possess(PawnParam);
}
