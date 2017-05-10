// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameInstance.h"

UHolodeckGameInstance::UHolodeckGameInstance() {
	Server = &HolodeckServer::getInstance();
}

void UHolodeckGameInstance::Tick(float DeltaTime) {
	Server->release();
	Server->acquire();
}

void UHolodeckGameInstance::Init(){
	Super::Init();

	// TODO: Ensure this code also gets called when a new level is loaded
	UWorld* world = GetWorld();
	if (world)
		WorldSettings = (AHolodeckWorldSettings*)GetWorld()->GetWorldSettings();
}