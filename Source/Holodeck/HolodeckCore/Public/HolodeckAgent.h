// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Pawn.h"
#include "HolodeckPawnController.h"

#include "HolodeckAgent.generated.h"

/**
* AHolodeckAgent
* The base class for holodeck agents. 
* HolodeckAgents are what are controllable from the python side of the code. HolodeckAgents will 
* tell their controller to open up the channels needed for giving commands across the python binding.
*/
UCLASS()
class AHolodeckAgent : public APawn
{
	GENERATED_BODY()

public:
	/**
	  * Default Constructor
	  */
	AHolodeckAgent();

	/**
	  * BeginPlay
	  * Called when the game starts.
	  * Registers the reward and terminal signals.
	  */
	void BeginPlay() override;

	/**
	  * Tick
	  * Ticks the agent.
	  * If it is overridden, it must be called by the child class!
	  * @param DeltaSeconds the time since the last tick.
	  */
	void Tick(float DeltaSeconds) override;

	// Must be set in the editor.
	UPROPERTY(EditAnywhere)
	FString AgentName;

	/**
	  * SetReward
	  * Sets the reward in the server for this agent.
	  * @param Reward the value of the reward.
	  */
	void SetReward(int Reward) { if (RewardPtr != nullptr) *RewardPtr = Reward; };

	/**
	  * SetTerminal
	  * Sets the terminal in the server for this agent.
	  * @param Terminal the value of the terminal signal.
	  */
	void SetTerminal(bool Terminal) { if (TerminalPtr != nullptr) *TerminalPtr = Terminal; };

	/**
	  * Teleport
	  * Instantly moves the agent to target location, with the orientation that was given
	  * If no orientation was given, orientation remains unchanged (see overloaded function)
	  * @param NewLocation The location to move to
	  * @param NewRotation The rotation that the object will take on
	  * @return Bool if the teleport was successful.
	  */
	bool Teleport(const FVector& NewLocation, FRotator NewRotation);

	/**
	  * Teleport
	  * Instantly moves the agent to target location, with the orientation that was given
	  * Orientation remains unchanged
	  * @param NewLocation The location to move to
	  * @return Bool if the teleport was successful.
	  */
	bool Teleport(const FVector& NewLocation);

	/**
	  * GetSettingsBuffer
	  * Gets the settings buffer for this agent
	  */
	void GetSettingsBuffer();

	/**
	* UploadSettings 
	* This is called in HolodeckAgent::BeginPlay
	* In order to upload settings, this function must be overridden by derived classes. 
	* It is used to send all of the necessary data to the SettingsBuffer
	* For this function to be called, make sure to set bShouldExposeSettings to true. 
	*/
	virtual void UploadSettings() { check(0 && "You must override AHolodeckAgent::UploadSettings or set bShouldExposeSettings to false"); };

protected:
	/**
	* GetNumSettings
	* Derived classes must override this function.
	* It is used to represent the total number of settings that need 
	* to be uploaded to the python binding. 
	* @return The total number of settings.
	*/
	virtual int GetNumSettings() { check(0 && "You must override AHolodeckAgent::GetNumSettings or set bShouldExposeSettings to false"); return 0; };

	float* SettingsBuffer;

	bool bShouldExposeSettings = false; //The default is false. If you need to expose settings, make sure to set this to true before AHolodeckAgent::BeginPlay() is called. 
private:

	void GetServer();
	float* RewardPtr;
	bool* TerminalPtr;
	AHolodeckPawnController* HolodeckController;
	UHolodeckServer* Server;
};
