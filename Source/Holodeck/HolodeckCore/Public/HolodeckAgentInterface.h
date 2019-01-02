// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Pawn.h"

#include "HolodeckAgentInterface.generated.h"

/**
* AHolodeckAgentInterface
* The base class for holodeck agents.
* HolodeckAgents are controllable from python.
* A HolodeckAgent contains the logic for how it acts at a low level.
* To get to a higher level of control, the logic should be implemented in
* a HolodeckControlScheme.
*/
UCLASS()
class HOLODECK_API AHolodeckAgentInterface : public APawn {
	GENERATED_BODY()

public:
	/**
	* BeginPlay
	* Called when the game starts.
	*/
	virtual void BeginPlay() override { Super::BeginPlay(); };

	/**
	* Tick
	* Ticks the agent.
	* If it is overridden, it must be called by the child class!
	* @param DeltaSeconds the time since the last tick.
	*/
	virtual void Tick(float DeltaSeconds) override { Super::Tick(DeltaSeconds); };

	/**
	* SetReward
	* Sets the reward in the server for this agent.
	* @param Reward the value of the reward.
	*/
	virtual void SetReward(float Reward) {
		check(0 && "You must override SetReward");
	};

	/**
	* SetTerminal
	* Sets the terminal in the server for this agent.
	* @param Terminal the value of the terminal signal.
	*/
	virtual void SetTerminal(bool bTerminal) {
		check(0 && "You must override SetTerminal");
	};

	/**
	* SetState
	*/
	virtual bool SetState(const FVector& NewLocation, const FRotator& NewRotation, const FVector& NewVelocity, const FVector& NewAngVelocity){
		check(0 && "You must override SetState");
		return false;
	};

	/**
	* Teleport
	* Instantly moves the agent to target location, with the orientation that was given
	* If no orientation was given, orientation remains unchanged (see overloaded function)
	* @param NewLocation The location to move to
	* @param NewRotation The rotation that the object will take on
	* @return Bool if the teleport was successful.
	*/
	virtual bool Teleport(const FVector& NewLocation, const FRotator& NewRotation) {
		check(0 && "You must override Teleport");
		return false;
	};



	/**
	* Teleport
	* Instantly moves the agent to target location, with the orientation that was given
	* Orientation remains unchanged
	* @param NewLocation The location to move to
	* @return Bool if the teleport was successful.
	*/
	virtual bool Teleport(const FVector& NewLocation) {
		check(0 && "You must override Teleport");
		return false;
	};


	/**
	* InitializeController
	* Hooks up everything with the controller. This is normally called in the beginPlay function,
	* but if you have to manually configure a controller, you will have to call this function after
	* you do it.
	*/
	virtual bool InitializeController() {
		check(0 && "You must override InitializeController");
		return false;
	};

	/**
	* GetRawActionSizeInBytes
	* @return the number of bytes used by the action space.
	*/
	virtual unsigned int GetRawActionSizeInBytes() const {
		check(0 && "You must override GetRawActionSizeInBytes");
		return 0;
	};

	/**
	* GetRawActionBuffer
	* @return a pointer to the start of the action buffer.
	*/
	virtual void* GetRawActionBuffer() const {
		check(0 && "You must override GetRawActionBuffer");
		return nullptr;
	};

	// Must be set in the editor.
	UPROPERTY(EditAnywhere)
		FString AgentName;
};
