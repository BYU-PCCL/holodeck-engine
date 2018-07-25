// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "ChangeFogDensityCommand.h"

void UChangeFogDensityCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("ChangeFogDensityCommand::Execute change fog density"));

	if (StringParams.size() != 0 || NumberParams.size() != 1) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in UChangeFogDensityCommand. Command not executed."));
		return;
	}

	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	bool success = Game->ChangeFogDensity(NumberParams[0]);

	if (!success) {
		UE_LOG(LogHolodeck, Warning, TEXT("ChangeFogDensityCommand failed!"));
		return;
	}
	else {
		UE_LOG(LogHolodeck, Log, TEXT("ChangeFogDensityCommand was successful"));
	}
}
