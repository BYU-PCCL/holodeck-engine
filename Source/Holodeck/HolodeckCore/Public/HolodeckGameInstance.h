// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "HolodeckServer.h"
#include "HolodeckMessages.h"
#include "HolodeckWorldSettings.h"
#include "HolodeckGameInstance.generated.h"

UCLASS()
class UHolodeckGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:

	bool bIsRunning;
	
	AHolodeckWorldSettings* WorldSettings;


	UPROPERTY()
	UHolodeckServer* Server;

public:

	UHolodeckServer* GetServer();
	void StartServer();

	UHolodeckGameInstance(const FObjectInitializer& ObjectInitializer);

	void Tick(float DeltaTime);
	void Init();

};


