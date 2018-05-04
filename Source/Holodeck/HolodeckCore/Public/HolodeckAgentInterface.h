// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "HolodeckPawnController.h"

#include "HolodeckAgentInterface.generated.h"

const char REWARD_KEY[] = "Reward";
const int REWARD_SIZE = 1;
const char TERMINAL_KEY[] = "Terminal";
const int TERMINAL_SIZE = 1;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHolodeckAgentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOLODECK_API IHolodeckAgentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	* TeleportAndRotate
	* Instantly moves the agent to target location, with the orientation that was given
	* If no orientation was given, orientation remains unchanged (see overloaded function)
	* @param NewLocation The location to move to
	* @param NewRotation The rotation that the object will take on
	* @return Bool if the teleport was successful.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool TeleportAndRotate(const FVector& NewLocation, FRotator NewRotation);

	/**
	* Teleport
	* Instantly moves the agent to target location
	* Orientation remains unchanged
	* @param NewLocation The location to move to
	* @return Bool if the teleport was successful.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool Teleport(const FVector& NewLocation);

	/**
	* InitializeController
	* Hooks up everything with the controller. This is normally called in the beginPlay function,
	* but if you have to manually configure a controller, you will have to call this function after
	* you do it.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool InitializeController();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		FString GetAgentName();	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool SetAgentName(const FString& Name);

	/**
	* SpawnController
	* Spawns the controller fo the agent
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool SpawnController();

	/**
	* GetHolodeckPawnController
	* Returns the Pawn Controller for the agent
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		AHolodeckPawnController* GetHolodeckPawnController();

	/**
	* SetHolodeckPawnController
	* Sets the pawn controller for the agent
	* @param HolodeckController, the controller to assign to the agent
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool SetHolodeckPawnController(AHolodeckPawnController* HolodeckController);

	/**
	* SetTerminal
	* Sets the terminal in the server for this agent.
	* @param Terminal the value of the terminal signal.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool SetTerminal(bool Terminal);

	/**
	* SetReward
	* Sets the reward in the server for this agent.
	* @param Reward the value of the reward.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool SetReward(int Reward);
};
