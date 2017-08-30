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
	// TODO(joshgreaves) : Fix the Sphere robot and get this working.
	/*
	UE_LOG(LogTemp, Warning, TEXT("Received command to SphereRobot"));

	ASphereRobot* pawn = (ASphereRobot*) this->GetPawn();
	
	if (Command.Forward) {
		pawn->ForwardSpeed = Command.Forward;
	}
	else {
		pawn->ForwardSpeed = 0;
	}

	if (Command.Right) {
		pawn->RotSpeed = Command.Right;
	}
	else {
		pawn->RotSpeed = 0;
	}
	*/
}
