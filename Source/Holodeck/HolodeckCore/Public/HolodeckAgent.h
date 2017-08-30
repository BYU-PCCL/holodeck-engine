// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Pawn.h"
#include "HolodeckPawnController.h"

#include "HolodeckAgent.generated.h"

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

private:
	float* RewardPtr;
	bool* TerminalPtr;
	AHolodeckPawnController* HolodeckController;
};
