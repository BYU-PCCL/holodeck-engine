// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckSphereRobotController.h"

AHolodeckSphereRobotController::AHolodeckSphereRobotController(const FObjectInitializer& ObjectInitializer)
		: AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("SphereRobot Controller Initialized"));
}

AHolodeckSphereRobotController::~AHolodeckSphereRobotController() {}

void AHolodeckSphereRobotController::Possess(APawn* Pawn) {
	Super::Possess(Pawn);
}

void AHolodeckSphereRobotController::ExecuteCommand() {
	ASphereRobot* SpherePawn = static_cast<ASphereRobot*>(this->GetPawn());
	float* FloatBuffer = static_cast<float*>(ActionBuffer);
	
	SpherePawn->ForwardSpeed = FloatBuffer[0];
	SpherePawn->RotSpeed = FloatBuffer[1];
}
