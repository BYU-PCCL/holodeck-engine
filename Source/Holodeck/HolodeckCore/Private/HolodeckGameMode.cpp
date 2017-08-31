// Written by joshgreaves.

#include "Holodeck.h"
#include "HolodeckGameMode.h"

const char RESET_KEY[] = "RESET";
const int RESET_BYTES = 1;

AHolodeckGameMode::AHolodeckGameMode(const FObjectInitializer& ObjectInitializer) : AGameMode(ObjectInitializer), bHolodeckIsOn(true) {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}

void AHolodeckGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// If !bOn, then we never got instance or reset signal,
	// so we don't need to check bOn here.
	if (this->Instance)
		this->Instance->Tick(DeltaSeconds);

	if (ResetSignal != nullptr && *ResetSignal) {
		UGameplayStatics::OpenLevel(this->Instance, FName(*GetWorld()->GetName()), false);
		*ResetSignal = false;
	}

}

void AHolodeckGameMode::StartPlay() {
	if (bHolodeckIsOn) {
		this->Instance = (UHolodeckGameInstance*)(GetGameInstance());
		if (this->Instance) {
			this->Instance->StartServer();
			Server = this->Instance->GetServer();

			RegisterSettings();
		} else {
			UE_LOG(LogHolodeck, Warning, TEXT("Game Instance couldn't be found and initialized"));
		}
	}

	Super::StartPlay();
}

void AHolodeckGameMode::RegisterSettings() {
	if (Server != nullptr) {
		ResetSignal = static_cast<bool*>(Server->SubscribeSetting(RESET_KEY, RESET_BYTES));
	}
}
