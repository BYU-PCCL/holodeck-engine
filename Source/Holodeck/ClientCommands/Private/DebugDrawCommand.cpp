// MIT License (c) 2019 BYU PCCL see LICENSE file

#include "Holodeck.h"
#include "DebugDrawCommand.h"
#include "HolodeckGameMode.h"

const float lifetime = 1.f;

void UDebugDrawCommand::Execute() {
	if (StringParams.size() != 0 || NumberParams.size() != 11) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in TeleportCameraCommand. Command not executed."));
		return;
	}

	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	UWorld* World = Game->GetWorld();

	/*
		NumberParams[0] is the type of debug object to draw. 0:line, 1:arrow, 2:box, 3:point
		NumberParams[1-3] are the start vector
		NumberParams[4-6] are the end vector
		NumberParams[7-9] are the RGB color values
		NumberParams[10] is the size
	*/
	FVector Vec1 = FVector(NumberParams[1], NumberParams[2], NumberParams[3]);
	Vec1 = ConvertLinearVector(Vec1, ClientToUE);
	FVector Vec2 = FVector(NumberParams[4], NumberParams[5], NumberParams[6]);
	Vec2 = ConvertLinearVector(Vec2, ClientToUE);
	FColor Color = FColor(NumberParams[7], NumberParams[8], NumberParams[9]);

	// Draw debug line
	if (NumberParams[0] == 0)
		DrawDebugLine(World, Vec1, Vec2, Color, false, lifetime, 0, NumberParams[10]);
	// Draw debug arrow
	else if(NumberParams[0] == 1)
		DrawDebugDirectionalArrow(World, Vec1, Vec2, (NumberParams[10]*10+Vec1.Dist(Vec1, Vec2)), Color, false, lifetime, 0, NumberParams[10]); // First float param is arrow size, second is thickness
	// Draw debug box
	else if (NumberParams[0] == 2)
		DrawDebugBox(World, Vec1, Vec2, Color, false, lifetime, 0, NumberParams[10]);
	// Draw debug point
	else if (NumberParams[0] == 3)
		DrawDebugPoint(World, Vec1, NumberParams[10], Color, false, lifetime, 0);
}