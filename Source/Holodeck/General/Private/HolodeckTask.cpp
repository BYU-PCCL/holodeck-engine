// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckTask.h"


// Sets default values
AHolodeckTask::AHolodeckTask() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
	MainAgent = nullptr;
}

// Called when the game starts or when spawned
void AHolodeckTask::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AHolodeckTask::Tick(float DeltaTime) {
	Super::Tick( DeltaTime );
	if (MainAgent != nullptr) {
		MainAgent->SetReward(Reward);
		MainAgent->SetTerminal(Terminal);
	}
}
