// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "ChangeFogDensityCommand.h"

void UChangeFogDensityCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("ChangeFogDensityCommand::Execute change fog density"));

	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	bool success = Game->ChangeFogDensity(NumberParams[0]);
}
