// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "DebugDrawCommand.h"
#include "HolodeckGameMode.h"

void UDebugDrawCommand::Execute() {
	if (StringParams.size() != 0 || NumberParams.size() != 11) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in TeleportCameraCommand. Command not executed."));
		return;
	}

	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	UWorld* World = Game->GetWorld();

	FVector Vec1 = FVector(NumberParams[1], NumberParams[2], NumberParams[3]);
	FVector Vec2 = FVector(NumberParams[4], NumberParams[5], NumberParams[6]);
	FColor Color = FColor(NumberParams[7], NumberParams[8], NumberParams[9]);

	// Draw debug line
	if (NumberParams[0] == 0)
		DrawDebugLine(World, Vec1, Vec2, Color, false, 1.f, 0, NumberParams[10]);
	// Draw debug arrow
	else if(NumberParams[0] == 1)
		DrawDebugDirectionalArrow(World, Vec1, Vec2, NumberParams[10], Color, false, 1.f, 0, NumberParams[10]);
	// Draw debug box
	else if (NumberParams[0] == 2)
		DrawDebugBox(World, Vec1, Vec2, Color, false, 1.f, 0, NumberParams[10]);
	// Draw debug point
	else if (NumberParams[0] == 3)
		DrawDebugPoint(World, Vec1, NumberParams[10], Color, false, 1.f, 0);
}