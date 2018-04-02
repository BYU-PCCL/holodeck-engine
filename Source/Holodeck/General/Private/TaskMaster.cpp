// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "TaskMaster.h"


// Sets default values
ATaskMaster::ATaskMaster() {
}

// Called when the game starts or when spawned
void ATaskMaster::BeginPlay() {
	Super::BeginPlay();
	for (AHolodeckTask* Task : Tasks) {
		bool init = FParse::Param(FCommandLine::Get(), *Task->TaskKey);
		Task->TaskInit(init);
	}
}
