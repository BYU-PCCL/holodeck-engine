// Written by joshgreaves.

#pragma once

#include "Holodeck.h"

#include "Engine/GameInstance.h"
#include "HolodeckServer.h"
#include "HolodeckWorldSettings.h"

#include "HolodeckGameInstance.generated.h"

UCLASS()
class UHolodeckGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/**
	  * Default Constructor
	  */
	UHolodeckGameInstance(const FObjectInitializer& ObjectInitializer);

	/**
	  * GetServer
	  * StartServer must be called before this is called.
	  * @return a pointer to the server, or nullptr if Holodeck is off.
	  */
	UHolodeckServer* GetServer();

	/**
	  * StartServer
	  * Should only be called by HolodeckGameMode.
	  * Starts the server.
	  */
	void StartServer();

	/**
	  * Tick
	  * Ticks the game instance.
	  * Is called by HolodeckGameMode.
	  * @param DeltaTime the seconds passed since the last tick.
	  */
	void Tick(float DeltaTime);

	/**
	  * Init
	  * Starts the GameInstance.
	  */
	void Init();

private:
	UPROPERTY()
		UHolodeckServer* Server;
	AHolodeckWorldSettings* WorldSettings;
};
