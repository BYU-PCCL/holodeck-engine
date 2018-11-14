// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "SphereRobot.h"


// Sets default values
ASphereRobot::ASphereRobot() : ForwardSensitivity(10.0), RotSensitivity(5.0) {
	PrimaryActorTick.bCanEverTick = true;

	// Set the defualt controller
	AIControllerClass = LoadClass<AController>(NULL, TEXT("/Script/Holodeck.SphereRobotController"), NULL, LOAD_None, NULL);
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;
}

void ASphereRobot::InitializeAgent() {
	Super::InitializeAgent();
}

// Called every frame
void ASphereRobot::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	ForwardSpeed = CommandArray[0];
	RotSpeed = CommandArray[1];

	FVector DeltaLocation = GetActorForwardVector() * ForwardSpeed * ForwardSensitivity;
	AddActorWorldOffset(DeltaLocation, true);
	FRotator DeltaRotation(0, RotSpeed * RotSensitivity, 0);
	AddActorWorldRotation(DeltaRotation);
}
