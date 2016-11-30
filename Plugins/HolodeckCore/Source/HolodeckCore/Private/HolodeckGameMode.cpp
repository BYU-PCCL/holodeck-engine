// Fill out your copyright notice in the Description page of Project Settings.

#include "HolodeckPrivatePCH.h"
#include "HolodeckGameMode.h"


void AHolodeckGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UHolodeckGameInstance* GameInstance = Cast<UHolodeckGameInstance>(GetGameInstance());
	if (GameInstance)
		GameInstance->Tick(DeltaSeconds);

}

