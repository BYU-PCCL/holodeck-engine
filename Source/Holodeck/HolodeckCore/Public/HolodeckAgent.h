// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Pawn.h"
#include "HolodeckAgentInterface.h"
#include "HolodeckPawnControllerInterface.h"

#include "HolodeckAgent.generated.h"

/**
* AHolodeckAgent
* The base class for holodeck agents. 
* HolodeckAgents are controllable from python.
* A HolodeckAgent contains the logic for how it acts at a low level.
* To get to a higher level of control, the logic should be implemented in
* a HolodeckControlScheme.
*/
UCLASS()
class HOLODECK_API AHolodeckAgent : public AHolodeckAgentInterface {
	GENERATED_BODY()

public:
	/**
	  * Default Constructor
	  */
	AHolodeckAgent();

	/**
	  * BeginPlay
	  * Called when the game starts.
	  */
	virtual void BeginPlay() override;

	/**
	  * Tick
	  * Ticks the agent.
	  * If it is overridden, it must be called by the child class!
	  * @param DeltaSeconds the time since the last tick.
	  */
	virtual void Tick(float DeltaSeconds) override;

	/**
	  * SetReward
	  * Sets the reward in the server for this agent.
	  * @param Reward the value of the reward.
	  */
	void SetReward(float Reward) override;

	/**
	  * SetTerminal
	  * Sets the terminal in the server for this agent.
	  * @param Terminal the value of the terminal signal.
	  */
	void SetTerminal(bool bTerminal) override;

	/**
	  * Teleport
	  * Instantly moves the agent to target location, with the orientation that was given
	  * If no orientation was given, orientation remains unchanged (see overloaded function)
	  * @param NewLocation The location to move to
	  * @param NewRotation The rotation that the object will take on
	  * @return Bool if the teleport was successful.
	  */
	bool Teleport(const FVector& NewLocation, const FRotator& NewRotation) override;

	/**
	  * Teleport
	  * Instantly moves the agent to target location, with the orientation that was given
	  * Orientation remains unchanged
	  * @param NewLocation The location to move to
	  * @return Bool if the teleport was successful.
	  */
	bool Teleport(const FVector& NewLocation) override;

	/**
	  * SetHyperparameterAddress
	  * Sets the where the Hyperparameters pointer points to
	  * You must give it a pointer to a place that has the proper memory allocated for it
	  * @param Input The pointer
	  */
	void SetHyperparameterAddress(float* Input) override;

	/**
	  * GetHyperparameterCount
	  * @return The total number of Hyper parameters.
	  */
	int GetHyperparameterCount() const override { return 1; };
	
	/**
	  * GetHyperparameters
	  * This function is pointer safe, you can't access a bad pointer with it unless you 
	  * gave it a bad pointer to point to via SetHyperparameterAddress().
	  * @return A const pointer to the Hyperparameters Array.
	  */
	virtual const float* GetHyperparameters() override;

	/**
	* InitializeController
	* Hooks up everything with the controller. This is normally called in the beginPlay function,
	* but if you have to manually configure a controller, you will have to call this function after
	* you do it.
	*/
	bool InitializeController() override;

	/**
	* GetDefaultHyperparameters
	* You must override this function iff GetHyperparameterCount() does not return 1 (the default value)
	* @return a const pointer to the default hyperParameters
	*/
	virtual const float* GetDefaultHyperparameters() const override;

	/**
	  * GetRawActionSizeInBytes
	  * @return the number of bytes used by the action space.
	  */
	virtual unsigned int GetRawActionSizeInBytes() const override {
		check(0 && "You must override GetRawActionSizeInBytes");
	};

	/**
	  * GetRawActionBuffer
	  * @return a pointer to the start of the action buffer.
	  */
	virtual void* GetRawActionBuffer() const override {
		check(0 && "You must override GetRawActionBuffer");
	};
private:
	float* RewardPtr;
	bool* TerminalPtr;
	const float* Hyperparameters;
	AHolodeckPawnControllerInterface* HolodeckController;
};
