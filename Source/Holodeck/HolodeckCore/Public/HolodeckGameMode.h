// Written by joshgreaves.

#pragma once

#include "Holodeck.h"

#include "GameFramework/GameMode.h"
#include "HolodeckGameInstance.h"
#include "CommandCenter.h"
#include "WeatherMaster.h"
#include "HolodeckGameMode.generated.h"

/**
 * AHolodeckGameMode
 * The base game mode for Holodeck.
 * HolodeckGameModeBP can be used to turn on and off Holodeck functionality
 * from the editor.
 */
UCLASS()
class AHolodeckGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	/**
	  * Default Constructor.
	  */
	explicit AHolodeckGameMode(const FObjectInitializer& ObjectInitializer);

	/**
	  * Tick
	  * Ticks the game mode.
	  * Checks the change in settings, and ticks the game instance.
	  * @param DeltaSeconds how many seconds passed since last tick.
	  */
	void Tick(float DeltaSeconds) override;

	/**
	  * StartPlay
	  * Called when the game begins.
	  * Registers all settings.
	  */
	void StartPlay() override;

	// Can be set off to turn off holodeck functionality.
	UPROPERTY(EditAnywhere)
	bool bHolodeckIsOn;

	/**
	  * GetAssociatedServer
	  * Returns the private server pointer that the instance contains.
	  */
	UHolodeckServer* GetAssociatedServer() { return this->Server; };

	// These functions allow the Holodeck to do things which cannot normally be done from pure c++ code
	UFUNCTION(BlueprintImplementableEvent)
		AHolodeckAgent* SpawnAgent(const FString& Type, const FVector& Location);
	UPROPERTY(BlueprintReadWrite)
		AWeatherMaster* WeatherMaster;

private:
	/**
	  * RegisterSettings
	  * Registers all the settings on the server.
	  */
	void RegisterSettings();

	// Setting buffers
	bool* ResetSignal;

	UPROPERTY()
	UHolodeckServer* Server;
	UPROPERTY()
	UHolodeckGameInstance* Instance;
	UPROPERTY()
	UCommandCenter* CommandCenter;
};
