// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "AIController.h"
#include "HolodeckPawnControllerInterface.h"
#include "HolodeckAgentInterface.h"
#include "HolodeckControlScheme.h"
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
class HOLODECK_API AHolodeckPawnController : public AHolodeckPawnControllerInterface
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
	void* Subscribe(const FString& AgentName, const FString& SensorName, int NumItems, int ItemSize) override;

	/**
	  * GetActionBuffer
	  * Gets the action buffer for this agent.
	  * @param AgentName the name of the agent to subscribe an action buffer for.

	  */
	void AllocateBuffers(const FString& AgentName);

	virtual void AddControlSchemes() override { check(0 && "You must override AddControlSchemes"); };

	/**
	* ExecuteTeleport
	* Tells the controlled agent to teleport to the location in the shared memory.
	*/
	virtual void ExecuteTeleport() override;

	/**
	* SetServer
	* Sets the server object within this object.
	*/
	virtual void SetServer(UHolodeckServer* const ServerParam) override;

protected:
	void* ActionBuffer;
	uint8* ControlSchemeIdBuffer;
	float* TeleportBuffer;
	float* RotationBuffer;
	uint8* ShouldTeleportBuffer;

	UPROPERTY()
	TArray<UHolodeckControlScheme*> ControlSchemes;
	AHolodeckAgentInterface* ControlledAgent;

private:
	/**
	* GetServer
	* Sets the server object within this object.
	*/
	void UpdateServerInfo();

	/**
	  * CheckBoolBuffer
	  * Checks to see if the buffer is true or not, sets the buffer to false,
	  * then returns the value.
	  */
	bool CheckBoolBuffer(void* Buffer);

	UHolodeckServer* Server;
	const int SINGLE_BOOL = 1;
	const int TELEPORT_COMMAND_SIZE = 3;
	const int ROTATE_COMMAND_SIZE = 3;

};
