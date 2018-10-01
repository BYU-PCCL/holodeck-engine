// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "SphereRobot.h"


// Sets default values
ASphereRobot::ASphereRobot() {
	PrimaryActorTick.bCanEverTick = true;

	// Set the defualt controller
	AIControllerClass = LoadClass<AController>(NULL, TEXT("/Script/Holodeck.SphereRobotController"), NULL, LOAD_None, NULL);
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;
}

void ASphereRobot::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ASphereRobot::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	ForwardSpeed = CommandArray[0];
	RotSpeed = CommandArray[1];
}
