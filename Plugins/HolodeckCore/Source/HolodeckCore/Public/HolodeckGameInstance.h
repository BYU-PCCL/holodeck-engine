// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "HolodeckServer.h"
#include "HolodeckMessages.h"
#include "HolodeckWorldSettings.h"
#include "HolodeckGameInstance.generated.h"

UCLASS()
class HOLODECKCORE_API UHolodeckGameInstance : public UGameInstance, public FTickFunction
{
	GENERATED_BODY()
	
private:

	struct FHolodeckGameInstanceTickFunction;

	FHolodeckGameInstanceTickFunction* TickFunction;
	
	AHolodeckWorldSettings* WorldSettings;

	UPROPERTY()
	UHolodeckServer* Server;
	
	int CurrentTickWait = -1;

	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> MessageEndpoint;

	struct FHolodeckGameInstanceTickFunction : public FTickFunction
	{
		UHolodeckGameInstance* GameInstance;

		virtual void ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;
	};

public:
	
	UHolodeckGameInstance();

	virtual void Shutdown();
	virtual void Init();
	void StartServer();
	void SetGamePaused(bool isPaused);

	void OnReceiveSimulatorCommand(const FHolodeckSimulatorCommand& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void OnReceiveCommandMessage(const FHolodeckCommandMessage& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
};


