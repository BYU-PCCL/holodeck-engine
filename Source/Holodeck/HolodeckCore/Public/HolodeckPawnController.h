// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "AIController.h"
#include "HolodeckGameInstance.h"
#include "HolodeckServer.h"

#include "HolodeckPawnController.generated.h"

/**
  * AHolodeckPawnController
  * A controller for Holodeck Agents.
  * If a HolodeckAgent doesn't contain this controller or a controller which
  * inherits it, then you will run into problems. This class handles the 
  * subscribing of sensors and setting up action buffer channels. The 
  * sensors themselves tell the controller to subscribe them. Its base 
  * classes must report what size of action buffer they need. 
  * HolodeckPawnControllers get the data from the shared memory and give
  * the commands to the pawns/agents. 
  */
UCLASS()
class AHolodeckPawnController : public AAIController
{
	GENERATED_BODY()

public:
	/**
	  * Default Constructor
	  */
	AHolodeckPawnController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	  * Default Destructor
	  */
	~AHolodeckPawnController();

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
	
	/**
	  * Possess
	  * Called to Possess a pawn.
	  */
	void Possess(APawn* InPawn) override;

	/**
	  * UnPossess
	  * Called to Unpossess a pawn
	  */
	void UnPossess() override;

	/**
	  * Subscribe
	  * Subscribes a sensor on the HolodeckServer.
	  * @param AgentName the name of the agent the sensor is being registered for.
	  * @param SensorName the name of the sensor to register.
	  * @param NumItems the number of items in the data buffer.
	  * @param ItemSize the size of each item in the data buffer.
	  * @return a pointer to the data buffer.
	  */
	void* Subscribe(const FString& AgentName, const FString& SensorName, int NumItems, int ItemSize);

	/**
	  * GetActionBuffer
	  * Gets the action buffer for this agent.
	  * @param AgentName the name of the agent to subscribe an action buffer for.
	  */
	void GetBuffers(const FString& AgentName);

	/**
	  * ExecuteCommand
	  * Carries out the command for the agent, as contained in the action buffer.
	  * Must be overidden by the child class.
	  * Is called from the tick on the controlled pawn.
	  */
	virtual void ExecuteCommand() { check(0 && "You must override ExecuteCommand"); };

	virtual void ExecuteTeleport();

protected:
	/**
	  * GetActionSpaceDimension
	  * Gets the dimension of the action space.
	  * @return the size of the action space.
	  */
	virtual int GetActionSpaceDimension() { check(0 && "You must override GetActionSpaceDimension"); return 0; };

	void* ActionBuffer;
	void* TeleportBuffer;
	void* ShouldTeleportBuffer;

private:
	/**
	* GetServer
	* Sets the server object within this object.
	*/
	void GetServer();

	/**
	  * CheckBoolBuffer
	  * Checks to see if the buffer is true or not, sets the buffer to false,
	  * then returns the value.
	  */
	bool CheckBoolBuffer(void* Buffer);

	UHolodeckServer* Server;
	const int SINGLE_BOOL = 1;
	const int TELEPORT_COMMAND_SIZE = 3;
};
