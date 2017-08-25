// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "SphereRobot.h"
#include "HoldeckSphereRobotController.h"

AHoldeckSphereRobotController::AHoldeckSphereRobotController(const FObjectInitializer& ObjectInitializer) : AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("SphereRobot Controller Initialized"));
}

AHoldeckSphereRobotController::~AHoldeckSphereRobotController() {

}

void AHoldeckSphereRobotController::Possess(APawn* pawn) {
	Super::Possess(pawn);

	//TArray<UStaticMeshComponent*> components;
	//Pawn->GetComponents<UStaticMeshComponent>(components);
	//StaticMesh = components[0];
}

void AHoldeckSphereRobotController::OnReceiveCommand(const FHolodeckSphereRobotCommand& Command) {

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

}
