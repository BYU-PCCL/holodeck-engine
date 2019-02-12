// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "TurtleAgent.h"


// Sets default values
ATurtleAgent::ATurtleAgent() : ForwardSensitivity(10.0), RotSensitivity(5.0) {
	PrimaryActorTick.bCanEverTick = true;

	// Set the defualt controller
	AIControllerClass = LoadClass<AController>(NULL, TEXT("/Script/Holodeck.TurtleAgentController"), NULL, LOAD_None, NULL);
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;
}

void ATurtleAgent::InitializeAgent() {
	Super::InitializeAgent();
	RootMesh = Cast<UStaticMeshComponent>(RootComponent);
}

// Called every frame
void ATurtleAgent::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	ForwardForce = CommandArray[0];
	RotForce = CommandArray[1];
	float maxThrust = 15.0f;
	float maxYaw = 15.0f;

	//TODO make numbers not magic
	float ThrustToApply = FMath::Clamp(ForwardForce, -maxThrust, maxThrust);
	float YawTorqueToApply = FMath::Clamp(RotForce, -maxYaw, maxYaw);

	FVector LocalThrust = FVector(ThrustToApply * 1000, 0, 0);
	FVector LocalTorque = FVector(0, 0, YawTorqueToApply * 1000);

	RootMesh->AddTorque(GetActorRotation().RotateVector(LocalTorque));
	RootMesh->AddForce(GetActorRotation().RotateVector(LocalThrust));
}



