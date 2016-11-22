// Fill out your copyright notice in the Description page of Project Settings.

#include "HolodeckPrivatePCH.h"
#include "HolodeckServer.h"
#include "HolodeckMessages.h"
#include "HolodeckWorldSettings.h"
#include "HolodeckGameInstance.h"


UHolodeckGameInstance::UHolodeckGameInstance() {

}

void UHolodeckGameInstance::Shutdown() {
	// End the server
	if (Server) {
		Server->End();
		delete Server;
	}

	// Unregister ticker delegate
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);

	Super::Shutdown();
}

void UHolodeckGameInstance::StartServer() {
	//Start Server
	Server = NewObject<UHolodeckServer>();

	if (Server) {
		Server->Start();
	}
}


bool UHolodeckGameInstance::Tick(float DeltaTime) {

	//GEngine->AddOnScreenDebugMessage(-1, 2000.f, FColor::Purple, FString::Printf(TEXT("Ticking...")));

	if (this->WorldSettings->GetAllowedTicksBetweenCommands() >= 0) {
		this->CurrentTickWait -= 1;

		if (this->CurrentTickWait <= 0) {
			UE_LOG(LogHolodeck, Warning, TEXT("Pausing Game."));
			this->SetGamePaused(true);
		}
	}

	return true;
}

void UHolodeckGameInstance::Init(){

	// Register delegate for ticker callback
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UHolodeckGameInstance::Tick));

	Super::Init();
	
	MessageEndpoint = FMessageEndpoint::Builder("FHolodeckPawnGameInstanceMessageEndpoint")
		.Handling<FHolodeckSimulatorCommand>(this, &UHolodeckGameInstance::OnReceiveSimulatorCommand)
		.Handling<FHolodeckCommandMessage>(this, &UHolodeckGameInstance::OnReceiveCommandMessage);

	if (MessageEndpoint.IsValid())
	{
		MessageEndpoint->Subscribe<FHolodeckSimulatorCommand>();
		MessageEndpoint->Subscribe<FHolodeckCommandMessage>();
	}

	// TODO: Ensure this code also gets called when a new level is loaded
	UWorld* world = GetWorld();
	if (world) {
		WorldSettings = (AHolodeckWorldSettings*)GetWorld()->GetWorldSettings();
	}

	if(WorldSettings->GetAllowedTicksBetweenCommands() > 0){
		SetGamePaused(true);
	}
	
	StartServer();
}

void UHolodeckGameInstance::SetGamePaused(bool isPaused)
{
	UWorld* world = GetWorld();
	if (GEngine && world) {
		
		APlayerController* const LocalPlayer = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(world));
		if (LocalPlayer != NULL)
		{
			LocalPlayer->SetPause(isPaused);
		}
	}
}

void UHolodeckGameInstance::OnReceiveSimulatorCommand(const FHolodeckSimulatorCommand& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context) {

	if (Command.AllowedTicksBetweenCommands > 0) {
		WorldSettings->SetAllowedTicksBetweenCommands(Command.AllowedTicksBetweenCommands);
	}
	
	if (Command.TimeDeltaBetweenTicks > 0) {
		WorldSettings->SetConstantTimeDeltaBetweenTicks(Command.TimeDeltaBetweenTicks);
	}

	if (Command.Restart == true) {
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}

	if (Command.LoadLevel != "") {
		UGameplayStatics::OpenLevel(this, FName(*Command.LoadLevel), false);
	}
	
}

void UHolodeckGameInstance::OnReceiveCommandMessage(const FHolodeckCommandMessage& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context) {
	
	// Always unpause the game and reset the CurrentTickWait, even if we are not using them
	// this ensures that the game can't get "stuck" paused
	CurrentTickWait = WorldSettings->GetAllowedTicksBetweenCommands();
	SetGamePaused(false);

	
}