// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameInstance.h"

void UHolodeckGameInstance::StartServer() {
	Server = NewObject<UHolodeckServer>();
	Server->start();
}

UHolodeckServer* UHolodeckGameInstance::GetServer() {
	if (Server == nullptr) StartServer();
	return Server;
}

UHolodeckGameInstance::UHolodeckGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
}

void UHolodeckGameInstance::Tick(float DeltaTime) {
	static bool bFirstTime = true;
	if (bFirstTime)
		Server->start();
	else
		Server->release();
	Server->acquire();
	bFirstTime = false;
}

void UHolodeckGameInstance::Init(){
	Super::Init();

	// TODO: Ensure this code also gets called when a new level is loaded
	UWorld* world = GetWorld();
	if (world)
		WorldSettings = (AHolodeckWorldSettings*)GetWorld()->GetWorldSettings();
}