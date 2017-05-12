// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameMode.h"


void AHolodeckGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (this->Instance)
		this->Instance->Tick(DeltaSeconds);
}

void AHolodeckGameMode::StartPlay() {
	this->Instance = (UHolodeckGameInstance*)(GetGameInstance());
	if (this->Instance) this->Instance->StartServer();
	else UE_LOG(LogHolodeck, Warning, TEXT("Game Instance couldn't be found and initialized"));
}

