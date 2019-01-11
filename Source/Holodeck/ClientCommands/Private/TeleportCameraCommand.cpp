#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "TeleportCameraCommand.h"

void UTeleportCameraCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("TeleportCameraCommand::Execute teleport camera command"));

	if (StringParams.size() != 0) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in TeleportCameraCommand. Command not executed."));
		return;
	}

	UWorld* World = Target->GetWorld();
	float UnitsToMeter = World->GetWorldSettings()->WorldToMeters;
	FVector Location = FVector(NumberParams[0], NumberParams[1], NumberParams[2]) * UnitsToMeter;
	FVector Rotation = FVector(NumberParams[3], NumberParams[4], NumberParams[5]);

	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);

	Game->TeleportCamera(Location, Rotation);
}
