#include "Holodeck.h"
#include "PixelCameraRateCommand.h"

void UPixelCameraRateCommand::Execute() {

	UE_LOG(LogHolodeck, Log, TEXT("PixelCameraRateCommand::Execute set camera capture rate"));
	//Program should throw an error if any of these params aren't the correct size. They should always be this size.
	if (StringParams.size() != 1 || NumberParams.size() != 1) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in PixelCameraRateCommand. Camera Rate not adjusted."));
		return;
	}

	AHolodeckGameMode* GameTarget = static_cast<AHolodeckGameMode*>(Target);
	if (GameTarget == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("UCommand::Target is not a UHolodeckGameMode*. PixelCameraRateCommand::Execute Camera Rate not adjusted."));
		return;
	}

	UWorld* World = Target->GetWorld();
	if (World == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("PixelCameraRateCommand::Execute found world as nullptr. Camera Rate not adjusted."));
		return;
	}

	FString AgentName = StringParams[0].c_str();
	int ticksPerCapture = NumberParams[0];
}
