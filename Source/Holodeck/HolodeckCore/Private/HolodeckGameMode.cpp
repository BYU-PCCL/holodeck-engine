// Written by joshgreaves.

#include "Holodeck.h"
#include "HolodeckGameMode.h"

const char RESET_KEY[] = "RESET";
const int RESET_BYTES = 1;

AHolodeckGameMode::AHolodeckGameMode(const FObjectInitializer& ObjectInitializer) : AGameMode(ObjectInitializer), bHolodeckIsOff(true) {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	UE_LOG(LogHolodeck, Log, TEXT("HolodeckGameMode initialized"));
}

void AHolodeckGameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	// If !bHolodeckIsOff, then we never got instance or reset signal,
	// so we don't need to check bOn here.
	if (this->Instance)
		this->Instance->Tick(DeltaSeconds);
	if (this->CommandCenter)
		this->CommandCenter->Tick(DeltaSeconds);
	//Check if we should reset, and then reset the level. 
	if (ResetSignal != nullptr && *ResetSignal) {
		UGameplayStatics::OpenLevel(this->Instance, FName(*GetWorld()->GetName()), false);
		*ResetSignal = false;
	}

}

void AHolodeckGameMode::StartPlay() {
	UE_LOG(LogHolodeck, Log, TEXT("HolodeckGameMode starting play"));

	// To prevent crashing in standalone games, check the HolodeckOn command is supplied.
	// This overrides the bHolodeckIsOff value supplied in the editor.
	if (GetWorld()->WorldType == EWorldType::Game)
		bHolodeckIsOff = FParse::Param(FCommandLine::Get(), TEXT("HolodeckOff"));

	if (!bHolodeckIsOff) {
		this->Instance = (UHolodeckGameInstance*)(GetGameInstance());
		if (this->Instance) {
			this->Instance->StartServer();
			Server = this->Instance->GetServer();

			RegisterSettings();
		} else {
			UE_LOG(LogHolodeck, Warning, TEXT("Game Instance couldn't be found and initialized"));
		}
		if (this->Server) {
			this->CommandCenter = NewObject<UCommandCenter>();
			CommandCenter->Init(Server, this);
		}
	}

	Super::StartPlay();
}

void AHolodeckGameMode::RegisterSettings() {
	UE_LOG(LogHolodeck, Log, TEXT("Registering Settings"));
	if (Server != nullptr) {
		ResetSignal = static_cast<bool*>(Server->SubscribeSetting(RESET_KEY, RESET_BYTES));
		UE_LOG(LogHolodeck, Log, TEXT("Reset signal registered"));
	}
}
