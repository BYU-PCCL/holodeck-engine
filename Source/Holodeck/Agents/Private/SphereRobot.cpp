// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "SphereRobot.h"


// Sets default values
ASphereRobot::ASphereRobot() {
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ASphereRobot::Tick(float DeltaTime) {
	Super::Tick( DeltaTime );
}
