// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "HolodeckServer.h"
#include "HolodeckMessages.h"
#include "HolodeckWorldSettings.h"
#include "HolodeckGameInstance.generated.h"

UCLASS()
class HOLODECKCORE_API UHolodeckGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:

	bool Tick(float DeltaSeconds);

	FDelegateHandle TickDelegateHandle;
	
	AHolodeckWorldSettings* WorldSettings;

	UPROPERTY()
	UHolodeckServer* Server;
	
	int CurrentTickWait = -1;

	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> MessageEndpoint;

public:
	
	UHolodeckGameInstance();

	virtual void Shutdown();
	virtual void Init();
	void StartServer();
	void SetGamePaused(bool isPaused);

	void OnReceiveSimulatorCommand(const FHolodeckSimulatorCommand& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void OnReceiveCommandMessage(const FHolodeckCommandMessage& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
};


